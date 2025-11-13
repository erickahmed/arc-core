#!/usr/bin/env bash
set -euo pipefail

IMAGE_NAME="ffi-build:latest"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_PATH="$(cd "$SCRIPT_DIR/../../" && pwd)"
DOCKERFILE_PATH="$SCRIPT_DIR/../ffi.Dockerfile"

RUN_UNIT_TESTS=false
if [[ "${1-}" == "--tests" ]]; then
    RUN_UNIT_TESTS=true
fi

# Try to understand where tf are we
if [[ -n "${GITHUB_REPOSITORY-}" ]]; then
    BASE_IMAGE="ghcr.io/$GITHUB_REPOSITORY/occt-build:latest"
    echo "Using GHCR image: $BASE_IMAGE"
    docker pull "$BASE_IMAGE" || true
else
    BASE_IMAGE="occt-build:latest"
    echo "Using local image: $BASE_IMAGE"
fi

echo "Building base image..."
docker build --target ffi-base -f "$DOCKERFILE_PATH" -t "$IMAGE_NAME" \
    --build-arg BASE_IMAGE="$BASE_IMAGE" \
    "$PROJECT_PATH"

if [ "$RUN_UNIT_TESTS" = true ]; then
    echo "Building unit-testing stage..."
    docker build --target ffi-unit-testing -f "$DOCKERFILE_PATH" -t "$IMAGE_NAME" \
        --build-arg BASE_IMAGE="$BASE_IMAGE" \
        "$PROJECT_PATH"

    echo "Running tests..."
    docker run --rm "$IMAGE_NAME" cargo test --verbose
fi
