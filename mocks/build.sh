#!/usr/bin/env bash
# Regenerate the mock tool's inputs, then serve it.
#   ./mocks/build.sh          regenerate + serve on :8009
#   ./mocks/build.sh --no-serve
set -euo pipefail
cd "$(dirname "$0")/.."

make -j8

# Screenshot and dump come from the same frame; see headless_screenshots.cpp.
./output/ui_tester.exe --headless-screenshots \
  --image-output mocks/shots/ --dump-ui-json mocks/trees/ >/dev/null 2>&1

# Manifest, so index.html knows what to page through.
python3 - <<'PY'
import json, pathlib
names = sorted(p.stem for p in pathlib.Path("mocks/trees").glob("*.json"))
pathlib.Path("mocks/screens.json").write_text(json.dumps(names, indent=0))
print(f"{len(names)} screens")
PY

[[ "${1:-}" == "--no-serve" ]] && exit 0
echo "http://localhost:8009/mocks/"
exec python3 -m http.server 8009
