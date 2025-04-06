#ifndef CLASIF_H
#define CLASIF_H

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>

#define MUTEX_TIMEOUT_MS 1000
#define INITIAL_CAPACITY 10

extern volatile sig_atomic_t termination_flag;

typedef struct {
    int *data;
    size_t size;
    size_t count;
    pthread_rwlock_t rwlock;
} NumberList;

typedef struct {
    int numbers_per_thread;
    int thread_num;
} Config;

typedef struct {
    NumberList *main_list;
    NumberList *even_list;
    NumberList *odd_list;
    int thread_id;
    int numbers_per_thread;
} ThreadArgs;


/* CONFIG.C*/
int read_config_file(const char *file_path, Config *config);
int validate_config(const Config *config);

/*CLASIF.C*/
void free_main_number_list(void);
void initialize_main_list(const Config *config);
void init_number_list(NumberList *list);
int add_number_to_list(NumberList *list, int number);
void print_number_list(char *list_name, NumberList *list);
void free_number_list(NumberList *list);
int start_threads(int num_threads, int numbers_per_thread, NumberList *even_list, NumberList *odd_list);

/*THREADS.C*/

void *thread_function(void *arg);


// void cleanup(void);
// void display_help(void);
// void signal_handler(int signal);

#endif