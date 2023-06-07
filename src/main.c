#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "../include/setup.h"
#include "../include/graph.h"
#include "../include/util.h"

#define TAGS_FILE "data/tags.txt"
//#include "file.h"
//#include "tag.h"

Graph* graph;

void print_usage_and_exit(void) {
    printf("Usage: nhfs <command> [options]\n");
    printf("\n");
    printf("Commands:\n");
    printf("  add    Add tag or file\n");
    printf("  remove Remove a tag or file\n");
    printf("  search Search for files by tag\n");
    printf("  list   List all tags\n");
    printf("  clear  Clear the tag file, deleting all tags\n");
    printf("\n");
    printf("Options:\n");
    printf("  -h, --help       Display this help message\n");
    printf("\n");
    exit(1);
}

int add(int argc, char *argv[]) {
    char *name = argv[1];
    char *type = argv[2];
    char header_path[256];  // Allocate memory for header_path
    char file_create_path[256];  // Allocate memory for file_create_path

    printf("Checking if file exists\n");
    // check if file already exists
    for (int i = 1; i <= graph->nodeCount; i++) {
        // Open corr. header file and read name
        // return 1 if exists ie == name
        printf("Checking node: %d\n", i);
        if (sprintf(header_path, "data/%d.txt", i) < 0) {  // Check if sprintf was successful
            printf("Error: couldn't create header path\n");
            return -1;  // Return an error code
        }

        char *read_name = readNthLine(header_path, 1);

        if (read_name != NULL && strcmp(read_name, name) == 0) {
            printf("Name already used!\n");
            return -1;
        }
    }

    // Create header file
    if (sprintf(file_create_path, "data/%d.txt", graph->nodeCount + 1) < 0) {
        printf("Error: Couldn't create header file\n");
        return -1;
    }

    // add Node to graph
    addNode(graph, graph->nodeCount+1, type);

    FILE *head_file = fopen(file_create_path, "w");
    if (head_file == NULL) {
        printf("Error: could not create header at %s\n", file_create_path);
        return -1;
    }
    fprintf(head_file, "%s\n", name);
    fclose(head_file);

    saveGraph(graph, "data/graph.txt");

    return 0;
}

int rmv(int argc, char *argv[]) {
    char * name = argv[1];
    char header_path[256];
    
    printf("Searching for node with name %s\n", name);
    // Check if file with name exists
    for (int i = 1; i <= graph->nodeCount; i++) {
        if (sprintf(header_path, "data/%d.txt", i) < 0) {  // Check if sprintf was successful
            printf("Error: couldn't create header path\n");
            return -1;  // Return an error code
        }

        char *read_name = readNthLine(header_path, 1);

        if (read_name != NULL && strcmp(read_name, name) == 0) {
            // Delete the file
            remove(header_path);
            removeNode(graph, i);
            saveGraph(graph, "data/graph.txt");
            printf("Removed header file at %s\n", header_path);
        }
    }

    // Update graph

    return 0;
}

void handle_search_command(int argc, char *argv[]) {
    // TODO: handle search command
    printf("Not yet implemented\n");
}

void list(void) {
    char header_path[256];

    printf("Tags:\n");

    for (int i = 1; i <= graph->nodeCount; i++) {
        if (sprintf(header_path, "data/%d.txt", i) < 0) {  // Check if sprintf was successful
            printf("Error: couldn't create header path\n");
            return;  // Return an error code
        }

        char *read_name = readNthLine(header_path, 1);
        char *read_type = readNthLine(header_path, 2);

        if (read_type != NULL && strcmp(read_type, "tag") == 0) {
            printf("   %s\n", read_name);
        }
    }

    return;
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

    graph = loadGraph("data/graph.txt");
    if (graph == NULL) {
        graph = createGraph();
    }
    printGraph(graph);

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

    if (strcmp(command, "add") == 0) {
        add(argc - 1, &argv[1]);
    } else if (strcmp(command, "remove") == 0) {
        rmv(argc - 1, &argv[1]);
    } else if (strcmp(command, "clear") == 0) {
        handle_clear_command();
    } else if (strcmp(command, "list") == 0) {
        list();
    } else if (strcmp(command, "-h") == 0 || strcmp(command, "--help") == 0) {
        print_usage_and_exit();
    } else {
        fprintf(stderr, "Error: Unknown command '%s'\n", command);
        exit(1);
    }

    return 0;
}
