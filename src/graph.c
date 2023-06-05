#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/graph.h"

// Graph constructor
Graph* createGraph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->nodes = NULL;
    graph->nodeCount = 0;
    return graph;
}

// Node constructor, type should be "file" or "tag" NOT CHECKED!
Node* createNode(int id, const char* type) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->id = id;
    node->type = (char*)malloc(strlen(type) + 1);
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

// Use this to free the graph after use
void freeGraph(Graph* graph) {
    for (int i = 0; i < graph->nodeCount; i++) {
        Node* current = graph->nodes[i];
        while (current != NULL) {
            Node* next = current->next;
            free(current->type);
            free(current);
            current = next;
        }
    }

    free(graph->nodes);
    free(graph);
}

// Example main method for testing (comment out to avoid duplicate main)
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

    removeEdge(graph, 1, 3);
    removeNode(graph, 4);

    printGraph(graph);

    freeGraph(graph);
    return 0;
}
