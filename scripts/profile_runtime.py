import argparse
import json
from pathlib import Path
import platform
import os
import resource
import re
import shutil
import subprocess
import tempfile
import time


def run(binary, root, work, name, frames, cycles, timeout, frame_metrics=False):
    run_dir = work / name
    run_dir.mkdir()
    (run_dir / 'resources').symlink_to(root / 'resources', target_is_directory=True)
    if (root / 'settings.json').exists():
        shutil.copy2(root / 'settings.json', run_dir / 'settings.json')
    phases = [('ready', 'buttons', 2)]
    if frames:
        phases += [('idle', 'buttons', frames), ('active', 'chart_lab', frames)]
        phases += [(f'switch_{index}_{screen}', screen, 12) for index in range(cycles) for screen in ('buttons', 'chart_lab')]
        phases += [('returned_idle', 'buttons', frames)]
    lines = []
    markers = []
    for index, (phase, screen, count) in enumerate(phases):
        marker = run_dir / f'{index:02d}_{phase}'
        markers.append((phase, marker.with_suffix('.json'), count))
        lines += [f'goto_screen {screen}', f'wait_frames {count}', f'audit_ui {marker}', 'wait_frames 2']
    if frame_metrics:
        lines += ['key F3', 'wait_frames 2', 'click_ui profiler_record', 'wait_frames 2', f'audit_ui {run_dir}/profiler', 'wait_frames 2']
    script = run_dir / 'workload.e2e'
    script.write_text('\n'.join(lines) + '\n')
    command = ['nice', '-n', '10', str(binary), '--headless', '--quiet', '--test-script', str(script), '--timeout', str(timeout), *(['--profile'] if frame_metrics else [])]
    samples = []
    observations = []
    before = resource.getrusage(resource.RUSAGE_CHILDREN)
    start = time.monotonic()
    with (run_dir / 'run.log').open('w') as log:
        process = subprocess.Popen(command, cwd=run_dir, stdout=log, stderr=subprocess.STDOUT, env={**os.environ, "WM_PROFILE_STARTUP": "1"})
        try:
            while process.poll() is None:
                elapsed = time.monotonic() - start
                if elapsed > timeout:
                    process.kill()
                    raise TimeoutError(f'{name} exceeded {timeout}s; see {run_dir}/run.log')
                result = subprocess.run(['ps', '-o', 'rss=,%cpu=', '-p', str(process.pid)], capture_output=True, text=True)
                values = result.stdout.split()
                sample = {'seconds': elapsed, 'rss_mib': float(values[0]) / 1024, 'cpu_percent': float(values[1])} if len(values) == 2 else {'seconds': elapsed}
                samples.append(sample)
                while len(observations) < len(markers) and markers[len(observations)][1].exists():
                    phase, path, count = markers[len(observations)]
                    try:
                        data = json.loads(path.read_text())
                    except json.JSONDecodeError:
                        break
                    observations.append({'phase': phase, 'frames': count, 'seconds': elapsed,
                                         'rss_mib': sample.get('rss_mib'), 'ui_elements': len(data.get('elements', []))})
                time.sleep(0.05)
        finally:
            if process.poll() is None:
                process.kill()
            process.wait()
    end = time.monotonic() - start
    after = resource.getrusage(resource.RUSAGE_CHILDREN)
    cpu = after.ru_utime + after.ru_stime - before.ru_utime - before.ru_stime
    record = {'name': name, 'command': command, 'exit_code': process.returncode, 'wall_seconds': end,
              'cpu_seconds_with_sampler': cpu, 'average_cpu_percent_with_sampler': 100 * cpu / end,
              'sampled_peak_rss_mib': max((sample.get('rss_mib', 0) for sample in samples), default=0),
              'first_usable_seconds_upper_bound': observations[0]['seconds'] if observations else None,
              'observations': observations, 'samples': samples}
    record['font_loads'] = [{'name': match[0], 'milliseconds': float(match[1]), 'glyphs': int(match[2])} for match in re.findall(r'\[startup\] font=(\S+) ms=([\d.e+-]+) glyphs=(\d+)', (run_dir / 'run.log').read_text())]
    profiler_path = run_dir / 'profiler.json'
    if profiler_path.exists():
        record['profiler_labels'] = [item['label'] for item in json.loads(profiler_path.read_text()).get('elements', []) if item.get('label')]
    (run_dir / 'measurement.json').write_text(json.dumps(record, indent=2) + '\n')
    if process.returncode:
        raise RuntimeError(f'{name} failed: {run_dir}/run.log')
    return record


def main():
    parser = argparse.ArgumentParser(description='Bounded headless startup, CPU and RSS audit with isolated settings.')
    parser.add_argument('--binary', type=Path, required=True)
    parser.add_argument('--root', type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument('--output', type=Path)
    parser.add_argument('--startup-runs', type=int, default=3)
    parser.add_argument('--frames', type=int, default=180)
    parser.add_argument('--switch-cycles', type=int, default=5)
    parser.add_argument('--frame-metrics', action='store_true', help='Record aggregate frame timings through the existing profiler overlay')
    parser.add_argument('--timeout', type=float, default=120)
    args = parser.parse_args()
    if not 1 <= args.startup_runs <= 20 or not 1 <= args.frames <= 10000 or not 0 <= args.switch_cycles <= 100 or not 0 < args.timeout <= 600:
        parser.error('Require runs 1..20, frames 1..10000, cycles 0..100, timeout (0, 600]')
    work = args.output.resolve() if args.output else Path(tempfile.mkdtemp(prefix='wm-runtime-'))
    work.mkdir(parents=True, exist_ok=True)
    results = []
    for index in range(args.startup_runs):
        result = run(args.binary.resolve(), args.root.resolve(), work, f'startup_{index}', 0, 0, args.timeout)
        results.append(result)
        print(f"{result['name']}: ready <= {result['first_usable_seconds_upper_bound']:.3f}s; peak RSS {result['sampled_peak_rss_mib']:.1f} MiB", flush=True)
    results.append(run(args.binary.resolve(), args.root.resolve(), work, 'workload', args.frames, args.switch_cycles, args.timeout, args.frame_metrics))
    report = {'platform': platform.platform(), 'binary': str(args.binary.resolve()), 'runs': results,
              'notes': ['The first launch is filesystem-cache-uncontrolled, not a guaranteed cold launch. Later launches are warm repeats.',
                        'Startup ends at a UI audit after two rendered frames. Polling and audit overhead make it an upper bound.',
                        'RSS is sampled every ~50ms plus ps overhead. Process CPU totals include ps sampler subprocesses.',
                        'Headless software rendering is uncapped; CPU usage is not representative of a vsynced window.',
                        'Phase wall times include navigation, audit screenshots, E2E dispatch and polling. They are not isolated frame times.',
                        'Optional profiler timings cover the whole workload and include dispatch/audit frames, with collection enabled.',
                        'UI element counts cover the visible audit tree, not all retained ECS entities or GPU textures.',
                        'All runs use a separate working directory and a settings copy. Original settings are never written.']}
    (work / 'report.json').write_text(json.dumps(report, indent=2) + '\n')
    print(work / 'report.json')


if __name__ == '__main__':
    main()
