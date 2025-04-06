#!/bin/bash

EXECUTABLE="../../../build/tests"
LOG_DIR="../../../logs_sh"

run_test_success() {
    TEST_NAME="$1"
    COMMAND="$2"
    LOG_FILE="$LOG_DIR/${TEST_NAME// /_}.log"
    echo "Running test: $TEST_NAME"
    echo "  Executing: $COMMAND"
    ACTUAL_OUTPUT=$("$COMMAND" 2>&1)
    EXIT_CODE=$?
    if [ "$EXIT_CODE" -eq 0 ] && [[ "$ACTUAL_OUTPUT" =~ "even list numbers.: \[" ]] && [[ "$ACTUAL_OUTPUT" =~ "odd list numbers.: \[" ]]; then
        echo "  Result: PASSED" >> "$LOG_FILE"
    else
        echo "  Result: FAILED" >> "$LOG_FILE"
        echo "    Exit Code: $EXIT_CODE (Expected: 0)" >> "$LOG_FILE"
        echo "    Expected Output (Regex): 'even list numbers.: \\[' and 'odd list numbers.: \\['" >> "$LOG_FILE"
        echo "    Actual Output:" >> "$LOG_FILE"
        echo "    --------------------" >> "$LOG_FILE"
        echo "$ACTUAL_OUTPUT" >> "$LOG_FILE"
        echo "    --------------------" >> "$LOG_FILE"
    fi
    echo "  Log file: $LOG_FILE"
    echo ""
}

run_test_failure_stderr() {
    TEST_NAME="$1"
    COMMAND="$2"
    EXPECTED_ERROR_REGEX="$3"
    EXPECTED_EXIT_CODE="$4"
    LOG_FILE="$LOG_DIR/${TEST_NAME// /_}.log"
    echo "Running test: $TEST_NAME"
    echo "  Executing: $COMMAND"
    ACTUAL_OUTPUT=$("$COMMAND" 2>&1)
    EXIT_CODE=$?
    ERROR_OUTPUT=$(echo "$ACTUAL_OUTPUT" | grep -E "$EXPECTED_ERROR_REGEX")
    if [ "$EXIT_CODE" -eq "$EXPECTED_EXIT_CODE" ] && [ -n "$ERROR_OUTPUT" ]; then
        echo "  Result: PASSED" >> "$LOG_FILE"
    else
        echo "  Result: FAILED" >> "$LOG_FILE"
        echo "    Exit Code: $EXIT_CODE (Expected: $EXPECTED_EXIT_CODE)" >> "$LOG_FILE"
        echo "    Expected Error (stderr Regex): $EXPECTED_ERROR_REGEX" >> "$LOG_FILE"
        echo "    Actual Output:" >> "$LOG_FILE"
        echo "    --------------------" >> "$LOG_FILE"
        echo "$ACTUAL_OUTPUT" >> "$LOG_FILE"
        echo "    --------------------" >> "$LOG_FILE"
    fi
    echo "  Log file: $LOG_FILE"
    echo ""
}

echo "--- Configuration File Tests ---"

mkdir -p "$LOG_DIR"

echo "numbers_per_thread=10" > valid_config.txt
echo "thread_num=2" >> valid_config.txt
run_test_success "1.1 - Valid Configuration File (small)" "$EXECUTABLE -f valid_config.txt"
rm valid_config.txt

echo "numbers_per_thread=100" > valid_config_large.txt
echo "thread_num=4" >> valid_config_large.txt
run_test_success "1.2 - Valid Configuration File (large)" "$EXECUTABLE -f valid_config_large.txt"
rm valid_config_large.txt

echo "numbers_per_thread=5" > valid_config_single_thread.txt
echo "thread_num=1" >> valid_config_single_thread.txt
run_test_success "1.3 - Valid Configuration File (single thread)" "$EXECUTABLE -f valid_config_single_thread.txt"
rm valid_config_single_thread.txt

echo "numbers_per_thread=10" > config_with_comments.txt
echo "# This is a comment" >> config_with_comments.txt
echo "thread_num=2 # Another comment" >> config_with_comments.txt
run_test_success "1.4 - Configuration File with Comments" "$EXECUTABLE -f config_with_comments.txt"
rm config_with_comments.txt

echo "" > empty_config.txt
run_test_failure_stderr "1.5 - Empty Configuration File" "$EXECUTABLE -f empty_config.txt" "Missing required keys." "1"
rm empty_config.txt

echo "numbers_per_thread= 10  " > config_with_whitespace.txt
echo "thread_num =  2 " >> config_with_whitespace.txt
run_test_success "1.6 - Configuration File with Whitespace" "$EXECUTABLE -f config_with_whitespace.txt"
rm config_with_whitespace.txt

run_test_failure_stderr "1.7 - Nonexistent Configuration File" "$EXECUTABLE -f nonexistent_config.txt" "Error opening config file" "1"

echo "numbers_per_thread 10" > invalid_syntax_equal_config.txt
echo "thread_num=2" >> invalid_syntax_equal_config.txt
run_test_failure_stderr "1.8 - Invalid Syntax Configuration File (missing equals)" "$EXECUTABLE -f invalid_syntax_equal_config.txt" "Incorrect line format." "1"
rm invalid_syntax_equal_config.txt

echo "numbers_per_thread=10" > invalid_syntax_unknown_key_config.txt
echo "thread_num=2" >> invalid_syntax_unknown_key_config.txt
echo "otro_parametro=valor" >> invalid_syntax_unknown_key_config.txt
run_test_success "1.9 - Invalid Syntax Configuration File (unknown key - should be ignored)" "$EXECUTABLE -f invalid_syntax_unknown_key_config.txt"
rm invalid_syntax_unknown_key_config.txt

echo "numbers_per_thread=-5" > invalid_value_numbers_config.txt
echo "thread_num=2" >> invalid_value_numbers_config.txt
run_test_failure_stderr "1.10 - Invalid Value Configuration File (negative numbers)" "$EXECUTABLE -f invalid_value_numbers_config.txt" "numbers_per_thread must be greater than zero." "1"
rm invalid_value_numbers_config.txt

echo "numbers_per_thread=10" > invalid_value_threads_config.txt
echo "thread_num=0" >> invalid_value_threads_config.txt
run_test_failure_stderr "1.11 - Invalid Value Configuration File (zero threads)" "$EXECUTABLE -f invalid_value_threads_config.txt" "thread_num must be greater than zero." "1"
rm invalid_value_threads_config.txt

echo "numbers_per_thread=10" > missing_key_config.txt
run_test_failure_stderr "1.12 - Missing Required Key (thread_num)" "$EXECUTABLE -f missing_key_config.txt" "Missing required keys." "1"
rm missing_key_config.txt