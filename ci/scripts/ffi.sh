#!/usr/bin/env bash
set -euo pipefail

IMAGE_NAME="ffi-builder:latest"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_PATH="$(cd "$SCRIPT_DIR/../../" && pwd)"
DOCKERFILE_PATH="$SCRIPT_DIR/../ffi.Dockerfile"

RUN_UNIT_TESTS=false
if [[ "${1-}" == "--tests" ]]; then
    RUN_UNIT_TESTS=true
fi

echo "Building base image..."
docker build --target ffi-base -f "$DOCKERFILE_PATH" -t "$IMAGE_NAME" "$PROJECT_PATH"

if [ "$RUN_UNIT_TESTS" = true ]; then
    echo "Building unit-testing stage..."
    docker build --target ffi-unit-testing -f "$DOCKERFILE_PATH" -t "$IMAGE_NAME" "$PROJECT_PATH"

    echo "Running tests..."
    docker run --rm -it "$IMAGE_NAME" cargo test
fi
