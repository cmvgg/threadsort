#include "../include/clasif.h"

//extern volatile sig_atomic_t termination_flag;

char *trim(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0)
        return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

int read_config_file(const char *file_path, Config *config) {
    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
        perror("Error opening config file");
        return -1;
    }


    char line[256];
    int numbers_per_thread_found = 0;
    int thread_num_found = 0;

    while (fgets(line, sizeof(line), fp) != NULL ) {
        char *trimmed_line = trim(line);
        if (trimmed_line[0] == '#' || trimmed_line[0] == '\0') {
            continue;
        }

        char *key = strtok(trimmed_line, "=");
        char *value_str = strtok(NULL, "=");

        if (key == NULL || value_str == NULL) {
            fflush(stderr);
            fprintf(stderr, "Incorrect line format.\n");
            if (fclose(fp) != 0)
                perror("Error closing config file");
            return -2;
        }

        key = trim(key);
        value_str = trim(value_str);

        if (strcmp(key, "numbers_per_thread") == 0) {
            config->numbers_per_thread = atoi(value_str);
            numbers_per_thread_found = 1;
        } else if (strcmp(key, "thread_num") == 0) {
            config->thread_num = atoi(value_str);
            thread_num_found = 1;
        } else {
            fflush(stderr);
            fprintf(stderr, "Unknown key '%s'.\n", key);
        }
    }

    if (!numbers_per_thread_found || !thread_num_found) {
        fflush(stderr);
        fprintf(stderr, "Missing required keys.\n");
        if (fclose(fp) != 0)
            perror("Error closing config file");
        return -3;
    }

    if (fclose(fp) != 0) {
        perror("Error closing config file");
        return -4;
    }

    return 0;
}
