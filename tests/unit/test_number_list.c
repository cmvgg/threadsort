#include "unit_test.h"

void test_init_number_list(void) {
    NumberList list;
    init_number_list(&list);
    TEST_ASSERT_NOT_NULL(list.data);
    TEST_ASSERT_EQUAL_UINT(INITIAL_CAPACITY, list.size);
    TEST_ASSERT_EQUAL_UINT(0, list.count);
    free_number_list(&list);
}

void test_add_number_to_list_empty_list(void) {
    NumberList list;
    init_number_list(&list);
    int result = add_number_to_list(&list, 10);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_UINT(1, list.count);
    TEST_ASSERT_EQUAL_INT(10, list.data[0]);
    free_number_list(&list);
}

void test_add_number_to_list_multiple_numbers(void) {
    NumberList list;
    init_number_list(&list);
    add_number_to_list(&list, 10);
    add_number_to_list(&list, 20);
    add_number_to_list(&list, 30);
    TEST_ASSERT_EQUAL_UINT(3, list.count);
    TEST_ASSERT_EQUAL_INT(10, list.data[0]);
    TEST_ASSERT_EQUAL_INT(20, list.data[1]);
    TEST_ASSERT_EQUAL_INT(30, list.data[2]);
    free_number_list(&list);
}

void test_add_number_to_list_beyond_initial_capacity(void) {
    NumberList list;
    init_number_list(&list);
    for (int i = 0; i < 10; i++) {
        TEST_ASSERT_EQUAL_INT(0, add_number_to_list(&list, i));
    }
    TEST_ASSERT_EQUAL_UINT(10, list.count);
    for (int i = 0; i < 10; i++) {
        TEST_ASSERT_EQUAL_INT(i, list.data[i]);
    }
    free_number_list(&list);
}

void test_add_number_to_list_zero_and_negative(void) {
    NumberList list;
    init_number_list(&list);
    TEST_ASSERT_EQUAL_INT(0, add_number_to_list(&list, 0));
    TEST_ASSERT_EQUAL_INT(0, add_number_to_list(&list, -5));
    TEST_ASSERT_EQUAL_UINT(2, list.count);
    TEST_ASSERT_EQUAL_INT(0, list.data[0]);
    TEST_ASSERT_EQUAL_INT(-5, list.data[1]);
    free_number_list(&list);
}

void test_print_number_list_empty(void) {
    NumberList list;
    init_number_list(&list);
    print_number_list("empty_list", &list);
    free_number_list(&list);
}

void test_print_number_list_single_element(void) {
    NumberList list;
    init_number_list(&list);
    add_number_to_list(&list, 7);
    print_number_list("single_element_list", &list);
    free_number_list(&list);
}

void test_print_number_list_multiple_elements(void) {
    NumberList list;
    init_number_list(&list);
    add_number_to_list(&list, 1);
    add_number_to_list(&list, 2);
    add_number_to_list(&list, 3);
    print_number_list("multiple_elements_list", &list);
    free_number_list(&list);
}

void test_free_number_list_empty(void) {
    NumberList list;
    init_number_list(&list);
    free_number_list(&list);
}

void test_free_number_list_with_elements(void) {
    NumberList list;
    init_number_list(&list);
    add_number_to_list(&list, 5);
    free_number_list(&list);
}

void test_add_number_to_list_null_list(void) {
    NumberList *list = NULL;
    int result = add_number_to_list(list, 10);
    TEST_ASSERT_EQUAL_INT(-1, result);
}


void test_add_number_to_list_max_and_min_values(void) {
    NumberList list;
    init_number_list(&list);
    int result1 = add_number_to_list(&list, INT_MAX);
    TEST_ASSERT_EQUAL_INT(0, result1);
    TEST_ASSERT_EQUAL_INT(INT_MAX, list.data[0]);
    int result2 = add_number_to_list(&list, INT_MIN);
    TEST_ASSERT_EQUAL_INT(0, result2);
    TEST_ASSERT_EQUAL_INT(INT_MIN, list.data[1]);
    free_number_list(&list);
}

void test_add_number_to_list_mutex_timeout(void) {
    NumberList list;

	init_number_list(&list);
	pthread_rwlock_wrlock(&list.rwlock);
    int result = add_number_to_list(&list, 42);
    TEST_ASSERT_EQUAL_INT(-2, result);
    TEST_ASSERT_EQUAL_UINT(0, list.count);
	pthread_rwlock_unlock(&list.rwlock);
    free_number_list(&list);
}

void run_number_list_tests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_init_number_list);
    RUN_TEST(test_add_number_to_list_empty_list);
    RUN_TEST(test_add_number_to_list_multiple_numbers);
    RUN_TEST(test_add_number_to_list_beyond_initial_capacity);
    RUN_TEST(test_add_number_to_list_zero_and_negative);
    RUN_TEST(test_add_number_to_list_max_and_min_values);
    RUN_TEST(test_add_number_to_list_null_list);
    RUN_TEST(test_print_number_list_empty);
    RUN_TEST(test_print_number_list_single_element);
    RUN_TEST(test_print_number_list_multiple_elements);
    RUN_TEST(test_free_number_list_empty);
    RUN_TEST(test_free_number_list_with_elements);
    UNITY_END();
}