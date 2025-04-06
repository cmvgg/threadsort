#!/bin/bash

EXECUTABLE="../../../build/tests"
CONFIG_FILE="../../config/valid_config.txt"
LOG_DIR="../../../logs_sh"

echo "--- Signal Handling Tests (Automated) ---"

mkdir -p "$LOG_DIR"

# Test 3.1: Send SIGINT
echo "Running test: 3.1 - Send SIGINT"
LOG_FILE_INT="$LOG_DIR/signal_sigint.log"
{
    "$EXECUTABLE" -f "$CONFIG_FILE" &
    PID=$!
    sleep 1 # Give the program a moment to start
    kill -SIGINT "$PID"
    wait "$PID"
} > "$LOG_FILE_INT" 2>&1
EXIT_CODE_INT=$?
OUTPUT_INT=$(cat "$LOG_FILE_INT")

if [ "$EXIT_CODE_INT" -eq 0 ] && [[ "$OUTPUT_INT" =~ "Program terminated by user signal." ]]; then
    echo "  Result: PASSED (SIGINT)"
else
    echo "  Result: FAILED (SIGINT) - Exit Code: $EXIT_CODE_INT"
    echo "    Output:"
    cat "$LOG_FILE_INT"
fi
echo "  Log file: $LOG_FILE_INT"
echo ""

# Test 3.2: Send SIGTERM
echo "Running test: 3.2 - Send SIGTERM"
LOG_FILE_TERM="$LOG_DIR/signal_sigterm.log"
{
    "$EXECUTABLE" -f "$CONFIG_FILE" &
    PID=$!
    sleep 1 # Give the program a moment to start
    kill -SIGTERM "$PID"
    wait "$PID"
} > "$LOG_FILE_TERM" 2>&1
EXIT_CODE_TERM=$?
OUTPUT_TERM=$(cat "$LOG_FILE_TERM")

if [ "$EXIT_CODE_TERM" -eq 0 ] && [[ "$OUTPUT_TERM" =~ "Program terminated by system signal." ]]; then
    echo "  Result: PASSED (SIGTERM)"
else
    echo "  Result: FAILED (SIGTERM) - Exit Code: $EXIT_CODE_TERM"
    echo "    Output:"
    cat "$LOG_FILE_TERM"
fi
echo "  Log file: $LOG_FILE_TERM"
echo ""