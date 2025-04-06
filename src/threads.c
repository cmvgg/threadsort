#include "../include/clasif.h"

extern volatile sig_atomic_t termination_flag;

static int add_number_to_list_internal(NumberList *list, int number) {
    if (list->count >= list->size) {
        size_t new_size = list->size == 0 ? INITIAL_CAPACITY : list->size * 2;
        int *new_data = realloc(list->data, new_size * sizeof(int));
        if (new_data == NULL) return -1;
        list->data = new_data;
        list->size = new_size;
    }
    list->data[list->count++] = number;
    return 0;
}

void *thread_function(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    NumberList *main_list = args->main_list;
    NumberList *even_list = args->even_list;
    NumberList *odd_list = args->odd_list;
    int thread_id = args->thread_id;
    int numbers_per_thread = args->numbers_per_thread;

    size_t start_index = thread_id * numbers_per_thread;
    size_t end_index = start_index + numbers_per_thread;


    pthread_rwlock_rdlock(&main_list->rwlock);

    for (size_t i = start_index; i < end_index && i < main_list->count; ++i) {
        int number = main_list->data[i];
        if (number % 2 == 0) {
        
            pthread_rwlock_wrlock(&even_list->rwlock);
            add_number_to_list_internal(even_list, number);
            pthread_rwlock_unlock(&even_list->rwlock);
        } else {
            pthread_rwlock_wrlock(&odd_list->rwlock);
            add_number_to_list_internal(odd_list, number);
            pthread_rwlock_unlock(&odd_list->rwlock);
        }
    }

    pthread_rwlock_unlock(&main_list->rwlock);

    pthread_exit(NULL);
    return NULL;
}