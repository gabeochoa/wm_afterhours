import argparse
import hashlib
import json
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
AUDIT = ROOT / "docs/design-audit"


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def escape_cell(value):
    return str(value).replace("|", "\\|").replace("\n", " ")


def finding_table(findings, prefix):
    lines = ["| ID | Priority | Type | Visible evidence | Proposed change |",
             "|---|---|---|---|---|"]
    for item in findings:
        lines.append("| " + " | ".join(map(escape_cell, [
            f"{prefix}-{item['id']}", item["priority"], item["kind"],
            f"{item['area']}: {item['observation']}", item["change"]])) + " |")
    return lines


def build(check=False):
    screens = []
    for shard in ("screens-a.json", "screens-b.json", "screens-c.json"):
        screens.extend(json.loads((AUDIT / shard).read_text()))
    screens.sort(key=lambda screen: screen["id"])
    expected = {path.stem.removesuffix("_720p")
                for path in (ROOT / "screenshot-baselines/screens").glob("*.png")}
    actual = [screen["id"] for screen in screens]
    if len(actual) != len(set(actual)) or set(actual) != expected:
        raise ValueError(f"Screen coverage mismatch: missing={expected - set(actual)}, "
                         f"extra={set(actual) - expected}")
    mock_html = (ROOT / "mocks.html").read_text()
    mock_ids = set(re.findall(r"(?:\bid\s*:|^add\()\s*'(\w+)'", mock_html, re.MULTILINE))
    references = {}
    for match in re.finditer(r"\bid\s*:\s*'(\w+)'", mock_html):
        reference = re.search(r"\breference:\s*'([^']+)'", mock_html[match.end():match.end() + 240])
        if reference:
            references[match[1]] = reference[1]
    legacy = json.loads((AUDIT / "legacy.json").read_text())
    legacy_paths = {str(path.relative_to(ROOT)) for path in (ROOT / "baseline_screenshots").glob("*.png")}
    if {capture["baseline"] for capture in legacy} != legacy_paths:
        raise ValueError("Legacy capture coverage mismatch")
    total = defects = 0
    for screen in screens:
        path = ROOT / screen["baseline"]
        if path != ROOT / f"screenshot-baselines/screens/{screen['id']}_720p.png":
            raise ValueError(f"Unexpected baseline path: {path}")
        if screen["sha256"] != digest(path):
            raise ValueError(f"Baseline changed since review: {screen['id']}")
        screen["has_mock"] = screen["id"] in mock_ids
        screen["reference"] = references.get(screen["id"])
        screen["legacy"] = [capture for capture in legacy if capture["screen"] == screen["id"]]
        identifiers = [item["id"] for item in screen["findings"]]
        if not identifiers or len(identifiers) != len(set(identifiers)):
            raise ValueError(f"Missing or duplicate findings: {screen['id']}")
        for item in screen["findings"]:
            if item["kind"] not in {"defect", "proposal"} or item["priority"] not in {"P1", "P2", "P3"}:
                raise ValueError(f"Invalid finding classification: {screen['id']}-{item['id']}")
            if not all(item[field].strip() for field in ("area", "observation", "change")):
                raise ValueError(f"Empty finding: {screen['id']}-{item['id']}")
            defects += item["kind"] == "defect"
        total += len(screen["findings"])
    for capture in legacy:
        if capture["screen"] not in expected or capture["sha256"] != digest(ROOT / capture["baseline"]):
            raise ValueError(f"Stale or unmapped legacy capture: {capture['id']}")
    metadata = {
        "date": "2026-09-12", "screens": len(screens), "legacy_captures": len(legacy),
        "findings": total, "defects": defects, "proposals": total - defects,
        "legacy_findings": sum(len(capture["additional_findings"]) for capture in legacy),
        "screens_at_target": sum(len(screen["findings"]) >= 25 for screen in screens),
        "mock_screens": sum(screen["has_mock"] for screen in screens),
    }
    lines = ["# Baseline design gaps", "",
             "Audit of the committed 1280×720 screenshots on 2026-09-12. "
             "The review covers visible composition, typography, controls, artwork and the clarity of test demonstrations. "
             "Static captures cannot establish hover behavior, keyboard support, animation quality or performance.", "",
             f"{len(screens)} current screens, {len(legacy)} additional captures, {total} findings. "
             f"{defects} are classified as visible defects and {total - defects} as design proposals. "
             f"The additional captures contribute {metadata['legacy_findings']} more proposals. "
             f"{metadata['screens_at_target']} screens reach the requested 25 findings. "
             "Sparse screens have fewer where further findings would repeat an existing issue or invent a problem.", "",
             "All entries are open review candidates. Ownership starts in wm; none establishes a missing afterhours API. "
             "Preserve intentional test fixtures. A proposal is a possible improvement, not proof of broken behavior. "
             "P1 means unreadable or obscured content, P2 affects ordinary reading or comparison, and P3 is polish or added explanation.", "",
             "[Open the visual review](../../design-audit.html) · "
             "[Existing product mocks](../../mocks.html) · [Main gaps file](../AFTERHOURS_GAPS.md)", "",
             "## Coverage", "", "| Screen | Findings | Defects | Web mock |", "|---|---:|---:|---|"]
    for screen in screens:
        count = len(screen["findings"])
        count_defects = sum(item["kind"] == "defect" for item in screen["findings"])
        lines.append(f"| [{screen['id']}](#{screen['id'].replace('_', '-')}) | {count} | "
                     f"{count_defects} | {'yes' if screen['has_mock'] else 'not authored'} |")
    for screen in screens:
        lines += ["", f"## {screen['id'].replace('_', ' ')}", "",
                  f"[Current baseline](../../{screen['baseline']}) · "
                  f"[Visual review](../../design-audit.html#screen={screen['id']})", "",
                  f"Baseline SHA-256: `{screen['sha256']}`", "", screen["summary"], ""]
        lines += finding_table(screen["findings"], screen["id"])
        for capture in screen["legacy"]:
            lines += ["", f"### Additional capture: {capture['id']}", "",
                      f"[Image](../../{capture['baseline']})", "", capture["summary"]]
            if capture["additional_findings"]:
                lines += [""] + finding_table(capture["additional_findings"], capture["id"])
    outputs = {
        AUDIT / "GAPS.md": "\n".join(lines) + "\n",
        AUDIT / "data.js": "window.designAudit = " + json.dumps(
            {"metadata": metadata, "screens": screens}, ensure_ascii=False,
            separators=(",", ":")) + ";\n",
    }
    for path, content in outputs.items():
        if check:
            if not path.exists() or path.read_text() != content:
                raise ValueError(f"Generated audit is stale: {path.relative_to(ROOT)}")
            continue
        path.write_text(content)
    print(json.dumps(metadata, indent=2))


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    build(parser.parse_args().check)
