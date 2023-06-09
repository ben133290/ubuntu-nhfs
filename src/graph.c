/*
How to use this:
There is an example main function at the bottom of this implementation.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../include/graph.h"

#ifndef GRAPH_FILE
#define GRAPH_FILE "data/graph.txt"
#endif

// Graph constructor
Graph* createGraph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->nodes = NULL;
    graph->nodeCount = 0;
    return graph;
}

// Node constructor, type should be "file" or "tag" NOT CHECKED!
Node* createNode(int id, const char* type) {
    Node* node = (Node*)malloc(sizeof(Node)); //TODO: Causes leak
    node->id = id;
    node->type = (char*)malloc(strlen(type) + 1); //TODO: Causes leak
    strcpy(node->type, type);
    node->next = NULL;
    return node;
}

// Add a node to the graph
void addNode(Graph* graph, int id, const char* type) {
    Node* newNode = createNode(id, type);

    if (graph->nodes == NULL) {
        // Initializes the adj. list
        graph->nodes = (Node**)malloc(sizeof(Node*));
        graph->nodes[0] = newNode;
    } else {
        graph->nodes = (Node**)realloc(graph->nodes, (graph->nodeCount + 1) * sizeof(Node*));
        graph->nodes[graph->nodeCount] = newNode;
    }

    graph->nodeCount++;
}

// Add an edge to the graph
void addEdge(Graph* graph, int id1, int id2) {
    Node* node1 = NULL;
    Node* node2 = NULL;

    // Check if both nodes exist (are in the graph)
    for (int i = 0; i < graph->nodeCount; i++) {
        if (graph->nodes[i]->id == id1) {
            node1 = graph->nodes[i];
        } else if (graph->nodes[i]->id == id2) {
            node2 = graph->nodes[i];
        }

        if (node1 != NULL && node2 != NULL) {
            break;
        }
    }

    if (node1 == NULL || node2 == NULL) {
        printf("One or both nodes not found.\n");
        return;
    }

    // Add edge between nodes
    Node* edge1 = createNode(id2, node2->type);
    Node* edge2 = createNode(id1, node1->type);

    Node* temp1 = node1->next;
    node1->next = edge1;
    edge1->next = temp1;

    Node* temp2 = node2->next;
    node2->next = edge2;
    edge2->next = temp2;
}

// Removes a node from the graph
void removeNode(Graph* graph, int id) {
    Node* nodeToDelete = NULL;
    int nodeIndex = -1;

    for (int i = 0; i < graph->nodeCount; i++) {
        if (graph->nodes[i]->id == id) {
            nodeToDelete = graph->nodes[i];
            nodeIndex = i;
            break;
        }
    }

    if (nodeToDelete == NULL) {
        printf("Node not found.\n");
        return;
    }

    for (int i = 0; i < graph->nodeCount; i++) {
        if (graph->nodes[i] == nodeToDelete) {
            continue;
        }

        Node* currentNode = graph->nodes[i];
        Node* previousNode = NULL;

        while (currentNode != NULL) {
            if (currentNode->id == id) {
                if (previousNode != NULL) {
                    previousNode->next = currentNode->next;
                } else {
                    graph->nodes[i] = currentNode->next;
                }
                free(currentNode);
                break;
            }
            previousNode = currentNode;
            currentNode = currentNode->next;
        }
    }

    free(nodeToDelete->type);
    free(nodeToDelete);

    for (int i = nodeIndex; i < graph->nodeCount - 1; i++) {
        graph->nodes[i] = graph->nodes[i + 1];
    }

    graph->nodes = (Node**)realloc(graph->nodes, (graph->nodeCount - 1) * sizeof(Node*));
    graph->nodeCount--;
}

// Removes an edge from the graph
void removeEdge(Graph* graph, int id1, int id2) {
    Node* node1 = NULL;
    Node* node2 = NULL;

    for (int i = 0; i < graph->nodeCount; i++) {
        if (graph->nodes[i]->id == id1) {
            node1 = graph->nodes[i];
        } else if (graph->nodes[i]->id == id2) {
            node2 = graph->nodes[i];
        }

        if (node1 != NULL && node2 != NULL) {
            break;
        }
    }

    if (node1 == NULL || node2 == NULL) {
        printf("One or both nodes not found.\n");
        return;
    }

    Node* current = node1->next;
    Node* previous = node1;

    while (current != NULL) {
        if (current->id == id2) {
            previous->next = current->next;
            free(current);
            break;
        }
        previous = current;
        current = current->next;
    }

    current = node2->next;
    previous = node2;

    while (current != NULL) {
        if (current->id == id1) {
            previous->next = current->next;
            free(current);
            break;
        }
        previous = current;
        current = current->next;
    }
}

// Prints to console a graph representation
void printGraph(Graph* graph) {
    printf("Graph:\n");

    for (int i = 0; i < graph->nodeCount; i++) {
        Node* node = graph->nodes[i];
        printf("Node ID: %d, Type: %s\n", node->id, node->type);
        printf("Edges: ");

        Node* current = node->next;
        while (current != NULL) {
            printf("%d ", current->id);
            current = current->next;
        }

        printf("\n");
    }
}

void saveGraph(Graph* graph, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Unable to open the file for saving.\n");
        return;
    }

    // first line is the number of nodes
    fprintf(file, "%d\n", graph->nodeCount);

    for (int i = 0; i < graph->nodeCount; i++) {
        Node* node = graph->nodes[i];
        fprintf(file, "%d %s ", node->id, node->type);

        Node* current = node->next;
        while (current != NULL) {
            fprintf(file, "%d ", current->id);
            current = current->next;
        }

        fprintf(file, "\n");
    }

    fclose(file);
}

Graph* loadGraph(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Unable to open the file for loading from %s.\n", filename);
        return NULL;
    }

    Graph* graph = createGraph();
    int nodeCount;
    char buffer[256]; // Used for skipping parts of the file with fgets
    fscanf(file, "%d", &nodeCount);

    // Read and add nodes
    for (int i = 0; i < nodeCount; i++) {
        int id;
        char type[50];

        fscanf(file, "%d %s", &id, type);
        addNode(graph, id, type);
        fgets(buffer, sizeof(buffer), file);
    }

    // Read and add edges
    fseek(file, 0, SEEK_SET);  // Reset file pointer to the beginning

    fgets(buffer, sizeof(buffer), file);  // Skip the first line

    memset(buffer, '\0', sizeof(buffer));
    int id;
    while (fgets(buffer, sizeof(buffer), file)) {
        // Skip type
        char type[50];
        sscanf(buffer, "%d %s", &id, type);

        // Read edges
        char* token = strtok(buffer, " ");  // Skip the ID and type
        token = strtok(NULL, " ");  // Start with the first edge ID
        token = strtok(NULL, " "); // Get first token (edge ID)

        while (token != NULL) {
            int edgeId;
            if (sscanf(token, "%d", &edgeId) == 1) {
                if (hasEdge(graph, id, edgeId) == 0) {
                    addEdge(graph, id, edgeId);
                }
            }
            token = strtok(NULL, " ");  // Get the next token (edge ID)
        }
        memset(buffer, '\0', sizeof(buffer));
    }

    fclose(file);
    return graph;
}

int hasEdge(Graph* graph, int id1, int id2) {
    // Find the node with id
    Node* node = NULL;
    for (int i = 0; i < graph->nodeCount; i++) {
        if (graph->nodes[i]->id == id1) {
            node = graph->nodes[i];
            break;
        }
    }

    // Traverse the edges of the node to check if id2 exists
    if (node != NULL) {
        Node* current = node->next;
        while (current != NULL) {
            if (current->id == id2) {
                return 1;  // Edge found
            }
            current = current->next;
        }
    }

    return 0;  // Edge not found
}

void freeNode(Node* head){
    //printf("Bas news: i got here\n");
    Node* helper;
    while(head != NULL) {
        helper = head;
        head = head->next;
        free(helper->type);
        free(helper);
    }
}

// Use this to free the graph after use
void freeGraph(Graph* graph) {
    //printf("Do i get here then?\n");
    for (int i = 0; i < graph->nodeCount; i++) {
        Node *current = graph->nodes[i];
        freeNode(current);
    }
    free(graph->nodes);
    free(graph);
}

int getUnusedID() { // misfunctioning methode, replaced by getUnusedID2
    char *filename = GRAPH_FILE;

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return -1;
    }

    int lowestMissingNumber = 0;
    int isFirstLine = 1;
    char line[256];

    while (fgets(line, sizeof(line), file) != NULL) {
        // Skip the first line
        if (isFirstLine) {
            isFirstLine = 0;
            continue;
        }

        // Check if the line is non-empty
        if (line[0] != '\n') {
            int num = atoi(line);

            // Update the lowest missing number if necessary
            if (num == lowestMissingNumber) {
                //printf("num = %d, lowestMissingNumber = %d\n", num, lowestMissingNumber);
                lowestMissingNumber++;
            }
        }

    }


    fclose(file);
    //printf("%d is lowest missing number\n", lowestMissingNumber);
    return lowestMissingNumber;
}

int idIsInGraph(Graph* graph, int id) {
    for (int i = 0; i < graph->nodeCount; ++i) {
        int helper = graph->nodes[i]->id;
        if(helper == id) {
            //printf("Id is here %d and helper is %d\n", id, helper);
            return 1; //is in graph
        }
    }
    return 0; // is not in graph
}

int getUnusedID2(Graph* graph) {
    int size = graph->nodeCount;
    int* numbers = getUsedIDs();
    int max = getMaxID(numbers, size);
    //printf("Max ID here is %d\n", max);
    for (int i = 0; i < max+3; ++i) { //give some add one of three to be sure
        if(!idIsInGraph(graph, i)) {
            printf("Unused Id is %d\n", i);
            free(numbers);
            return i;
        }
    }
    //printf("In line 388 of getUnusedID2\n");
    free(numbers);
    return 0;
}

int getMaxID(int* numbers, int size) {
    int max = 0;
    for (int i = 0; i < size; ++i) {
        if(max < numbers[i]) {
            max = numbers[i];
        }
    }
    return max;
}

int* getUsedIDs() {
    char *filename = GRAPH_FILE;
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return NULL;
    }

    int* numbers = NULL;
    int numbersCount = 0;
    char line[256];
    int isFirstLine = 1;

    while (fgets(line, sizeof(line), file) != NULL) {
        // Skip the first line
        if (isFirstLine) {
            isFirstLine = 0;
            continue;
        }

        // Check if the line is non-empty
        if (line[0] != '\n') {
            int num = atoi(&line[0]);

            // Resize the numbers array
            numbersCount++;
            numbers = (int*)realloc(numbers, numbersCount * sizeof(int));

            // Store the number in the array
            numbers[numbersCount - 1] = num;
        }
    }

    fclose(file);
    return numbers;
}

int hasNeighbour(Graph* graph, int id) { //returns 0 if there is no neighbour
    if(graph->nodes[id] == NULL) {
        printf("Node id is %d", id);
        return 0;
    } else if(graph->nodes[id]->next == NULL) {
        //printf(" Else if Node id is %d", id);
        //printGraph(graph);
        return 0;
    } else return 1;
}

// Example main method for testing (comment out to avoid duplicate main)
/*
int main() {

    Graph* graph = createGraph();

    addNode(graph, 1, "tag");
    addNode(graph, 2, "tag");
    addNode(graph, 3, "file");
    addNode(graph, 4, "file");

    addEdge(graph, 1, 3);
    addEdge(graph, 1, 4);
    addEdge(graph, 2, 3);

    printGraph(graph);

    saveGraph(graph, "../data/graph.txt");

    freeGraph(graph);

    Graph* loadedGraph = loadGraph("../data/graph.txt");
    if (loadedGraph != NULL) {
        printGraph(loadedGraph);
        freeGraph(loadedGraph);
    }
    return 0;
}
*/