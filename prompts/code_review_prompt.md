# Code review prompt

Review the diff and affected callers for correctness, state ownership, lifetime,
units, input behavior and regressions. Prefer early returns and existing APIs.
Do not add comments unless needed to correct a false existing comment.

For each finding, give the file/line, triggering case, effect and smallest fix.
Separate proven bugs from questions. Run relevant checks with `nice -n 10` and
at most two build jobs. Keep feedback short and fold fixes into their original commits.
