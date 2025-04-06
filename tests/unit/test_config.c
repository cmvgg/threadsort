#include "unit_test.h"

const char *temp_config_file = "temp_config.txt";

void setUp(void) {
}

void tearDown(void) {
    remove(temp_config_file);
}

void test_read_config_valid_input(void) {
    FILE *fp = fopen(temp_config_file, "w");
    if (fp == NULL) {
        TEST_FAIL_MESSAGE("No se pudo crear el archivo");
        return;
    }
    fprintf(fp, "numbers_per_thread = 10\n");
    fprintf(fp, "thread_num = 4\n");
    fclose(fp);

    Config config;
    TEST_ASSERT_EQUAL_INT(0, read_config_file(temp_config_file, &config));
    TEST_ASSERT_EQUAL_INT(10, config.numbers_per_thread);
    TEST_ASSERT_EQUAL_INT(4, config.thread_num);
}

void test_read_config_invalid_keys(void) {
    FILE *fp = fopen(temp_config_file, "w");
    if (fp == NULL) {
        TEST_FAIL_MESSAGE("No se pudo crear el archivo");
        return;
    }
    fprintf(fp, "threads = 4\n");
    fprintf(fp, "numbers = 10\n");
    fclose(fp);

    Config config;
    TEST_ASSERT_NOT_EQUAL(0, read_config_file(temp_config_file, &config));
}

void test_read_config_non_integer_values(void) {
    FILE *fp = fopen(temp_config_file, "w");
    if (fp == NULL) {
        TEST_FAIL_MESSAGE("No se pudo crear el archivo");
        return;
    }
    fprintf(fp, "numbers_per_thread = abc\n");
    fprintf(fp, "thread_num = 4\n");
    fclose(fp);

    Config config;
    TEST_ASSERT_NOT_EQUAL(-2, read_config_file(temp_config_file, &config));
}

void test_read_config_invalid_format_missing_equals(void) {
    FILE *fp = fopen(temp_config_file, "w");
    if (fp == NULL) {
        TEST_FAIL_MESSAGE("No se pudo crear el archivo");
        return;
    }
    fprintf(fp, "numbers_per_thread 10\n");
    fprintf(fp, "thread_num = 4\n");
    fclose(fp);

    Config config;
    TEST_ASSERT_NOT_EQUAL(0, read_config_file(temp_config_file, &config));
}

void test_read_config_missing_keys(void) {
    FILE *fp = fopen(temp_config_file, "w");
    if (fp == NULL) {
        TEST_FAIL_MESSAGE("No se pudo crear el archivo");
        return;
    }
    fprintf(fp, "thread_num = 4\n");
    fclose(fp);

    Config config;
    TEST_ASSERT_NOT_EQUAL(0, read_config_file(temp_config_file, &config));
}

void test_read_config_empty_file(void) {
    FILE *fp = fopen(temp_config_file, "w");
    if (fp == NULL) {
        TEST_FAIL_MESSAGE("No se pudo crear el archivo");
        return;
    }
    fclose(fp);

    Config config;
    TEST_ASSERT_NOT_EQUAL(0, read_config_file(temp_config_file, &config));
}

void test_read_config_extra_content(void) {
    FILE *fp = fopen(temp_config_file, "w");
    if (fp == NULL) {
        TEST_FAIL_MESSAGE("No se pudo crear el archivo");
        return;
    }
    fprintf(fp, "numbers_per_thread = 10\n");
    fprintf(fp, "thread_num = 4\n");
    fprintf(fp, "extra_line = algo\n");
    fprintf(fp, "comentario\n");
    fclose(fp);

    Config config;
    TEST_ASSERT_EQUAL_INT(-2, read_config_file(temp_config_file, &config));
    TEST_ASSERT_EQUAL_INT(10, config.numbers_per_thread);
    TEST_ASSERT_EQUAL_INT(4, config.thread_num);
}

void test_read_config_with_comments(void) {
    FILE *fp = fopen(temp_config_file, "w");
    if (fp == NULL) {
        TEST_FAIL_MESSAGE("No se pudo crear el archivo");
        return;
    }
    fprintf(fp, "# comentario\n");
    fprintf(fp, "numbers_per_thread = 10\n");
    fprintf(fp, "thread_num = 4 # comentario2\n");
    fprintf(fp, "#comentario3\n");
    fclose(fp);

    Config config;
    TEST_ASSERT_EQUAL_INT(0, read_config_file(temp_config_file, &config));
    TEST_ASSERT_EQUAL_INT(10, config.numbers_per_thread);
    TEST_ASSERT_EQUAL_INT(4, config.thread_num);
}
void test_read_config_no_value(void) {
    FILE *fp = fopen(temp_config_file, "w");
    if (fp == NULL) {
        TEST_FAIL_MESSAGE("No se pudo crear el archivo");
        return;
    }
    fprintf(fp, "numbers_per_thread =\n");
    fprintf(fp, "thread_num = 4\n");
    fclose(fp);

    Config config;
    TEST_ASSERT_NOT_EQUAL(0, read_config_file(temp_config_file, &config));
}
void test_read_config_keys_out_of_order(void) {
    FILE *fp = fopen(temp_config_file, "w");
    if (fp == NULL) {
        TEST_FAIL_MESSAGE("No se pudo crear el archivo");
        return;
    }
    fprintf(fp, "thread_num = 4\n");
    fprintf(fp, "numbers_per_thread = 10\n");
    fclose(fp);

    Config config;
    TEST_ASSERT_EQUAL_INT(0, read_config_file(temp_config_file, &config));
    TEST_ASSERT_EQUAL_INT(10, config.numbers_per_thread);
    TEST_ASSERT_EQUAL_INT(4, config.thread_num);
}

void test_read_config_duplicate_keys(void) {
    FILE *fp = fopen(temp_config_file, "w");
    if (fp == NULL) {
        TEST_FAIL_MESSAGE("No se pudo crear el archivo");
        return;
    }
    fprintf(fp, "numbers_per_thread = 10\n");
    fprintf(fp, "numbers_per_thread = 20\n");
    fprintf(fp, "thread_num = 4\n");
    fclose(fp);

    Config config;
    TEST_ASSERT_EQUAL_INT(0, read_config_file(temp_config_file, &config));
    TEST_ASSERT_EQUAL_INT(20, config.numbers_per_thread);
    TEST_ASSERT_EQUAL_INT(4, config.thread_num);

    fp = fopen(temp_config_file, "w");
    if (fp == NULL) {
        TEST_FAIL_MESSAGE("No se pudo crear el archivo");
        return;
    }
    fprintf(fp, "numbers_per_thread = 10\n");
    fprintf(fp, "thread_num = 4\n");
    fprintf(fp, "thread_num = 8\n");
    fclose(fp);

    Config config2;
    TEST_ASSERT_EQUAL_INT(0, read_config_file(temp_config_file, &config2));
    TEST_ASSERT_EQUAL_INT(10, config2.numbers_per_thread);
    TEST_ASSERT_EQUAL_INT(8, config2.thread_num);
}
void run_config_tests(void) {
    UNITY_BEGIN();

    RUN_TEST(test_read_config_invalid_format_missing_equals);
	RUN_TEST(test_read_config_invalid_keys);
	RUN_TEST(test_read_config_non_integer_values);
    RUN_TEST(test_read_config_missing_keys);
	RUN_TEST(test_read_config_empty_file);
	RUN_TEST(test_read_config_extra_content);
    RUN_TEST(test_read_config_with_comments);
	RUN_TEST(test_read_config_no_value);
	RUN_TEST(test_read_config_keys_out_of_order);
	RUN_TEST(test_read_config_duplicate_keys);
    UNITY_END();
}