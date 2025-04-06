#include "unit_test.h"

int main(void) {
    UNITY_BEGIN();

    run_config_tests();
    run_validate_config_tests();
    run_number_list_tests();
    run_threads_tests();

    UNITY_END();
    return 0;
}