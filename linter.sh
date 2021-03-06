#!/bin/sh

LINTER="cpplint"
LINTER_FLAGS="--recursive --repository"
PROJECT_ROOT="catena-cpp"

LINTER_COMMAND="$LINTER $LINTER_FLAGS $PROJECT_ROOT"

SOURCE_FILES="$PROJECT_ROOT/catena/src"
HEADER_FILES="$PROJECT_ROOT/catena/include"

TEST_FILES="$PROJECT_ROOT/catena/test"

BENCHMARK_FILES="$PROJECT_ROOT/benchmarks"

SANITIZER_FILES="$PROJECT_ROOT/catena/sanitize"

echo ""
echo "HEADER FILES:"
eval "$LINTER_COMMAND $HEADER_FILES"

echo ""
echo "SOURCE FILES:"
eval "$LINTER_COMMAND $SOURCE_FILES"

echo ""
echo "TEST FILES:"
eval "$LINTER_COMMAND $TEST_FILES"

echo ""
echo "BENCHMARK FILES:"
eval "$LINTER_COMMAND $BENCHMARK_FILES"

echo ""
echo "SANITIZER FILES:"
eval "$LINTER_COMMAND $SANITIZER_FILES"
