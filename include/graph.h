#ifndef GRAPH_HEADER
#define GRAPH_HEADER

typedef struct Node {
    int id;
    char* type;
    struct Node* next;
} Node;

typedef struct Graph {
    Node** nodes;
    int nodeCount;
} Graph;

Graph* createGraph();
Node* createNode(int id, const char* type);
void addNode(Graph* graph, int id, const char* type);
void addEdge(Graph* graph, int id1, int id2);
void removeNode(Graph* graph, int id);
void removeEdge(Graph* graph, int id1, int id2);
void printGraph(Graph* graph);
void saveGraph(Graph* graph, const char* filename);
Graph* loadGraph(const char* filename);
int hasEdge(Graph* graph, int id1, int id2);
void freeGraph(Graph* graph);
void freeNode(Node* head);
int getUnusedID();
int* getUsedIDs();
int hasNeighbour(Graph* graph, int id);

#endif