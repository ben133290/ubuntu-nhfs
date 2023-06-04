#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "setup.h"

#define TAGS_FILE "data/tags.txt"
//#include "file.h"
//#include "tag.h"

void print_usage_and_exit(void) {
    printf("Usage: nhfs <command> [options]\n");
    printf("\n");
    printf("Commands:\n");
    printf("  tag    Add or remove tags from files\n");
    printf("  search Search for files by tag\n");
    printf("  list   List all tags\n");
    printf("  clear  Clear the tag file, deleting all tags\n");
    printf("\n");
    printf("Options:\n");
    printf("  -h, --help       Display this help message\n");
    printf("\n");
    exit(1);
}

void print_tag_usage(void) {
    printf("Usage: nhfs tag [operation]\n");
    printf("\n");
    printf("Operations:\n");
    printf("  add    Add tags to files\n");
    printf("  remove Remove tags from files\n");
    printf("\n");
}

int handle_add_tag_command(int argc, char *argv[]) {
    char *filename = argv[1];
    char *tag = argv[2];

    // Open the tags file for reading and writing
    FILE *tags_file = fopen(TAGS_FILE, "r+");
    if (tags_file == NULL) {
        printf("Error: could not open tags file\n");
        return -1;
    }

    // Search for the tag in the tags file
    char line[1024];
    int tag_found = 0;
    while (fgets(line, 1024, tags_file)) {
        char *line_tag = strtok(line, " ");
        if (strcmp(line_tag, tag) == 0) {
            // Tag found, append filename to the line and write back to file
            tag_found = 1;
            printf("Info: Tag already exists\n");

            // TODO: add file name to the end of the current line in the file
            // Move to character right before the new line character on that line
            fseek(tags_file, -1, SEEK_CUR);
            long insert_position = ftell(tags_file);

            // Create buffer to store text to be moved and move cursor back
            char *buffer;
            fseek(tags_file, 0, SEEK_END);
            long end_position = ftell(tags_file);
            fseek(tags_file, insert_position, SEEK_SET);
            long num_bytes = end_position - insert_position;
            buffer = (char *)calloc(num_bytes, sizeof(char));

            // Read rest of file into buffer
            fread(buffer, sizeof(char), num_bytes, tags_file);
            fseek(tags_file, insert_position, SEEK_SET);

            fprintf(tags_file, " %s%s", filename, buffer);
            printf("Debug: File cursor postition %ld\n", ftell(tags_file));
            break;
        }
    }

    // If the tag doesn't exist in the tags file, write a new line to the end of the file
    if (!tag_found) {
        fseek(tags_file, 0, SEEK_END);
        fprintf(tags_file, "%s %s\n", tag, filename);
    }

    // Close the tags file
    fclose(tags_file);

    return 0;
}

int handle_remove_tag_command(int argc, char *argv[]) {

    //TODO: implment remove tag
    printf("Not yet implemented\n");
    return -1;
}

int handle_tag_command(int argc, char *argv[]) {

    // handle incorrect usage
    if (argc <= 1 || argc > 4) {
        print_tag_usage();
        return 0;
    }

    char *command_option = argv[1];

    if (strcmp(command_option, "add") == 0) {
        if (handle_add_tag_command(argc - 1, &argv[1]) == -1) {
            printf("Error: could not add tag\n");
            return -1;
            
        } else {
            printf("Tag added successfully\n");
            return 0;
        }
    }

    if (strcmp(command_option, "remove") == 0) {
        if (handle_remove_tag_command(argc - 1, &argv[1]) == -1) {
            printf("Error: could not remove tag\n");
            return -1;
        } else {
            printf("Tag added successfully\n");
            return 0;
        }
    }
}

void handle_search_command(int argc, char *argv[]) {
    // TODO: handle search command
    printf("Not yet implemented\n");
}

void handle_list_command(void) {



    FILE *tags_file = fopen(TAGS_FILE, "r");
    if (tags_file == NULL) {
        printf("Error: could not open tags file\n");
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), tags_file)) {
        printf("%s", line);
    }

    fclose(tags_file);
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
