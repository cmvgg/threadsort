#include "../include/clasif.h"

volatile sig_atomic_t termination_flag = false;

NumberList even_list;
NumberList odd_list;

void cleanup(void) {
    free_number_list(&even_list);
    free_number_list(&odd_list);
    free_main_number_list();
}

void signal_handler(int signal) {
    if (signal == SIGINT) {
        termination_flag = true;
		cleanup();
		fprintf(stderr, "Program terminated by user signal.\n");
		exit(0);
    } else if (signal == SIGTERM) {
        termination_flag = true;
		cleanup();
		fprintf(stderr, "Program terminated by system signal.\n");
		exit(0);
    }
}