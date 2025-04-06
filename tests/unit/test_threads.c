#include "unit_test.h"

void test_start_threads_creation(void) {
    NumberList even_list;
    init_number_list(&even_list);
    NumberList odd_list;
    init_number_list(&odd_list);
    int num_threads = 2;
    int numbers_per_thread = 5;

    int result = start_threads(num_threads, numbers_per_thread, &even_list, &odd_list);

    TEST_ASSERT_EQUAL_INT(0, result);

    free_number_list(&even_list);
    free_number_list(&odd_list);
}

void run_threads_tests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_start_threads_creation);
    UNITY_END();
}