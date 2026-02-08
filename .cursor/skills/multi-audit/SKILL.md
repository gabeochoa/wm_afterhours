---
name: multi-audit
description: Take screenshots of all screens, run 4 parallel design audits (Apple HIG, Google Material, Sun Java L&F, Microsoft Win95), then consolidate and rank feedback by frequency for triage.
---

# Multi-Audit: Parallel Design Review

Capture screenshots of every screen, fan them out to 4 design-guideline auditors running in parallel, collect their feedback, and produce a single consolidated report ranked by how many auditors flagged each issue.

## When to Use

- `/multi-audit` — Full audit of all screens against all 4 guidelines
- `/multi-audit:screen <name>` — Audit a single screen against all 4 guidelines
- When you want cross-guideline consensus on what to fix first
- Before a major release or after adding new screens

## The Process

### Step 1: Capture Screenshots

1. Build the project (`make`)
2. Run `python3 screenshot_all_screens.py` to capture all screens
3. Screenshots land in `/tmp/ui_showcase_screenshots/*.png`
4. If a specific screen was requested, only capture that one

### Step 2: Fan Out to 4 Auditors

For each screenshot, spin up **4 subagents in parallel**, one per guideline:

| Auditor | Skill | Guidelines Doc | Output File |
|---------|-------|---------------|-------------|
| Apple HIG | `/audit-design` | `docs/apple_hig.md` | `docs/{screen}_audit_apple.md` |
| Google Material | `/audit-google-design` | `docs/google_hig.md` | `docs/{screen}_audit_google.md` |
| Sun Java L&F | `/audit-sun-design` | `docs/sun_java_look_and_feel_hig.md` | `docs/{screen}_audit_sun.md` |
| Microsoft Win95 | `/audit-win95-design` | `docs/microsoft_win95_hig.md` | `docs/{screen}_audit_microsoft.md` |

**Each auditor receives these instructions:**

```
You are auditing the UI screenshot at: {screenshot_path}

Read the design guidelines at: {guidelines_doc}

Look at the screenshot and produce exactly 5 issues to fix.
Each issue must have:
1. A short title (one line)
2. What's wrong (specific observation referencing the screenshot)
3. Which guideline it violates (with section reference)
4. How to fix it (concrete, actionable recommendation)

Rank them from most impactful to least impactful.

Output format — write to {output_file}:

# {Guideline Name} Audit: {screen_name}

## 1. {Title}
**Problem:** {What's wrong}
**Guideline:** {Section reference}
**Fix:** {How to fix it}

## 2. {Title}
...

## 3. {Title}
...

## 4. {Title}
...

## 5. {Title}
...
```

### Step 3: Wait for All Auditors

Wait for all 4 subagents to complete and write their output files.

### Step 4: Consolidate and Rank

Once all auditor files exist, read them all and produce a single consolidated report.

**Consolidation rules:**

1. **Group by theme** — Merge issues that describe the same underlying problem
   (e.g., "low contrast text" from Apple + "text below 4.5:1 ratio" from Google = same issue)
2. **Count agreement** — Note how many of the 4 auditors flagged each issue (4/4, 3/4, 2/4, 1/4)
3. **Sort by frequency** — Most-agreed-upon issues first
4. **Within same frequency, sort by severity** — Critical > Major > Minor

**Write the consolidated report to:** `docs/{screen}_audit_consolidated.md`

Use this format:

```markdown
# Consolidated Design Audit: {screen_name}

**Screens audited:** {count}
**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

## Agreed by 4/4 auditors

### 1. {Issue title}
**Problem:** {Merged description}
**Flagged by:** Apple (#{n}), Google (#{n}), Sun (#{n}), Microsoft (#{n})
**Fix:** {Best recommendation from the 4}

---

## Agreed by 3/4 auditors

### 2. {Issue title}
...

---

## Agreed by 2/4 auditors

...

---

## Flagged by 1 auditor only

...

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | {n} | **Do** — universal consensus |
| 3/4 | {n} | **Probably do** — strong consensus |
| 2/4 | {n} | **Maybe** — review individually |
| 1/4 | {n} | **Skip unless compelling** |
```

### Step 5: Present for Triage

Present the consolidated report to the user. For each issue, the user decides:
- **Yes** — Will fix
- **Maybe** — Might fix later
- **No** — Won't fix

Update the consolidated doc with the triage decisions.

---

## Example Invocation

```
/multi-audit:screen cozy_cafe
```

This will:
1. Capture `cozy_cafe.png`
2. Spin up 4 subagents in parallel
3. Each writes `docs/cozy_cafe_audit_{apple,google,sun,microsoft}.md`
4. Consolidate into `docs/cozy_cafe_audit_consolidated.md`
5. Present ranked issues for triage

---

## Notes

- Each auditor MUST produce exactly 5 issues per screenshot — no more, no less
- Auditors should focus on **actionable, specific** feedback, not generic advice
- The consolidation step is the key value — it surfaces issues that multiple independent perspectives agree on
- Screenshots are at 1280x720 (720p) by default
- If a screen has no screenshot yet, capture it first with `output/ui_tester.exe --screen={name} --headless --frames=3`
