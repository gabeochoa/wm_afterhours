#!/bin/bash
# Quick E2E test - runs all scripts without rebuilding
# Usage: ./scripts/run_e2e_quick.sh

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
E2E_SCRIPTS_DIR="$PROJECT_DIR/tests/e2e_scripts"

cd "$PROJECT_DIR"

if [[ ! -f "./build/ui_tester" ]]; then
    echo "Binary not found. Run 'make' first."
    exit 1
fi

echo "Running all E2E scripts..."
./build/ui_tester --test-script-dir "$E2E_SCRIPTS_DIR" --timeout 600

