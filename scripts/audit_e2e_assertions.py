import argparse
import json
from pathlib import Path
import shlex

INTERACTIONS = set('click click_ui click_text click_button right_click right_click_ui double_click_ui triple_click_ui key type tab shift_tab select_all mouse_down mouse_up drag_to scroll_wheel pinch set_slider action'.split())
NAVIGATION = {'goto_screen', 'next_screen', 'prev_screen', 'reset_test_state'}
LAYOUT_CHECKS = {'assert_ui', 'assert_ui_text', 'assert_within_parents', 'assert_no_overflow'}


def audit(path):
    commands = []
    for number, line in enumerate(path.read_text().splitlines(), 1):
        tokens = shlex.split(line, comments=True)
        if tokens:
            commands.append((number, tokens[0]))
    interactions = [line for line, command in commands if command in INTERACTIONS]
    checks = [line for line, command in commands if command.startswith(('expect_', 'assert_')) or command == 'validate_screen']
    behavioral = [line for line, command in commands if command.startswith('expect_')]
    pending = []
    unchecked = []
    for line, command in commands:
        if command in NAVIGATION:
            unchecked.extend(pending)
            pending = []
        if command in INTERACTIONS:
            pending.append(line)
        if line in checks:
            pending = []
    unchecked.extend(pending)
    if not interactions:
        status = 'no_interactions'
    elif not checks:
        status = 'assertion_free'
    elif not behavioral:
        status = 'layout_or_visual_checks_only'
    else:
        status = 'has_expectations'
    return {'script': path.name, 'status': status, 'interactions': interactions,
            'checks': checks, 'unchecked_before_navigation_or_end': unchecked}


def main():
    parser = argparse.ArgumentParser(description='Inventory E2E checks without treating screenshots as assertions.')
    parser.add_argument('directory', nargs='?', type=Path, default=Path('tests/e2e_scripts'))
    parser.add_argument('--exclude', type=Path, help='JSON map of script names to reviewed exclusion reasons')
    parser.add_argument('--json', type=Path)
    parser.add_argument('--fail-assertion-free', action='store_true')
    args = parser.parse_args()
    exclusions = json.loads(args.exclude.read_text()) if args.exclude else {}
    records = [audit(path) for path in sorted(args.directory.glob('*.e2e'))]
    if not records:
        parser.error('No E2E scripts found')
    names = {record['script'] for record in records}
    if set(exclusions) - names or any(not isinstance(reason, str) or not reason.strip() for reason in exclusions.values()):
        parser.error('Exclusions require existing script names and nonempty review reasons')
    for record in records:
        if record['script'] in exclusions:
            record['exclusion_reason'] = exclusions[record['script']]
    counts = {status: sum(record['status'] == status for record in records) for status in sorted({record['status'] for record in records})}
    report = {'counts': counts, 'excluded': len(exclusions), 'scripts': records, 'limits': [
        'An expectation after an interaction may still check unrelated text; this is an inventory, not proof of behavioral coverage.',
        'Layout and visual checks count as checks, but have their own category when no expect_* command exists.',
        'Unchecked tails include intentional cleanup. Review them before changing scripts.',
        'Navigation, resize, mouse movement and focus setup are excluded from the interaction count. Screenshots and dumps never count as assertions.',
        'No scripts are excluded by default. Explicit exclusions remain visible with their review reasons.'
    ]}
    print(json.dumps(counts, sort_keys=True))
    for record in records:
        if record['status'] == 'assertion_free':
            print(f"{record['script']}: interaction lines {record['interactions']}" + (f"; excluded: {record['exclusion_reason']}" if 'exclusion_reason' in record else ''))
    if args.json:
        args.json.write_text(json.dumps(report, indent=2) + '\n')
    return int(args.fail_assertion_free and any(record['status'] == 'assertion_free' and 'exclusion_reason' not in record for record in records))


if __name__ == '__main__':
    raise SystemExit(main())
