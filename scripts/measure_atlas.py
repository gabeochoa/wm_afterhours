import argparse
import csv
import json
import os
from pathlib import Path
import shutil
import statistics
import subprocess


def measure(binary, root, work, probe, screen):
    work.mkdir(parents=True)
    (work / 'resources').symlink_to(root / 'resources', target_is_directory=True)
    shutil.copyfile(root / 'settings.json', work / 'settings.json')
    script = work / 'frames.e2e'
    script.write_text(f'goto_screen {screen}\nwait_frames 360\n')
    environment = {**os.environ, 'DYLD_INSERT_LIBRARIES': str(probe),
                   'WM_GL_PROBE_OUTPUT': str(work / 'gl.csv'), 'WM_GL_PROBE_VSYNC': '1'}
    with (work / 'run.log').open('w') as log:
        subprocess.run([str(binary), '--quiet', '--test-script', str(script)],
                       cwd=work, env=environment, preexec_fn=lambda: os.nice(max(0, 10 - os.nice(0))),
                       stdout=log, stderr=subprocess.STDOUT, check=True, timeout=90)
    with (work / 'gl.csv').open() as file:
        frames = list(csv.DictReader(file))[100:-30]
    if len(frames) < 200 or any(int(f['swap_interval']) != 1 for f in frames):
        raise RuntimeError(f'Insufficient settled vsynced frames: {work}')
    if any(int(f['draw_calls']) == 0 for f in frames):
        raise RuntimeError(f'Draw-call hook did not observe rendering: {work}')
    times = sorted(float(f['frame_ms']) for f in frames)
    return {'screen': screen, 'frames': len(frames), 'binary': str(binary),
            'draw_calls_median': statistics.median(int(f['draw_calls']) for f in frames),
            'texture_bind_calls_median': statistics.median(int(f['texture_bind_calls']) for f in frames),
            'frame_ms_median': statistics.median(times),
            'frame_ms_p95': times[int(.95 * (len(times) - 1))]}


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--before', required=True, type=Path)
    parser.add_argument('--after', required=True, type=Path)
    parser.add_argument('--probe', required=True, type=Path)
    parser.add_argument('--output', required=True, type=Path)
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[1]
    results = []
    for repeat in range(2):
        variants = [('before', args.before), ('after', args.after)]
        if repeat:
            variants.reverse()
        for screen in ['cozy_cafe', 'images']:
            for name, binary in variants:
                result = measure(binary.resolve(), root,
                                 args.output.resolve() / f'{screen}_{name}_{repeat}',
                                 args.probe.resolve(), screen)
                result.update(variant=name, repeat=repeat)
                results.append(result)
                print(json.dumps(result), flush=True)
    (args.output / 'report.json').write_text(json.dumps(results, indent=2) + '\n')


if __name__ == '__main__':
    main()
