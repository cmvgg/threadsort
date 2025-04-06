#include "unit_test.h"

int main(void) {
    UNITY_BEGIN();

    run_config_tests();
    run_validate_config_tests();

    UNITY_END();
    return 0;
}