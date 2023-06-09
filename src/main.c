#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "../include/setup.h"
#include "../include/graph.h"
#include "../include/util.h"
#include "../include/interactionSys.h"

#ifndef GRAPH_FILE
#define GRAPH_FILE "data/graph.txt"
#endif
#define GRAPH_MAX 1000
//#include "file.h"
//#include "tag.h"

Graph* graph;

void print_usage_and_exit(void) {
    printf("Usage: nhfs <command> [options]\n");
    printf("\n");
    printf("Commands:\n");
    printf("  add name type [filepath]   Add tag or file\n"); //mem leak and error free
    printf("  remove name                Remove a tag or file from file system\n"); //mem leak and error free
    printf("  list                       List all tags\n"); //mem leak and error free
    printf("  lost                       List all untagged files\n"); // mem leak and error free
    printf("  info name                  Print infomation on tag or file\n"); //mem leak and error free
    printf("  link tagname filename      Create a link between a file and a tag\n"); //leak and error free
    printf("  unlink tagname filename    Remove a link between a file and a tag\n");
    printf("  rename oldname newname     Rename a file or tag\n");
    printf("  open name                  Open file in standard application\n"); //leak and error free
    printf("  clear                      Clear the tag file, deleting all tags\n"); // leak and eror free
    printf("  create name filepath       Create a txt file and add it to the file system\n"); //leak and error free
    printf("  delete name                Delete a file or tag entirely\n"); // leak and error free
    printf("\n");
    printf("Options:\n");
    printf("  -h, --help                 Display this help message\n");
    printf("\n");
    exit(1);
}

int add(int argc, char *argv[]) { //TODO: still leaks memory
    char *name = argv[1];
    char *type = argv[2];
    char header_path[256];  // Allocate memory for header_path
    char file_create_path[256];  // Allocate memory for file_create_path
    int id = 0;
    int *nodes = getUsedIDs();
    if (argc < 3 ) {
        printf("Error: Too few arguments supplied\n");
        free(nodes);
        return -1;
    }
    if (strcmp(type, "file") != 0 && strcmp(type, "tag") != 0) {
        printf("Error: Please specify type.\n");
        free(nodes);
        return 0;
    }

    printf("Checking if file exists\n");
    // check if file already exists
    for (int i = 0; i < graph->nodeCount; i++) {
        id = nodes[i];
        // Open corr. header file and read name
        // return 1 if exists ie == name
        printf("Checking node: %d\n", id);
        if (sprintf(header_path, "data/%d.txt", id) < 0) {  // Check if sprintf was successful
            printf("Error: couldn't create header path\n");
            free(nodes);
            return -1;  // Return an error code
        }

        char *read_name = readNthLine(header_path, 1);

        if (read_name != NULL && strcmp(read_name, name) == 0) { //name should be unique?
            printf("Name already used!\n");
            free(read_name);
            free(nodes);
            return -1;
        }
        free(read_name);
    }

    // Find unused id
    int newId = getUnusedID2(graph);

    // Create header file path
    if (sprintf(file_create_path, "data/%d.txt", newId) < 0) {
        printf("Error: Couldn't create header file\n");
        free(nodes);
        return -1;
    }

    // Add Node to graph
    addNode(graph, newId, type);

    if (strcmp(type, "file") == 0) {
        if (argv[3] == NULL) {
            printf("Error: Please supply a filepath to the file you are adding\n");
            free(nodes);
            return -1;
        }
    }

    // Create new head file 
    FILE *head_file = fopen(file_create_path, "w");
    if (head_file == NULL) {
        printf("Error: could not create header at %s\n", file_create_path);
        free(nodes);
        return -1;
    }

    if (strcmp(type, "file") == 0) {
        fprintf(head_file, "%s\n%s\n%s\n", name, type, argv[3]);
    } else {
        fprintf(head_file, "%s\n%s\n", name, type);
    }
    fclose(head_file);

    saveGraph(graph, "data/graph.txt");
    free(nodes);
    return 0;
}

// Remove header file and node from graph
int rmv(int argc, char *argv[]) {
    char * name = argv[1];
    char header_path[256];
    int *nodes = getUsedIDs();
    int i;
    int exists = 0;
    
    printf("Searching for node with name %s\n", name);
    // Check if file with name exists
    for (int j = 0; j < graph->nodeCount; j++) {
        i = nodes[j];
        if (sprintf(header_path, "data/%d.txt", i) < 0) {  // Check if sprintf was successful
            printf("Error: couldn't create header path\n");
            free(nodes);
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
        free(read_name);
    }

    // TODO??: Update graph
    free(nodes);
    return 0;
}

void list(void) {
    char header_path[256];
    int *nodes = getUsedIDs();
    int i;
    

    printf("Tags:\n");

    for (int j = 0; j < graph->nodeCount; j++) {
        i = nodes[j];
        if (sprintf(header_path, "data/%d.txt", i) < 0) {  // Check if sprintf was successful
            printf("Error: couldn't create header path\n");
            free(nodes);
            return;  // Return an error code
        }

        char *read_name = readNthLine(header_path, 1);
        char *read_type = readNthLine(header_path, 2);

        if (read_type != NULL && strcmp(read_type, "tag") == 0) {
            printf("   %s\n", read_name);
        }
        free(read_name);
        free(read_type);
    }
    free(nodes);
    return;
}

void listUntaggedFiles(void) {
    char header_path[256];
    int *nodes = getUsedIDs();
    int i;


    printf("Untagged Files:\n");

    for (int j = 0; j < graph->nodeCount; j++) {
        i = nodes[j];
        if (sprintf(header_path, "data/%d.txt", i) < 0) {  // Check if sprintf was successful
            printf("Error: couldn't create header path\n");
            free(nodes);
            return;  // Return an error code
        }

        char *read_name = readNthLine(header_path, 1);
        char *read_type = readNthLine(header_path, 2);

        if (read_type != NULL && strcmp(read_type, "file") == 0) {
            if (!hasNeighbour(graph, i)) {
                //printf("i is %d and nodeCount is %d\n", i, graph->nodeCount);
                printf("    %s\n", read_name);
            }
        }
        free(read_name);
        free(read_type);
    }
    free(nodes);
    return;
}

void clear(void) {
    removeDirectory("data");
    return;
}

int link(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Error: Too few arguments supplied\n");
        return -1;
    }
    char header_path[256];
    char *name1 = argv[1];
    char *name2 = argv[2];
    int file1_exists = 0;
    int file2_exists = 0;
    int id1 = 0;
    int id2 = 0;
    int *nodes = getUsedIDs();
    int i;
    

    // Check for file 1
    for (int j = 0; j < graph->nodeCount; j++) {
        i = nodes[j];
        sprintf(header_path, "data/%d.txt", i);
        char *read_name = readNthLine(header_path, 1);
        char *read_type = readNthLine(header_path, 2);

        if (read_name != NULL && strcmp(read_name, name1) == 0) {
            printf("Found file with name %s\n", read_name);
            if (strcmp(read_type, "tag") == 0) {
                file1_exists = 1;
            } else if (strcmp(read_type, "file") == 0) {
                file1_exists = 2;
            }
            id1 = i;
        }
        free(read_name);
        free(read_type);
    }

    // Check for file 2
    for (int j = 0; j < graph->nodeCount; j++) {
        i = nodes[j];
        sprintf(header_path, "data/%d.txt", i);
        char *read_name = readNthLine(header_path, 1);
        char *read_type = readNthLine(header_path, 2);

        if (read_name != NULL && strcmp(read_name, name2) == 0) {
            printf("Found file with name %s\n", read_name);
            if (strcmp(read_type, "tag") == 0) {
                file2_exists = 1;
            } else if (strcmp(read_type, "file") == 0) {
                file2_exists = 2;
            }
            id2 = i;
        }
        free(read_name);
        free(read_type);
    }

    // Check if they are different
    if (file1_exists == 0 || file2_exists == 0 || file1_exists == file2_exists) {
        printf("Error: Couldn't find files with different types (Key: %d %d)\n", file1_exists, file2_exists);
        free(nodes);
        return -1;
    }

    addEdge(graph, id1, id2);
    saveGraph(graph, GRAPH_FILE);
    free(nodes);
    return 0;
}

int unlink(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Error: Too few arguments supplied\n");
        return -1;
    }
    char header_path[256];
    char *name1 = argv[1];
    char *name2 = argv[2];
    int file1_exists = 0;
    int file2_exists = 0;
    int id1 = 0;
    int id2 = 0;
    int *nodes = getUsedIDs();
    int i;

    // Check for file 1
    for (int j = 0; j < graph->nodeCount; j++) {
        i = nodes[j];
        sprintf(header_path, "data/%d.txt", i);
        char *read_name = readNthLine(header_path, 1);
        char *read_type = readNthLine(header_path, 2);

        if (read_name != NULL && strcmp(read_name, name1) == 0) {
            printf("Found file with name %s\n", read_name);
            if (strcmp(read_type, "tag") == 0) {
                file1_exists = 1;
            } else if (strcmp(read_type, "file") == 0) {
                file1_exists = 2;
            }
            id1 = i;
        }
        free(read_name);
        free(read_type);
    }

    // Check for file 2
    for (int j = 0; j < graph->nodeCount; j++) {
        i = nodes[j];
        sprintf(header_path, "data/%d.txt", i);
        char *read_name = readNthLine(header_path, 1);
        char *read_type = readNthLine(header_path, 2);

        if (read_name != NULL && strcmp(read_name, name2) == 0) {
            printf("Found file with name %s\n", read_name);
            if (strcmp(read_type, "tag") == 0) {
                file2_exists = 1;
            } else if (strcmp(read_type, "file") == 0) {
                file2_exists = 2;
            }
            id2 = i;
        }
        free(read_name);
        free(read_type);
    }

    // Check if they are different
    if (file1_exists == 0 || file2_exists == 0 || file1_exists == file2_exists) {
        printf("Error: Couldn't find files with different types (Key: %d %d)\n", file1_exists, file2_exists);
        free(nodes);
        return -1;
    }

    removeEdge(graph, id1, id2);
    saveGraph(graph, GRAPH_FILE);
    free(nodes);
    printf("Success\n");
    return 0;
}

int print_adj(int nodeId) {
    char header_path[256];
    int i;
    int *nodes = getUsedIDs();

    printf("  Files:\n");
    
    for (int j = 0; j < graph->nodeCount; j++) {
        i = nodes[j];
        if (hasEdge(graph, nodeId, i)) {
            sprintf(header_path, "data/%d.txt", i);
            char *read_name = readNthLine(header_path, 1);
            printf("    %s\n", read_name);
            free(read_name);
        }

    }
    free(nodes);
    return 0;
}

int fileinfo(int argc, char *argv[]) {
    char *name1 = argv[1];
    char header_path[256];
    int i;
    int *nodes = getUsedIDs();

    // Check for file 2
    for (int j = 0; j < graph->nodeCount; j++) {
        i = nodes[j];
        sprintf(header_path, "data/%d.txt", i);
        char *read_name = readNthLine(header_path, 1);
        char *read_type = readNthLine(header_path, 2);

        if (read_name != NULL && strcmp(read_name, name1) == 0) {
            if (strcmp(read_type, "tag") == 0) {
                printf("INFO\n");
                printf("  Id:   %d\n", i);
                printf("  Name: %s\n", read_name);
                print_adj(i);
            } else if (strcmp(read_type, "file") == 0) {
                printf("INFO\n");
                printf("  Id:   %d\n", i);
                printf("  Name: %s\n", read_name);
                char * path = readNthLine(header_path, 3);
                char * perm = readNthLine(header_path, 4);
                printf("  Path: %s\n", path);
                //printf("  Perm: %s\n", perm);
                free(path);
                free(perm);
            }
        }
        free(read_name);
        free(read_type);
    }
    free(nodes);
    return 0;
}

int open(int argc, char *argv[]) {
    char *name1 = argv[1];
    char header_path[256];
    int i;
    int *nodes = getUsedIDs();
    int file_exits = 0;

    // Check for file, don't open tags (possible but not useful), open file via interactionSys
    for (int j = 0; j < graph->nodeCount; j++) {
        i = nodes[j];
        int ret = sprintf(header_path, "data/%d.txt", i);
        char *read_name = readNthLine(header_path, 1);
        char *read_type = readNthLine(header_path, 2);
        if (read_name != NULL && strcmp(read_name, name1) == 0) {
            if (strcmp(read_type, "file") == 0) {
                file_exits = 0;
                char* path = readNthLine(header_path, 3);
                if (path == NULL) {
                    printf("Error: To few arguments.\n");
                    return 1;
                }
                openFile(path);
                free(path);
                free(nodes);
                free(read_name);
                free(read_type);
                return 0;
            } else if (strcmp(read_type, "tag") == 0) {
                file_exits = 0;
                printf("Can't open a tag. Try opening a file.\n");
                free(read_name);
                free(read_type);
                free(nodes);
                return 0;
            }
        } else if (strcmp(read_name, name1) != 0) {
            file_exits = 1;
        }
        free(read_name);
        free(read_type);
    }
    if (file_exits != 0) { // exception handeling
        printf("Error: Specified file doesn't seem to exist. Check for typo or add file.\n");
    }
    free(nodes);
    return 0;
}

int create(int argc,char *argv[]) {
    if (argc < 3) {
        printf("Error: Please enter a name and a file path where you want to create the file\n");
    }

    char *name = argv[1];
    char *path = argv[2];

    if (createFile(path) != 0) {
        return -1;
    }

    char *newargs[4]; 
    newargs[1] = name;
    newargs[2] = "file";
    newargs[3] = path;
    return add(4, newargs);
}

void deletefile(int argc, char *argv[]) {
    char *name = argv[1];
    char header_path[256];
    int i;
    int *nodes = getUsedIDs();

    // Check for file
    for (int j = 0; j < graph->nodeCount; j++) {
        i = nodes[j];
        sprintf(header_path, "data/%d.txt", i);
        char *read_name = readNthLine(header_path, 1);
        char *read_type = readNthLine(header_path, 2);

        if (read_name != NULL && strcmp(name, read_name) == 0) {
            printf("Attempting to delete %d (key %d)\n", i, strcmp(read_type, "file"));
            if (strcmp(read_type, "file") == 0) {
                printf("Yes! It's a file!");
                char *read_path = readNthLine(header_path, 3);
                printf("Deleting file %s\n", read_path);
                if (remove(read_path) == 0) {
                    printf("File deleted successfully.\n");
                } else {
                    printf("Unable to delete the file.\n");
                }
                free(read_path);
            } 
            rmv(argc, argv);
            free(read_name);
            free(read_type);
            free(nodes);
            return;
        }
        free(read_name);
        free(read_type);
    }

    free(nodes);
    return;
}

int renamefile(int argc, char *argv[]) {
    char *oldName = argv[1];
    char *newName = malloc(strlen(argv[2]) + 2);  // Allocate memory for newName
    char header_path[256];
    int i;
    int *nodes = getUsedIDs();

    if (newName == NULL) {
        printf("Error: Memory allocation failed.\n");
        free(nodes);
        free(newName);
        return -1;
    }

    strcpy(newName, argv[2]);  // Copy argv[2] to newName

    // Add newline character to string
    size_t len = strlen(newName);
    newName = realloc(newName, len + 2);
    strcat(newName, "\n");

    // Check for file
    for (int j = 0; j < graph->nodeCount; j++) {
        i = nodes[j];
        sprintf(header_path, "data/%d.txt", i);
        char *read_name = readNthLine(header_path, 1);
        char *read_type = readNthLine(header_path, 2);

        if (read_name != NULL && strcmp(read_name, oldName) == 0) {
            replaceNthLine(header_path, 1, newName);
            free(nodes);
            free(newName);
            free(read_name);
            free(read_type);
            return 0;
        }
        free(read_name);
        free(read_type);
    }

    free(newName);

    printf("Error: Couldn't find file %s\n", oldName);
    free(nodes);
    return -1;
}

int main(int argc, char *argv[]) {
    // Create the data directory if it doesn't exist
    struct stat st = {0};
    if (stat("data", &st) == -1) {
        mkdir("data", 0777);
    }

    graph = loadGraph("data/graph.txt"); //TODO: causes mem leak
    if (graph == NULL) {
        graph = createGraph();
        saveGraph(graph, "data/graph.txt");
    }

    // Parse command-line arguments
    if (argc < 2) {
        freeGraph(graph);
        print_usage_and_exit();
    }

    char *command = argv[1];

    if (strcmp(command, "add") == 0) {
        add(argc - 1, &argv[1]);
    } else if (strcmp(command, "remove") == 0) {
        //printf("Here we have %d arguments.\n", argc);
        if (argc < 3) {
            printf("Error: Not enough arguments supplied.\n");
        } else {
            rmv(argc - 1, &argv[1]);
        }
    } else if (strcmp(command, "clear") == 0) {
        clear();
    } else if (strcmp(command, "list") == 0) {
        list();
    } else if (strcmp(command, "lost") == 0) {
        listUntaggedFiles();
    }else if (strcmp(command, "link") == 0) {
        link(argc - 1, &argv[1]);
    } else if (strcmp(command, "open") == 0) {
        //printf("Here we have %d arguments.\n", argc);
        if (argc < 3) {
            printf("Error: Not enough arguments supplied.\n");
        } else {
            open(argc - 1, &argv[1]);
        }
    }else if (strcmp(command, "create") == 0) {
        if (argc < 3) {
            printf("Error: Not enough arguments supplied.\n");
        } else {
            create(argc - 1, &argv[1]);
        }
    } else if (strcmp(command, "delete") == 0) {
        if (argc < 3) {
            printf("Error: Not enough arguments supplied.\n");
        } else {
            deletefile(argc - 1, &argv[1]);
        }
    } else if (strcmp(command, "unlink") == 0) {
        unlink(argc - 1, &argv[1]);
    } else if (strcmp(command, "info") == 0) {
        fileinfo(argc - 1, &argv[1]);
    } else if (strcmp(command, "rename") == 0) {
        if (argc < 3) {
            printf("Error: Not enough arguments supplied.\n");
        } else {
            renamefile(argc - 1, &argv[1]);
        }
    } else if (strcmp(command, "-h") == 0 || strcmp(command, "--help") == 0) {
        freeGraph(graph);
        print_usage_and_exit();
    } else {
        fprintf(stderr, "Error: Unknown command '%s'\n", command);
        freeGraph(graph);
        exit(1);
    }
    //printf("Before free graph\n");
    freeGraph(graph);//TODO: make sure it frees everything
    return 0;
}
