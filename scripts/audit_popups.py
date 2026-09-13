import argparse
import json
from pathlib import Path
import subprocess
import tempfile


def probe_cases():
    cases = []

    def add(screen, name, steps="", tabs=14):
        cases.append((screen, name, steps, tabs))

    for name in ("basic", "form", "info", "confirm", "fyi", "stacked"):
        add(
            "modals",
            name,
            "click_ui "
            + {
                "basic": "btn_simple",
                "form": "btn_composable",
                "fyi": "btn_notice",
            }.get(name, "btn_" + name),
        )
    add(
        "modals",
        "nested",
        "click_ui btn_stacked\nwait 0.3\nclick_ui modals_settings_reset",
    )
    for name in ("login", "wizard", "settings", "feedback"):
        add("advanced_modals", name, "click_ui btn_" + name)

    for name in ("delete", "palette", "tos", "drawer", "sheet", "cookie"):
        add("real_world_modals", name, "click_ui btn_" + name)

    for name in ("confirm", "danger", "fyi", "info", "prompt"):
        add("dialog_" + name, name)
    for name, trigger in [
        ("account", "bk_manage"),
        ("restore", "bk_restore"),
        ("help", "bk_help"),
    ]:
        add("offsite_backup", name, "click_ui " + trigger)
    for trigger in ("tn_account", "tn_diagnostics"):
        add("secure_tunnel", trigger, "click_ui " + trigger)
    for name, trigger in [
        ("warn", "warn_btn"),
        ("block", "block_btn"),
        ("help", "aim_help"),
        ("fonts", "aim_my_menu"),
        ("people", "aim_people_menu"),
        ("start", "aim_start"),
    ]:
        add("aim_chat", name, "click_ui " + trigger, 40)
    for n in range(4):
        add("aim_chat", "menu" + str(n), "click_ui aim_menu_" + str(n), 40)
    add(
        "aim_chat",
        "info",
        "click_ui aim_people_menu\nwait 0.3\nclick_ui aim_menu_action_0",
        40,
    )
    add(
        "aim_chat",
        "clear",
        "click_ui aim_menu_0\nwait 0.3\nclick_ui aim_menu_action_0",
        40,
    )
    for name in ("play", "profile", "settings", "server"):
        add("media_library", name, "click_ui ml_" + name, 35)
    for name in ("language", "progress", "help", "terms", "about"):
        add(
            "casual_settings",
            name,
            "click_ui casual_" + ("support" if name == "help" else name),
        )
    for name in ("basic", "labeled", "many", "indicator", "default_gem", "styled"):
        add("dropdowns", name, "click_ui " + name + "_dropdown_trigger")
    for name in ("File", "Edit", "Bottom", "Corner"):
        add("menu_showcase", name, "click_ui menu_" + name + "_trigger")
    for name, trigger in [
        ("branch", "pv_trigger_pinned"),
        ("commit", "pv_trigger"),
        ("above", "pv_place_0"),
        ("right", "pv_place_1"),
        ("left", "pv_place_2"),
        ("bottom", "pv_trigger_low"),
    ]:
        add("popover_lab", name, "click_ui " + trigger)
    add(
        "context_menu_lab",
        "context",
        "key ESCAPE\nmouse_move 200 570\nwait 0.1\nright_click 200 570",
    )
    add("context_menu_lab", "corner", "key ESCAPE\nwait 0.1\nclick_ui cm_target_corner")
    add(
        "marlo_kart",
        "pause",
        "click_ui mk_start\nwait 0.2\nclick_ui mk_driver_next\nwait 0.2\nclick_ui mk_cup_start\nwait 0.2\nkey ESCAPE",
    )
    add("chart_lab", "navigator", "key GRAVE", 30)
    add("mini_motorways_settings", "tutorial", "click_ui mm_tutorial")
    for name in ("Inventory", "Research", "Crafting"):
        add("cozy_cafe", name, "click_ui tool_" + name)
    add("casual_settings", "credits", "click_ui casual_credits")
    return cases


def write_probes(root, cases):
    scripts = root / "scripts"
    scripts.mkdir(parents=True)
    (root / "cases.json").write_text(json.dumps(cases))
    for index, (screen, name, steps, tabs) in enumerate(cases):
        key = f"{index:03}_{screen}_{name}"
        lines = [f"goto_screen {screen}", "resize 1280 720", "wait 0.3"]
        if screen in ("menu_showcase", "popover_lab"):
            lines += ["key ESCAPE", "wait 0.1"]
        if steps:
            lines += [steps, "wait 0.3"]
        lines.append(f"audit_ui {root}/captures/{key}_open")
        for direction, command, count in [
            ("tab", "tab", tabs),
            ("back", "shift_tab", min(tabs, 16)),
        ]:
            for step in range(count):
                lines += [
                    command,
                    "wait 0.04",
                    f"audit_ui {root}/captures/{key}_{direction}{step+1:02} crop",
                ]
        lines += ["key ESCAPE", "wait 0.2", f"audit_ui {root}/captures/{key}_escape"]
        (scripts / f"{key}.e2e").write_text("\n".join(lines) + "\n")


def analyze(root):
    cases = json.loads((root / "cases.json").read_text())
    findings = []
    samples = 0
    modal_samples = 0
    popup_screens = {
        "dropdowns",
        "menu_showcase",
        "popover_lab",
        "context_menu_lab",
        "cozy_cafe",
        "chart_lab",
    }
    for index, (screen, name, _, _) in enumerate(cases):
        key = f"{index:03}_{screen}_{name}"
        paths = sorted((root / "captures").glob(f"{key}_*.json"))
        states = {
            p.stem.removeprefix(key + "_"): json.loads(p.read_text()) for p in paths
        }
        if "open" not in states or "escape" not in states:
            findings.append({"case": key, "problem": "missing open or Escape capture"})
            continue
        opened = states["open"]
        escaped = states["escape"]
        if screen not in popup_screens and not opened["modals"]:
            findings.append({"case": key, "problem": "dialog did not open"})
        if screen in popup_screens - {"chart_lab"} and not opened.get("popups"):
            findings.append({"case": key, "problem": "popup did not open"})
        for stage, data in states.items():
            samples += 1
            elements = {e["id"]: e for e in data["elements"]}

            def inside(entity, parent):
                seen = set()
                while entity in elements and entity not in seen:
                    if entity == parent:
                        return True
                    seen.add(entity)
                    entity = elements[entity]["parent"]
                return False

            visual = data.get("visual")
            ring = data.get("ring")
            if visual and ring:
                x, y, width, height = ring.get("bounds", ring["outer"])
                vw, vh = data["viewport"]
                problem = None
                if min(ring["inner"][2:]) <= 0:
                    problem = "degenerate focus ring"
                elif x < -1 or y < -1 or x + width > vw + 1 or y + height > vh + 1:
                    problem = "focus ring outside viewport"
                elif visual["clip"]:
                    cx, cy, cw, ch = visual["clip"]
                    if (
                        x < cx - 1
                        or y < cy - 1
                        or x + width > cx + cw + 1
                        or y + height > cy + ch + 1
                    ):
                        problem = "focus ring clipped by ancestor"
                if problem:
                    findings.append(
                        {
                            "case": key,
                            "stage": stage,
                            "problem": problem,
                            "target": visual.get("name", visual["id"]),
                        }
                    )
            if data["modals"]:
                modal_samples += 1
                top = data["modals"][-1]["id"]
                if not inside(data["focus_id"], top):
                    findings.append(
                        {
                            "case": key,
                            "stage": stage,
                            "problem": "focus outside top modal",
                        }
                    )
                if any(not inside(e["id"], top) for e in data["candidates"]):
                    findings.append(
                        {
                            "case": key,
                            "stage": stage,
                            "problem": "background remains focusable",
                        }
                    )
        if not opened["modals"]:
            continue
        if len(escaped["modals"]) != len(opened["modals"]) - 1:
            findings.append(
                {"case": key, "problem": "Escape did not close exactly one dialog"}
            )
        restore = opened["modals"][-1]["restore_focus"]
        if restore >= 0 and escaped["focus_id"] != restore:
            findings.append(
                {
                    "case": key,
                    "problem": "focus not restored to opener",
                    "expected": restore,
                    "actual": escaped["focus_id"],
                }
            )
    return {
        "cases": len(cases),
        "samples": samples,
        "modal_samples": modal_samples,
        "findings": findings,
    }


def run(root, binary, screen):
    cases = [case for case in probe_cases() if not screen or case[0] == screen]
    if not cases:
        raise ValueError(f"No popup probes for {screen}")
    write_probes(root, cases)
    with (root / "run.log").open("w") as log:
        process = subprocess.run(
            [
                "nice",
                "-n",
                "10",
                binary,
                "--headless",
                "--test-script-dir",
                str(root / "scripts"),
                "--time-scale",
                "4",
            ],
            stdout=log,
            stderr=subprocess.STDOUT,
        )
    result = analyze(root)
    result["runner_exit_code"] = process.returncode
    return result


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--existing", type=Path)
    parser.add_argument("--capture-dir", type=Path)
    parser.add_argument("--binary", default="./output/ui_tester.exe")
    parser.add_argument("--screen")
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    if args.existing:
        result = analyze(args.existing)
    elif args.capture_dir:
        args.capture_dir.mkdir(parents=True, exist_ok=True)
        result = run(args.capture_dir.resolve(), args.binary, args.screen)
    else:
        with tempfile.TemporaryDirectory(prefix="wm-popup-audit-") as directory:
            result = run(Path(directory), args.binary, args.screen)
    text = json.dumps(result, indent=2) + "\n"
    if args.output:
        args.output.write_text(text)
    else:
        print(text, end="")
    raise SystemExit(bool(result["findings"] or result.get("runner_exit_code", 0)))


if __name__ == "__main__":
    main()
