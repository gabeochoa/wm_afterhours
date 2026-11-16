#!/bin/bash
set -uo pipefail
b=$1; base=${2:-main}; body=$3; repo=/Users/gabeochoa/p/afterhours
git -C $repo fetch -q origin
echo "== $b on $base"; git -C $repo log --oneline origin/$base..$b
git -C $repo diff --stat origin/$base...$b | tail -1
ex=$(git -C $repo diff --name-only origin/$base...$b | grep -c '^examples/catalog/.*/main.cpp$')
echo "example main.cpp files: $ex"
c=$(git -C $repo diff origin/$base...$b -- src tests examples | grep -E '^\+' | grep -vE '^\+\+\+' | grep -cE '^\+\s*(//|/\*)')
echo "added comment lines: $c"
t=$(git -C $repo diff origin/$base...$b | grep -E '^\+' | grep -ciE 'TODO|FIXME|XXX')
echo "added TODO/FIXME: $t"
[ -f "$body" ] || { echo "missing body"; exit 1; }
if [ "${FORCE:-0}" != 1 ] && { [ "$c" -gt 0 ] || [ "$t" -gt 0 ]; }; then echo "STOP: review comments/TODOs first"; git -C $repo diff origin/$base...$b | grep -nE '^\+\s*(//|/\*)|TODO|FIXME' | head; exit 2; fi
/tmp/prs/open.sh "$b" "$base" "$body"
