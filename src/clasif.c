#include "../include/clasif.h"

//extern volatile sig_atomic_t termination_flag;

NumberList main_number_list;

void free_main_number_list() {
    free_number_list(&main_number_list);
}

void display_help() {
    printf("Use: ./threadsort -f | --file <path_to_config file>\n");
}

void initialize_main_list(const Config *config) {
    if (config == NULL || config->numbers_per_thread <= 0 || config->thread_num <= 0) {
        fprintf(stderr, "Error: invalid config.\n");
        return;
    }

    int total_numbers = config->numbers_per_thread * config->thread_num;
    init_number_list(&main_number_list);
    srand(time(NULL));
    int  final= total_numbers;

 
    for (int i = 0; i < total_numbers; ++i) {
        fflush(stdout);
        if(final > 0)
            fprintf(stderr, "numbers left: %d\r", final--);
        else
            fprintf(stderr, "/r");
        
        int random_number;
        bool is_duplicate;
        int attempts = 0;
        do {
            if (attempts++ > 10000000) { 
                fprintf(stderr, "Advice: increase the number of threads.\n");
                break;
            }

            random_number = (rand() % 10000000) + 1;

            is_duplicate = false;
            for (size_t j = 0; j < main_number_list.count; ++j) {
                if (main_number_list.data[j] == random_number) {
                    is_duplicate = true;
                    break;
                }
            }
        } while (is_duplicate);

        if (!is_duplicate) {
            add_number_to_list(&main_number_list, random_number);
        }
    }
}

void init_number_list(NumberList *list) {
    if (list != NULL) {
        list->data = malloc(INITIAL_CAPACITY * sizeof(int));
        if (list->data == NULL) {
            perror("Error allocating initial memory for list");
            list->size = 0;
            list->count = 0;
            if (pthread_rwlock_init(&list->rwlock, NULL) != 0) {
                perror("Error initializing read-write lock");
            }
            return;
        }
        list->size = INITIAL_CAPACITY;
        list->count = 0;
        if (pthread_rwlock_init(&list->rwlock, NULL) != 0) {
            perror("Error initializing read-write lock");
            free(list->data);
            list->data = NULL;
            list->size = 0;
            list->count = 0;
        }
    }
}

int add_number_to_list(NumberList *list, int number) {
    if (list == NULL) {
        return -1;
    }

    struct timespec timeout;
    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += MUTEX_TIMEOUT_MS / 1000;
    timeout.tv_nsec += (MUTEX_TIMEOUT_MS % 1000) * 1000000;

    int result = pthread_rwlock_timedwrlock(&list->rwlock, &timeout);
    if (result != 0) {
        if (result == ETIMEDOUT) {
            perror("Timeout acquiring write lock");
            return -2;
        } else {
            perror("Error acquiring write lock");
            return -3;
        }
    }

    if (list->count >= list->size) {
        size_t new_size = list->size == 0 ? INITIAL_CAPACITY : list->size * 2;
        int *new_data = realloc(list->data, new_size * sizeof(int));
        if (new_data == NULL) {
            perror("Error reallocating memory");
            pthread_rwlock_unlock(&list->rwlock);
            return -4;
        }
        list->data = new_data;
        list->size = new_size;
    }

    list->data[list->count++] = number;

    pthread_rwlock_unlock(&list->rwlock);
    return 0;
}


void print_number_list(char *list_name, NumberList *list) {
    if (list != NULL) {
        struct timespec timeout;
        clock_gettime(CLOCK_REALTIME, &timeout);
        timeout.tv_sec += MUTEX_TIMEOUT_MS / 1000;
        timeout.tv_nsec += (MUTEX_TIMEOUT_MS % 1000) * 1000000;

        int result = pthread_rwlock_timedrdlock(&list->rwlock, &timeout);
        if (result != 0) {
            if (result == ETIMEDOUT) {
                perror("Timeout acquiring read lock for printing");
                return;
            } else {
                perror("Error acquiring read lock for printing");
                return;
            }
        }

        printf("%s: [\n", list_name);
        for (size_t i = 0; i < list->count; i++) {
            printf("Position: %ld: number %d\n", i + 1, list->data[i]);
        }
        printf("]\n");

        pthread_rwlock_unlock(&list->rwlock);
    } else {
        printf("%s: (null)\n", list_name);
    }
}

void free_number_list(NumberList *list) {
    if (list != NULL) {
        if (list->data != NULL) {
            free(list->data);
            list->data = NULL;
            list->size = 0;
            list->count = 0;
        }
        pthread_rwlock_destroy(&list->rwlock);
    }
}

int start_threads(int num_threads, int numbers_per_thread, NumberList *even_list, NumberList *odd_list) {
    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    if (threads == NULL) {
        perror("Error to allocate memory for threads.\n");
        return -1;
    }

    extern NumberList main_number_list;

    ThreadArgs *thread_args = malloc(num_threads * sizeof(ThreadArgs));
    if (thread_args == NULL) {
        perror("Error to allocate memory for thread arguments.\n");
        free(threads);
        return -1;
    }

    for (int i = 0; i < num_threads; i++) {
        thread_args[i].main_list = &main_number_list;
        thread_args[i].even_list = even_list;
        thread_args[i].odd_list = odd_list;
        thread_args[i].thread_id = i;
        thread_args[i].numbers_per_thread = numbers_per_thread;

        if (pthread_create(&threads[i], NULL, thread_function, &thread_args[i]) != 0) {
            perror("Error create thread.\n");
            free(threads);
            free(thread_args);
            return -1;
        }
    }

    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Error join thread.\n");
        }
    }

    free(threads);
    free(thread_args);
    return 0;
}