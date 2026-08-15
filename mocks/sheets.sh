#!/usr/bin/env bash
# Render sheet.html to PNGs: every screen as raylib-vs-CSS side by side,
# ranked worst-disagreement first. Run ./mocks/build.sh --no-serve first.
#
#   ./mocks/sheets.sh [per_page]
set -euo pipefail
cd "$(dirname "$0")"

PER=${1:-8}
SCALE=0.55
OUT=sheets
CHROME="/Applications/Google Chrome.app/Contents/MacOS/Google Chrome"

[ -f screens.json ] || { echo "no screens.json -- run ./build.sh --no-serve" >&2; exit 1; }
rm -rf "$OUT"; mkdir -p "$OUT"

python3 -m http.server 8011 >/dev/null 2>&1 &
SRV=$!
trap 'kill $SRV 2>/dev/null || true' EXIT
sleep 1

TOTAL=$(python3 -c "import json;print(len(json.load(open('screens.json'))))")
PAGES=$(( (TOTAL + PER - 1) / PER ))
# 1280*scale per pane, two panes, gaps and padding.
W=$(python3 -c "print(int(1280*$SCALE*2)+70)")
# 720*scale plus caption and pane chrome, per row, plus the page heading.
H=$(python3 -c "print(int((720*$SCALE+66)*$PER)+60)")

echo "$TOTAL screens, $PER per sheet -> $PAGES sheets at ${W}x${H}"
for ((p=0; p<PAGES; p++)); do
  "$CHROME" --headless --disable-gpu --hide-scrollbars \
    --virtual-time-budget=20000 --window-size="$W,$H" \
    --screenshot="$OUT/sheet$(printf '%02d' $((p+1))).png" \
    "http://localhost:8011/sheet.html?page=$p&per=$PER&scale=$SCALE" >/dev/null 2>&1
  echo "  $OUT/sheet$(printf '%02d' $((p+1))).png"
done

# One tall image of just the screens that disagree -- the review artifact.
BADN=$("$CHROME" --headless --disable-gpu --virtual-time-budget=60000 \
  --dump-dom "http://localhost:8011/sheet.html?per=500" 2>/dev/null \
  | grep -o 'class="n bad"' | wc -l | tr -d ' ')
BH=$(python3 -c "print(int((720*0.3+58)*$BADN)+60)")
BW=$(python3 -c "print(int(1280*0.3*2)+70)")
echo "all $BADN disagreeing screens -> $OUT/all_disagreements.png (${BW}x${BH})"
"$CHROME" --headless --disable-gpu --hide-scrollbars \
  --virtual-time-budget=90000 --window-size="$BW,$BH" \
  --screenshot="$OUT/all_disagreements.png" \
  "http://localhost:8011/sheet.html?only=bad&per=500&scale=0.3" >/dev/null 2>&1

echo
echo "Ranking:"
"$CHROME" --headless --disable-gpu --virtual-time-budget=90000 \
  --dump-dom "http://localhost:8011/sheet.html?per=500" 2>/dev/null \
  | python3 -c '
import re,sys
html=sys.stdin.read()
for m in re.finditer(r"<b>([^<]+)</b><span class=\"n(?: bad)?\">(\d+)/(\d+)", html):
    print(f"  {m.group(2):>4}/{m.group(3):<5} {m.group(1)}")
' | head -20
