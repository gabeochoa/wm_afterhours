#!/bin/bash
set -euo pipefail
branch=$1; base=${2:-main}; body=$3
repo=/Users/gabeochoa/p/afterhours
git -C "$repo" push -u origin "$branch" 2>&1 | tail -1
title=$(git -C "$repo" log --reverse --format=%s "origin/$base..$branch" 2>/dev/null | head -1 || git -C "$repo" log --reverse --format=%s "$base..$branch" | head -1)
gh pr create -R gabeochoa/afterhours --head "$branch" --base "$base" --title "$title" --body-file "$body"
