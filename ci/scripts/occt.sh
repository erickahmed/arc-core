#!/usr/bin/env bash
set -euo pipefail

IMAGE_NAME="occt-builder:latest"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_PATH="$(cd "$SCRIPT_DIR/../../" && pwd)"
DOCKERFILE_PATH="$SCRIPT_DIR/../occt.Dockerfile"

# Check for --unit-testing argument
RUN_UNIT_TESTS=false
if [[ "${1-}" == "--unit-testing" ]]; then
    RUN_UNIT_TESTS=true
fi

echo "Building image..."
if ! docker build -f "$DOCKERFILE_PATH" -t "$IMAGE_NAME" "$PROJECT_PATH"; then
    echo "Image build failed." >&2
    exit 1
fi

echo "Listing /usr/local/bin..."
if ! docker run --rm "$IMAGE_NAME" ls /usr/local/bin; then
    echo "Listing /usr/local/bin failed." >&2
    exit 1
fi

if [ "$RUN_UNIT_TESTS" = true ]; then
    echo "Running unit tests..."
    if ! docker run --rm "$IMAGE_NAME" ctest --test-dir /app/build/unit-tests -V; then
        echo "Unit tests failed." >&2
        exit 1
    fi
fi
