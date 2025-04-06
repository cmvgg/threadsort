#include "unit_test.h"

void test_validate_config_valid(void) {
    Config config;
    config.numbers_per_thread = 10;
    config.thread_num = 4;
    TEST_ASSERT_EQUAL_INT(0, validate_config(&config));
}

void test_validate_config_numbers_per_thread_zero(void) {
    Config config;
    config.numbers_per_thread = 0;
    config.thread_num = 4;
    TEST_ASSERT_NOT_EQUAL(0, validate_config(&config));
}

void test_validate_config_thread_num_zero(void) {
    Config config;
    config.numbers_per_thread = 10;
    config.thread_num = 0;
    TEST_ASSERT_NOT_EQUAL(0, validate_config(&config));
}

void test_validate_config_both_zero(void) {
    Config config;
    config.numbers_per_thread = 0;
    config.thread_num = 0;
    TEST_ASSERT_NOT_EQUAL(0, validate_config(&config));
}

void test_validate_config_numbers_per_thread_negative(void) {
    Config config;
    config.numbers_per_thread = -5;
    config.thread_num = 4;
    TEST_ASSERT_NOT_EQUAL(0, validate_config(&config));
}

void test_validate_config_thread_num_negative(void) {
    Config config;
    config.numbers_per_thread = 10;
    config.thread_num = -2;
    TEST_ASSERT_NOT_EQUAL(0, validate_config(&config));
}

void test_validate_config_both_negative(void) {
    Config config;
    config.numbers_per_thread = -5;
    config.thread_num = -2;
    TEST_ASSERT_NOT_EQUAL(0, validate_config(&config));
}

void run_validate_config_tests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_validate_config_valid);
    RUN_TEST(test_validate_config_numbers_per_thread_zero);
    RUN_TEST(test_validate_config_thread_num_zero);
    RUN_TEST(test_validate_config_both_zero);
    RUN_TEST(test_validate_config_numbers_per_thread_negative);
    RUN_TEST(test_validate_config_thread_num_negative);
    RUN_TEST(test_validate_config_both_negative);
    UNITY_END();
}