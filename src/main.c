#include "../include/clasif.h"

int main(int argc, char **argv) {
	if(argc == 2 || argc == 3)
	{
		if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
			display_help();
			return 0;
		}

		if (!strcmp(argv[1], "-f") && !strcmp(argv[1], "--file")) {
			printf("for use this program you need select option -h | --help or -f |--file\n");
			return 0;
		}

		const char *config_file_path = argv[2];
		Config config;

		if (read_config_file(config_file_path, &config) != 0) {
			fprintf(stderr, "Error to read config file: %s\n", config_file_path);
			return 1;
		}

		if (validate_config(&config) != 0) {
			fprintf(stderr, "Error: the configuration is invalid.\n");
			return 1;
		}

		if (signal(SIGINT, signal_handler) == SIG_ERR) {
			perror("Error to register the SIGINT handler");
			return 1;
		}

		if (signal(SIGTERM, signal_handler) == SIG_ERR) {
			perror("Error to register the SIGTERM handler");
			return 1;
		} 

		initialize_main_list(&config);

		NumberList even_list;
		init_number_list(&even_list);
		NumberList odd_list;
		init_number_list(&odd_list);

		if (start_threads(config.thread_num, config.numbers_per_thread, &even_list, &odd_list) != 0) {
			fprintf(stderr, "Error to start threads.\n");
			free_number_list(&even_list);
			free_number_list(&odd_list);
			free_main_number_list();
			return 1;
		}

		print_number_list("even list numbers.", &even_list);
		print_number_list("odd list numbers.", &odd_list);

		free_number_list(&even_list);
		free_number_list(&odd_list);
		free_main_number_list();

		if (termination_flag) {
			cleanup();
			fprintf(stderr, "Program terminated by user signal.\n");
			exit(0);
		}  else {
			cleanup();
		}
	}else
		printf("for use this program you need select option -h | --help or -f |--file\n");

	return 0;
}