#include <stdio.h>
#include "../include/graph.h"

struct node
{
    int vertex;
    struct node *next;
};

struct graph
{
    int numVertices;
    struct node** adjLists;
};
