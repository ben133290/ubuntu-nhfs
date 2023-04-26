#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define TAGS_FILE "data/tags.txt"
//#include "file.h"
//#include "tag.h"

void print_usage_and_exit(void) {
    printf("Usage: tfs <command> [options]\n");
    printf("\n");
    printf("Commands:\n");
    printf("  tag    Add or remove tags from files\n");
    printf("  search Search for files by tag\n");
    printf("  list   List all tags\n");
    printf("\n");
    printf("Options:\n");
    printf("  -h, --help       Display this help message\n");
    printf("\n");
    exit(1);
}

void handle_tag_command(int argc, char *argv[]) {

    if (argc != 4) {
        printf("Usage: tfs tag add [file] [tag]\n");
        return;
    }

    char *filename = argv[2];
    char *tag = argv[3];

    // Open the tags file for appending or create it if it doesn't exist
    FILE *tags_file = fopen(TAGS_FILE, "a");
    if (tags_file == NULL) {
        printf("Error: could not open tags file\n");
        return;
    }

    // Write the tag and filename to the tags file
    fprintf(tags_file, "%s %s\n", tag, filename);

    // Close the tags file
    fclose(tags_file);

    printf("Tag added successfully\n");
}

void handle_search_command(int argc, char *argv[]) {
    // TODO: handle search command
}

void handle_list_command(void) {
    // TODO: handle search command
}

void handle_clear_command(void) {
    // Open the tags file for writing or create it if it doesn't exist
    FILE *tags_file = fopen(TAGS_FILE, "w");
    if (tags_file == NULL) {
        printf("Error: could not open tags file\n");
        return;
    }

    // Close the tags file
    fclose(tags_file);

    printf("Tags cleared successfully\n");
}

int main(int argc, char *argv[]) {

    // Create the data directory if it doesn't exist
    struct stat st = {0};
    if (stat("data", &st) == -1) {
        mkdir("data", 0777);
    }

    // Create the tags file if it doesn't exist
    FILE *tags_file = fopen(TAGS_FILE, "a");
    if (tags_file == NULL) {
        printf("Error: could not create tags file\n");
        return 1;
    }
    fclose(tags_file);

    // Parse command-line arguments
    if (argc < 2) {
        print_usage_and_exit();
    }

    char *command = argv[1];

    if (strcmp(command, "tag") == 0) {
        handle_tag_command(argc - 1, &argv[1]);
    } else if (strcmp(command, "search") == 0) {
        handle_search_command(argc - 1, &argv[1]);
    } else if (strcmp(command, "clear") == 0) {
        handle_clear_command();
    } else if (strcmp(command, "list") == 0) {
        handle_list_command();
    } else if (strcmp(command, "-h") == 0 || strcmp(command, "--help") == 0) {
        print_usage_and_exit();
    } else {
        fprintf(stderr, "Error: Unknown command '%s'\n", command);
        exit(1);
    }

    return 0;
}
