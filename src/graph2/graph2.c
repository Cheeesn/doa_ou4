#include <graph.h>
#include <dlist.h>
#include <array_2d.h>
#include <array_1d.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    char *label;//A string that you use to find the node
    bool seen;//A bool that corresponds to if the node has been visted
    int index;//The index the node is in the nodeMatrixTitles array 
} node;

typedef struct graph
{
    array_2d *nodesMatrix;//2d array (matrix) representing neighbours of nodes in the graph
    array_1d *nodeMatrixTitles;//1d array containing titles or labels for each node in the graph
    int sizeNodeMatrixTitles;//Integer storing the number of titles in nodeMatrixTitles.
} graph;

/**
 * nodes_are_equal() - Check whether two nodes are equal.
 * @n1: Pointer to node 1.
 * @n2: Pointer to node 2.
 *
 * Returns: true if the nodes are considered equal, otherwise false.
 *
 */
bool nodes_are_equal(const node *n1, const node *n2)
{
    if (strcmp(n1->label, n2->label))
    {//If labels are the same string, it is considered the same label
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Destroys a node.
 * 
 * This function frees the memory allocated for a node and its associated label.
 * 
 * @param n A pointer to the node to be destroyed.
 */
void destroy_node(void* n)
{
    node* nodeToDestroy = (node*)n;
    free(nodeToDestroy->label);
    free(nodeToDestroy);
}

/**
 * graph_empty() - Create an empty graph.
 * @max_nodes: The maximum number of nodes the graph can hold.
 *
 */
graph *graph_empty(int max_nodes)
{
    graph *graph = malloc(sizeof(*graph)); // Allocate to heap the graph struct
    graph->nodesMatrix = array_2d_create(0, max_nodes, 0, max_nodes, free);
    graph->nodeMatrixTitles = array_1d_create(0, max_nodes, destroy_node);

    //Check if the memory was allocated
    if (graph->nodesMatrix == NULL || graph == NULL || graph->nodeMatrixTitles == NULL)
    {
        fprintf(stderr, "Not enought memory, cannot allocate");
        return NULL;
    }

    graph->sizeNodeMatrixTitles = 0;
    return graph;
}

/**
 * graph_is_empty() - Check if a graph is empty, i.e. has no nodes.
 * @g: Graph to check.
 *
 * Returns: True if graph is empty, otherwise false.
 */
bool graph_is_empty(const graph *g)
{
    // Check if the nodeMatrixTitles array is empty by verifying if the first index has a value
    return !array_1d_has_value(g->nodeMatrixTitles, array_1d_low(g->nodeMatrixTitles));
}

/**
 * graph_insert_node() - Inserts a node with the given name into the graph.
 * @g: Graph to manipulate.
 * @s: Node name, memory must be freed outside of graph implementation
 *
 * Creates a new node with a copy of the given name and puts it into
 * the graph.
 *
 * Returns: The modified graph.
 */
graph *graph_insert_node(graph *g, const char *s)
{
    node *inputNode = malloc(sizeof(*inputNode));
    inputNode->label = malloc(sizeof(s));

    strcpy(inputNode->label, s); // Copy input string into label
    inputNode->seen = false;     // Not seen as standard

    array_1d_set_value(g->nodeMatrixTitles, inputNode, g->sizeNodeMatrixTitles);//Add the node into the title array
    inputNode->index = g->sizeNodeMatrixTitles;
    g->sizeNodeMatrixTitles++;
    
    return g;
}

/**
 * graph_find_node() - Find a node stored in the graph.
 * @g: Graph to manipulate.
 * @s: Node identifier, e.g. a char *.
 *
 * Returns: A pointer to the found node, or NULL.
 */
node *graph_find_node(const graph *g, const char *s)
{
    for (int i = 0; i < g->sizeNodeMatrixTitles; i++)
    {
        //Go through all nodes in the title array, if the labels are the same as an index,
        //then we have found the node

        node *current = array_1d_inspect_value(g->nodeMatrixTitles, i);
        if (strcmp(s, current->label) == 0)
        {
            return current;
        }
    }

    return NULL;//Cannot find node
}

/**
 * graph_node_is_seen() - Return the seen status for a node.
 * @g: Graph storing the node.
 * @n: Node in the graph to return seen status for.
 *
 * Returns: The seen status for the node.
 */
bool graph_node_is_seen(const graph *g, const node *n)
{
    return n->seen;
}

/**
 * graph_node_set_seen() - Set the seen status for a node.
 * @g: Graph storing the node.
 * @n: Node in the graph to set seen status for.
 * @s: Status to set.
 *
 * Returns: The modified graph.
 */
graph *graph_node_set_seen(graph *g, node *n, bool seen)
{
    n->seen = seen;
    return g;
}

/**
 * graph_reset_seen() - Reset the seen status on all nodes in the graph.
 * @g: Graph to modify.
 *
 * Returns: The modified graph.
 */
graph *graph_reset_seen(graph *g)
{
    for (int i = 0; i < g->sizeNodeMatrixTitles; i++)
    {
        //Go through every node in the array, and set seen as false (false is the standard value)
        node *current = array_1d_inspect_value(g->nodeMatrixTitles, i);
        current->seen = false;
    }

    return g;
}

/**
 * graph_insert_edge() - Insert an edge into the graph.
 * @g: Graph to manipulate.
 * @n1: Source node (pointer) for the edge.
 * @n2: Destination node (pointer) for the edge.
 *
 * NOTE: Undefined unless both nodes are already in the graph.
 *
 * Returns: The modified graph.
 */
graph *graph_insert_edge(graph *g, node *n1, node *n2)
{
    bool* thereIsPath = malloc(sizeof(*thereIsPath));
    *thereIsPath = true;
    array_2d_set_value(g->nodesMatrix, thereIsPath, n1->index, n2->index);
    return g;
}

/**
 * graph_neighbours() - Return a list of neighbour nodes.
 * @g: Graph to inspect.
 * @n: Node to get neighbours for.
 *
 * Returns: A pointer to a list of nodes. Note: The list must be
 * dlist_kill()-ed after use.
 */
dlist *graph_neighbours(const graph *g, const node *n)
{
    //This function will create a list using the 2d array row, for the specific node (n)

    dlist *listOfNeighbours = dlist_empty(NULL);
    for (int i = array_2d_low(g->nodesMatrix, 2); i <= array_2d_high(g->nodesMatrix, 2); i++)//Go through the whole row
    {
        if (array_2d_has_value(g->nodesMatrix, n->index, i))//If it has a value, there is a neighbour
        {
            node *neighbour = array_1d_inspect_value(g->nodeMatrixTitles, i);
            dlist_insert(listOfNeighbours, neighbour, dlist_first(listOfNeighbours));//Add neighbour to the list
        }
    }

    return listOfNeighbours;
}

/**
 * graph_kill() - Destroy a given graph.
 * @g: Graph to destroy.
 *
 * Return all dynamic memory used by the graph.
 *
 * Returns: Nothing.
 */
void graph_kill(graph *g)
{
    array_2d_kill(g->nodesMatrix);
    array_1d_kill(g->nodeMatrixTitles);
    free(g);
}

/**
 * graph_print() - Iterate over the graph elements and print their values.
 * @g: Graph to inspect.
 *
 * Iterates over the graph and prints itsint contents.
 *
 * Returns: Nothing.
 */
void graph_print(const graph *g)
{
    int i = 0;

    printf("      ");

    // Iterate over the nodes and print their label.
    for (int i = 0; i < g->sizeNodeMatrixTitles; i++)
    {
        node* current = array_1d_inspect_value(g->nodeMatrixTitles, i);
        printf(" %s", current->label);

    }
    printf("\n");


    for (int j = array_2d_low(g->nodesMatrix, 1); j <= array_2d_high(g->nodesMatrix, 1); j++)
    {
        printf("\r");
        if (i < g->sizeNodeMatrixTitles)
        {
            //Prints the labels of the nodes, on the left side of the matrix
            node* current = array_1d_inspect_value(g->nodeMatrixTitles, j);
            printf("%s  ", current->label);
            i++;
        }
        
        //This for-loop prints all the values in the 2d array for this specific row
        for (int k = array_2d_low(g->nodesMatrix, 2); k <= array_2d_high(g->nodesMatrix, 2); k++)
        {
            bool value = (bool*)array_2d_inspect_value(g->nodesMatrix, j, k);
            printf("  %d   ", value);
        }
        printf("\n");
    }
}