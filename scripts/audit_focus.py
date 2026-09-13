import argparse
import collections
import json
from pathlib import Path
import subprocess
import tempfile


def analyze(root):
    screens = []
    for path in sorted(root.glob("focus_test/*/focus.json")):
        data = json.loads(path.read_text())
        candidates = {item["id"]: item for item in data["initial"]["candidates"]}
        visited = collections.defaultdict(set)
        issues = []
        available = {}
        for sample in data["samples"]:
            visited[sample["direction"]].add(sample["focus_id"])
            available[sample["direction"]] = set(
                sample.get("candidate_ids", candidates)
            )
            if sample["direction"] != "forward":
                continue
            visual = sample.get("visual")
            if not visual:
                continue
            name = visual.get("name", str(visual["id"]))
            problems = []
            ring = sample.get("ring")
            if not ring:
                problems.append("no ring")
            else:
                x, y, w, h = ring["inner"]
                if w <= 0 or h <= 0:
                    problems.append("degenerate ring")
                x, y, w, h = ring.get("bounds", ring["outer"])
                vw, vh = sample["viewport"]
                if x < -1 or y < -1 or x + w > vw + 1 or y + h > vh + 1:
                    problems.append("ring outside viewport")
                if visual["clip"]:
                    cx, cy, cw, ch = visual["clip"]
                    if (
                        x < cx - 1
                        or y < cy - 1
                        or x + w > cx + cw + 1
                        or y + h > cy + ch + 1
                    ):
                        problems.append("ring clipped by ancestor")
            if visual["hidden"] or not visual["visible"]:
                problems.append("invisible focus target")
            if problems:
                issues.append(
                    {"name": name, "step": sample["step"], "problems": problems}
                )
        for direction in ("forward", "backward"):
            if not data[direction + "_cycle"]:
                issues.append(
                    {
                        "direction": direction,
                        "problems": ["traversal did not complete a cycle"],
                    }
                )
        persistent = (
            set(candidates)
            & available.get("forward", set())
            & available.get("backward", set())
        )
        changing = set(candidates) - persistent
        screens.append(
            {
                "screen": data["screen"],
                "candidates": len(candidates),
                "forward": len(visited["forward"]),
                "backward": len(visited["backward"]),
                "forward_cycle": data["forward_cycle"],
                "backward_cycle": data["backward_cycle"],
                "unreached": [
                    v.get("name", str(k))
                    for k, v in candidates.items()
                    if k in persistent and k not in visited["forward"]
                ],
                "retired_targets": [
                    candidates[k].get("name", str(k)) for k in sorted(changing)
                ],
                "direction_mismatch": sorted(
                    (visited["forward"] ^ visited["backward"]) & persistent
                ),
                "issues": issues,
            }
        )
    if not screens:
        raise ValueError(f"No focus captures found in {root}")
    return {
        "screens": screens,
        "screen_count": len(screens),
        "focus_targets": sum(s["forward"] for s in screens),
        "flagged_screens": sum(
            bool(s["issues"] or s["unreached"] or s["direction_mismatch"])
            for s in screens
        ),
    }


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--existing", type=Path)
    parser.add_argument("--binary", default="./output/ui_tester.exe")
    parser.add_argument("--screen")
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    if args.existing:
        result = analyze(args.existing)
    else:
        with tempfile.TemporaryDirectory(prefix="wm-focus-audit-") as directory:
            root = Path(directory)
            command = ["nice", "-n", "10", args.binary, "--focus-test"]
            if args.screen:
                command.append(args.screen)
            command += [
                "--focus-audit",
                "--max-tabs",
                "512",
                "--image-output",
                str(root),
            ]
            with (root / "run.log").open("w") as log:
                subprocess.run(
                    command, stdout=log, stderr=subprocess.STDOUT, check=True
                )
            result = analyze(root)
    text = json.dumps(result, indent=2) + "\n"
    if args.output:
        args.output.write_text(text)
    else:
        print(text, end="")
    raise SystemExit(bool(result["flagged_screens"]))


if __name__ == "__main__":
    main()
