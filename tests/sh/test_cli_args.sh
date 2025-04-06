#!/bin/bash

EXECUTABLE="../../../build/tests"
LOG_DIR="../../../logs_sh"

run_test_help() {
    TEST_NAME="$1"
    COMMAND="$2"
    LOG_FILE="$LOG_DIR/${TEST_NAME// /_}.log"
    echo "Running test: $TEST_NAME"
    echo "  Executing: $COMMAND"
    ACTUAL_OUTPUT=$("$COMMAND")
    EXIT_CODE=$?
    if [ "$EXIT_CODE" -eq 0 ] && [[ "$ACTUAL_OUTPUT" =~ "Use: .\/threadsort -f \| --file <path_to_config file>" ]]; then
        echo "  Result: PASSED" >> "$LOG_FILE"
    else
        echo "  Result: FAILED" >> "$LOG_FILE"
        echo "    Exit Code: $EXIT_CODE (Expected: 0)" >> "$LOG_FILE"
        echo "    Expected Output (Regex): 'Use: .\/threadsort -f \| --file <path_to_config file>'" >> "$LOG_FILE"
        echo "    Actual Output:" >> "$LOG_FILE"
        echo "    --------------------" >> "$LOG_FILE"
        echo "$ACTUAL_OUTPUT" >> "$LOG_FILE"
        echo "    --------------------" >> "$LOG_FILE"
    fi
    echo "  Log file: $LOG_FILE"
    echo ""
}

run_test_incorrect_usage() {
    TEST_NAME="$1"
    COMMAND="$2"
    EXPECTED_OUTPUT_REGEX="$3"
    EXPECTED_EXIT_CODE="$4"
    LOG_FILE="$LOG_DIR/${TEST_NAME// /_}.log"
    echo "Running test: $TEST_NAME"
    echo "  Executing: $COMMAND"
    ACTUAL_OUTPUT=$("$COMMAND")
    EXIT_CODE=$?
    if [ "$EXIT_CODE" -eq "$EXPECTED_EXIT_CODE" ] && [[ "$ACTUAL_OUTPUT" =~ "$EXPECTED_OUTPUT_REGEX" ]]; then
        echo "  Result: PASSED" >> "$LOG_FILE"
    else
        echo "  Result: FAILED" >> "$LOG_FILE"
        echo "    Exit Code: $EXIT_CODE (Expected: $EXPECTED_EXIT_CODE)" >> "$LOG_FILE"
        echo "    Expected Output (Regex): $EXPECTED_OUTPUT_REGEX" >> "$LOG_FILE"
        echo "    Actual Output:" >> "$LOG_FILE"
        echo "    --------------------" >> "$LOG_FILE"
        echo "$ACTUAL_OUTPUT" >> "$LOG_FILE"
        echo "    --------------------" >> "$LOG_FILE"
    fi
    echo "  Log file: $LOG_FILE"
    echo ""
}

echo "--- Command-Line Argument Tests ---"

mkdir -p "$LOG_DIR"

run_test_help "2.1 - Help Option (-h)" "$EXECUTABLE -h"

run_test_help "2.2 - Help Option (--help)" "$EXECUTABLE --help"

run_test_incorrect_usage "2.3 - No Arguments" "$EXECUTABLE" "for use this program you need select option -h \| --help or -f \|--file" "0"

run_test_incorrect_usage "2.4 - Missing Configuration File Path (-f)" "$EXECUTABLE -f" "for use this program you need select option -h \| --help or -f \|--file" "0"

run_test_incorrect_usage "2.5 - Missing Configuration File Path (--file)" "$EXECUTABLE --file" "for use this program you need select option -h \| --help or -f \|--file" "0"