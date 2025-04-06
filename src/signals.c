#include "../include/clasif.h"

//volatile sig_atomic_t termination_flag = false;
NumberList even_list;
NumberList odd_list;

void cleanup(void) {
    free_number_list(&even_list);
    free_number_list(&odd_list);
    free_main_number_list();
}