import argparse
import json
from pathlib import Path
import re
import shlex
import shutil
import subprocess
import tempfile
import time

ROOT = Path(__file__).resolve().parents[1]
SCRIPTS = ROOT / "tests/e2e_scripts"
GROUPS = {
    "screens": [134, 136, 137, 148, 150, 151, 180, 184, 185, 192, 198, 199, 202, 204, 206, 209, 211, 219, 230],
    "overlays": [160, 182, 193, 207, 223, 233, 238, 267],
    "text": [251, 260, 261, 265],
    "new": [281, 282],
    "profiler": [156, 271],
}


def csv_values(values):
    return [part for value in values for part in value.split(",") if part]


def select_scripts(args):
    groups = csv_values(args.group)
    unknown = set(groups) - GROUPS.keys()
    if unknown:
        raise ValueError("Unknown groups: " + ", ".join(sorted(unknown)))
    ids = {int(value) for value in csv_values(args.ids)}
    selected_ids = set().union(*(set(GROUPS[group]) for group in (groups or GROUPS)))
    if ids:
        selected_ids = selected_ids & ids if groups else ids
    screens = set(csv_values(args.screen))
    result = []
    for path in sorted(SCRIPTS.glob("*.e2e")):
        prefix = path.name.split("_", 1)[0]
        if not prefix.isdigit():
            continue
        number = int(prefix)
        if number not in selected_ids:
            continue
        match = re.search(r"^goto_screen\s+(\S+)", path.read_text(), re.MULTILINE)
        screen = match.group(1) if match else ""
        if screens and screen not in screens:
            continue
        result.append((path, screen))
    missing = selected_ids - {int(path.name.split("_", 1)[0]) for path, _ in result}
    if missing and not screens:
        raise ValueError("Missing test IDs: " + ", ".join(map(str, sorted(missing))))
    if not result and not args.benchmark_only:
        raise ValueError("No scripts match the requested selection")
    return result


def prepare_script(path, work, generated, external, backups):
    lines = []
    for number, line in enumerate(path.read_text().splitlines(), 1):
        tokens = shlex.split(line, comments=True)
        if not tokens:
            lines.append(line)
            continue
        if tokens[0] == "screenshot":
            stem = f"{work.name}_{path.stem}_{number}"
            target = Path("/tmp") / f"e2e_screenshot_{stem}.png"
            if target.exists():
                raise FileExistsError(target)
            external.append(target)
            line = f"screenshot {stem}"
        elif tokens[0] == "audit_ui":
            stem = work / "artifacts" / f"{path.stem}_{number}"
            line = "audit_ui " + str(stem) + (" crop" if len(tokens) > 2 and tokens[2] == "crop" else "")
        elif tokens[0] == "validate_screen":
            for prefix in ("validate_", "validate_FAILED_"):
                target = Path("/tmp") / f"{prefix}{tokens[1]}.png"
                if target in generated:
                    continue
                generated.add(target)
                if target.exists():
                    backup = work / "preserved" / target.name
                    shutil.copy2(target, backup)
                    backups[target] = backup
        lines.append(line)
    (work / "scripts" / path.name).write_text("\n".join(lines) + "\n")


def benchmark_scripts(work, frames):
    for mode in ("hidden_idle", "shown_idle", "hidden_jumps", "shown_jumps"):
        shown = mode.startswith("shown")
        lines = ["goto_screen virtual_list_lab", "resize 1280 720", "wait 0.3", "focus_ui vl_start", "key ENTER", "wait 0.1", 'expect_text "Visible rows 0 to 21 of 10,000"', "key F3", "wait 0.2", "click_ui profiler_record", "click_ui profiler_reset", "wait 0.1"]
        if not shown:
            lines += ["key F3", "wait 0.1"]
        phases = ("start", "middle", "end", "start") if mode.endswith("jumps") else ("start",)
        for index, position in enumerate(phases):
            if mode.endswith("jumps"):
                if shown:
                    lines += ["key F3", "wait_frames 2"]
                lines += [f"focus_ui vl_{position}", "key ENTER", "wait_frames 2"]
                expected = {"start": "0 to 21", "middle": "5000 to 5021", "end": "9978 to 9999"}[position]
                lines += [f'expect_text "Visible rows {expected} of 10,000"']
                if shown:
                    lines += ["key F3", "wait_frames 2"]
            visibility = "expect_text" if shown else "expect_no_text"
            lines += [f'{visibility} "Live profiler / virtual_list_lab"']
            lines += [f"wait_frames {frames}", f"audit_ui {work}/artifacts/benchmark_{mode}_{index}", "wait_frames 2"]
        if not shown:
            lines += ["key F3", "wait_frames 2"]
        lines += ["click_ui profiler_record", "wait_frames 2", f"audit_ui {work}/artifacts/benchmark_{mode}_result", "wait_frames 2", "key F3"]
        (work / "scripts" / f"990_benchmark_{mode}.e2e").write_text("\n".join(lines) + "\n")


def read_benchmarks(work):
    records = []
    for path in sorted((work / "artifacts").glob("benchmark_*.json")):
        data = json.loads(path.read_text())
        elements = data.get("elements", [])
        labels = []
        for item in elements:
            label = item.get("label", "")
            if not label:
                continue
            labels.append({"name": item.get("name"), "parent": item["parent"], "text": label,
                           "numbers": [float(value.replace(",", "")) for value in re.findall(r"[-+]?\d[\d,]*(?:\.\d+)?(?:e[-+]?\d+)?", label, re.IGNORECASE)]})
        records.append({"sample": path.stem, "elements": len(elements), "focus_targets": len(data.get("candidates", [])), "labels": labels})
    return {"samples": records, "notes": [
        "Measured in one headless app process; results depend on machine load and build optimization.",
        "Hidden result snapshots briefly open the profiler to stop recording and read its values; those few frames are included.",
        "Shown jump samples briefly hide the profiler to activate the list's gated controls.",
    ]}


def run_logged(command, log_path, root=ROOT):
    print("Running " + shlex.join(command), flush=True)
    started = time.monotonic()
    process = subprocess.Popen(command, cwd=root, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors="replace")
    try:
        with log_path.open("w") as output:
            for line in process.stdout:
                output.write(line)
                if any(marker in line for marker in ("[PASS]", "[FAIL]", "Scripts passed:", "Scripts failed:")):
                    print(line.rstrip(), flush=True)
        return process.wait(), time.monotonic() - started
    except BaseException:
        process.terminate()
        try:
            process.wait(timeout=10)
        except subprocess.TimeoutExpired:
            process.kill()
            process.wait()
        raise


def finish_artifacts(work, external, generated, backups):
    for path in external:
        if path.exists():
            shutil.move(str(path), work / "artifacts" / path.name)
    for path in generated:
        if path.exists():
            shutil.copy2(path, work / "artifacts" / path.name)
        if path in backups:
            shutil.copy2(backups[path], path)
        else:
            path.unlink(missing_ok=True)


def main():
    parser = argparse.ArgumentParser(description="Run the current screen review tests in one low-priority headless process.")
    parser.add_argument("--screen", action="append", default=[], help="Screen names, comma-separated or repeated")
    parser.add_argument("--group", action="append", default=[], help="Groups: " + ", ".join(GROUPS))
    parser.add_argument("--ids", action="append", default=[], help="Script IDs, comma-separated or repeated")
    parser.add_argument("--list", action="store_true", help="List selected scripts without running them")
    parser.add_argument("--build", action="store_true", help="Build once with nice -n 10 make -j2")
    parser.add_argument("--binary", type=Path, default=ROOT / "output/ui_tester.exe")
    parser.add_argument("--keep-artifacts", action="store_true")
    parser.add_argument("--report", type=Path, help="Keep a JSON summary at this path")
    parser.add_argument("--benchmark", action="store_true", help="Add 10,000-row idle/jump samples with the profiler hidden and shown")
    parser.add_argument("--benchmark-only", action="store_true")
    parser.add_argument("--benchmark-frames", type=int, default=240)
    args = parser.parse_args()
    try:
        selected = [] if args.benchmark_only else select_scripts(args)
    except ValueError as error:
        parser.error(str(error))
    if args.benchmark_frames < 1:
        parser.error("--benchmark-frames must be positive")
    if args.list:
        for path, screen in selected:
            print(f"{path.name}\t{screen}")
        if args.benchmark or args.benchmark_only:
            print("4 benchmark scripts: hidden/shown, idle/jumps")
        return 0
    work = Path(tempfile.mkdtemp(prefix="wm-review-"))
    for directory in ("scripts", "artifacts", "preserved"):
        (work / directory).mkdir()
    generated, external, backups = set(), [], {}
    summary = {"scripts": [path.name for path, _ in selected], "status": "interrupted", "failures": []}
    result = 1
    settings_path = ROOT / "settings.json"
    settings_backup = work / "preserved" / "settings.json"
    settings_existed = settings_path.exists()
    if settings_existed:
        shutil.copy2(settings_path, settings_backup)
    try:
        for path, _ in selected:
            prepare_script(path, work, generated, external, backups)
        if args.benchmark or args.benchmark_only:
            benchmark_scripts(work, args.benchmark_frames)
        if args.build:
            result, elapsed = run_logged(["nice", "-n", "10", "make", "-j2"], work / "build.log")
            summary["build_seconds"] = elapsed
            if result:
                summary["status"] = "build_failed"
                print("Build failed:\n" + "\n".join((work / "build.log").read_text().splitlines()[-35:]))
                return result
        result, elapsed = run_logged(["nice", "-n", "10", str(args.binary.resolve()), "--test-script-dir", str(work / "scripts"), "--headless"], work / "tests.log")
        log = re.sub(r"\x1b\[[0-9;]*m", "", (work / "tests.log").read_text())
        summary.update(status="passed" if result == 0 else "failed", exit_code=result, seconds=elapsed)
        summary["passed"] = re.findall(r"\[PASS\]\s+([^\n]+)", log)
        summary["failures"] = [line for line in log.splitlines() if re.search(r"\[FAIL\]|\[E2E ERROR\]|TIMEOUT|ASSERT.*FAIL|assertion failed|error:|LOG_ERROR|timed out|Command failed", line, re.IGNORECASE)]
        if args.benchmark or args.benchmark_only:
            summary["benchmark"] = read_benchmarks(work)
        print(f"{summary['status']}: {len(summary['passed'])} scripts passed in {elapsed:.1f}s")
        for line in summary["failures"][-40:]:
            print(line)
        if result and not summary["failures"]:
            print("\n".join(log.splitlines()[-35:]))
        return result
    finally:
        if settings_existed:
            shutil.copy2(settings_backup, settings_path)
        else:
            settings_path.unlink(missing_ok=True)
        finish_artifacts(work, external, generated, backups)
        if args.report:
            args.report.parent.mkdir(parents=True, exist_ok=True)
            args.report.write_text(json.dumps(summary, indent=2) + "\n")
            print("Report: " + str(args.report.resolve()))
        if args.keep_artifacts:
            (work / "summary.json").write_text(json.dumps(summary, indent=2) + "\n")
            print("Artifacts: " + str(work))
        else:
            shutil.rmtree(work)


if __name__ == "__main__":
    raise SystemExit(main())
