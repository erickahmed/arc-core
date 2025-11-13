#!/usr/bin/env bash
set -euo pipefail

IMAGE_NAME="occt-build:latest"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_PATH="$(cd "$SCRIPT_DIR/../../" && pwd)"
DOCKERFILE_PATH="$SCRIPT_DIR/../occt.Dockerfile"

RUN_UNIT_TESTS=false
if [[ "${1-}" == "--unit-testing" ]]; then
    RUN_UNIT_TESTS=true
fi

echo "Building base image..."
docker build --target occt-base -f "$DOCKERFILE_PATH" -t "$IMAGE_NAME" "$PROJECT_PATH"

if [ "$RUN_UNIT_TESTS" = true ]; then
    echo "Building unit-testing stage..."
    docker build --target occt-unit-testing -f "$DOCKERFILE_PATH" -t "$IMAGE_NAME" "$PROJECT_PATH"

    echo "Running unit tests..."
    docker run --rm "$IMAGE_NAME" /app/build/unit-tests/tests
fi
