#!/bin/bash
# Run E2E tests
# Usage: ./scripts/run_e2e.sh [options]
#
# Options:
#   --script <path>     Run a single E2E script
#   --all               Run all E2E scripts in tests/e2e_scripts/
#   --timeout <frames>  Set timeout (default: 600)
#   -w, --width         Screen width (default: 1280)
#   -h, --height        Screen height (default: 720)

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
E2E_SCRIPTS_DIR="$PROJECT_DIR/tests/e2e_scripts"

# Default values
RUN_ALL=false
SCRIPT_PATH=""
TIMEOUT=600
WIDTH=1280
HEIGHT=720

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --script)
            SCRIPT_PATH="$2"
            shift 2
            ;;
        --all)
            RUN_ALL=true
            shift
            ;;
        --timeout)
            TIMEOUT="$2"
            shift 2
            ;;
        -w|--width)
            WIDTH="$2"
            shift 2
            ;;
        -h|--height)
            HEIGHT="$2"
            shift 2
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

# Build the project first
echo "Building project..."
cd "$PROJECT_DIR"
make -j8

# Run E2E tests
if [[ -n "$SCRIPT_PATH" ]]; then
    echo "Running E2E script: $SCRIPT_PATH"
    ./build/ui_tester --test-script "$SCRIPT_PATH" --timeout "$TIMEOUT" -w "$WIDTH" -h "$HEIGHT"
elif [[ "$RUN_ALL" == true ]]; then
    echo "Running all E2E scripts in $E2E_SCRIPTS_DIR"
    ./build/ui_tester --test-script-dir "$E2E_SCRIPTS_DIR" --timeout "$TIMEOUT" -w "$WIDTH" -h "$HEIGHT"
else
    echo "Usage: $0 --script <path> | --all [--timeout <frames>] [-w <width>] [-h <height>]"
    echo ""
    echo "Available E2E scripts:"
    ls -1 "$E2E_SCRIPTS_DIR"/*.e2e 2>/dev/null || echo "  (none found)"
    exit 1
fi

