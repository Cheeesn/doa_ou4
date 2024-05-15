#include <graph.h>
#include <dlist.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node 
{
    char* label;
    bool seen;
    dlist* neighbors;
} node;

typedef struct graph 
{
    dlist* nodes;
} graph;

/**
 * nodes_are_equal() - Check whether two nodes are equal.
 * @n1: Pointer to node 1.
 * @n2: Pointer to node 2.
 *
 * Returns: true if the nodes are considered equal, otherwise false.
 *
 */
bool nodes_are_equal(const node *n1,const node *n2)
{
    if (strcmp(n1->label, n2->label) && n1->neighbors == n2->neighbors)
    {
        return true;
    }
    else
    {
        return false;
    }    
}

/**
 * graph_empty() - Create an empty graph.
 * @max_nodes: The maximum number of nodes the graph can hold.
 *
 * Returns: A pointer to the new graph.
 */
graph *graph_empty(int max_nodes)
{
    dlist* nodes = dlist_empty(NULL);
    graph* graph = malloc(sizeof(*graph));
    graph->nodes = nodes;
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
    return dlist_is_empty(g->nodes);
}

/**
 * graph_has_edges() - Check if a graph has any edges.
 * @g: Graph to check.
 *
 * Returns: True if graph has any edges, otherwise false.
 */
bool graph_has_edges(const graph *g)
{
    dlist_pos pos = dlist_first(g->nodes);
    while (!dlist_is_end(g->nodes, pos))
    {
        node* currentNode = dlist_inspect(g->nodes, pos);
        if(currentNode->neighbors != NULL)
        {
            return true;
        }

        pos = dlist_next(g->nodes, pos);
    }

    return false;
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
    node* inputNode = malloc(sizeof(*inputNode));
    inputNode->label = malloc(sizeof(s));

    strcpy(inputNode->label, s);

    inputNode->seen = false;
    inputNode->neighbors = dlist_empty(NULL);

    
    dlist_insert(g->nodes, inputNode, dlist_first(g->nodes));
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
    dlist_pos pos = dlist_first(g->nodes);
    while (!dlist_is_end(g->nodes, pos))
    {
        node* current = dlist_inspect(g->nodes, pos);
        if (strcmp(current->label, s))
        {
            return current;
        }
        pos = dlist_next(g->nodes, pos);
    }

    return NULL;
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
    if (n->seen)
    {
        return true;
    }
    return false;
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
    dlist_pos pos = dlist_first(g->nodes);
    while (!dlist_is_end(g->nodes, pos))
    {
        node* current = dlist_inspect(g->nodes, pos);
        current->seen = false;
        pos = dlist_next(g->nodes, pos);
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
    dlist_insert(n1->neighbors, n2, dlist_first(n1->neighbors));
    return g;
}

/**
 * graph_delete_node() - Remove a node from the graph.
 * @g: Graph to manipulate.
 * @n: Node to remove from the graph.
 *
 * Returns: The modified graph.
 *
 * NOTE: Undefined if the node is not in the graph.
 */
graph *graph_delete_node(graph *g, node *n)
{
    dlist_pos pos = dlist_first(g->nodes);
    while (!dlist_is_end(g->nodes, pos))
    {
        node* current = dlist_inspect(g->nodes, pos);
        if (nodes_are_equal(n, current))
        {
            free(n->label);
            dlist_kill(n->neighbors);
            dlist_remove(g->nodes, pos);
            return g;
        }        
        pos = dlist_next(g->nodes, pos);
    }

    return g;
}

/**
 * graph_delete_edge() - Remove an edge from the graph.
 * @g: Graph to manipulate.
 * @n1: Source node (pointer) for the edge.
 * @n2: Destination node (pointer) for the edge.
 *NULL
 * Returns: The modified graph.
 *
 * NOTE: Undefined if the edge is not in the graph.
 */
graph *graph_delete_edge(graph *g, node *n1, node *n2)
{
    dlist_pos posNode1 = dlist_first(n1->neighbors);
    while (!dlist_is_end(n1->neighbors, posNode1))
    {
        node* current = dlist_inspect(n1->neighbors, posNode1);
        if (nodes_are_equal(n1, current))
        {
            dlist_remove(n1->neighbors, posNode1);
            return g;
        }
        posNode1 = dlist_next(n1->neighbors, posNode1);
    }

    return g;
}

/**
 * graph_choose_node() - Return an arbitrary node from the graph.
 * @g: Graph to inspect.
 *
 * Returns: A pointer to an arbitrayry node.
 *
 * NOTE: The return value is undefined for an empty graph.
 */
node *graph_choose_node(const graph *g)
{
    return dlist_inspect(g->nodes, dlist_first(g->nodes));
}

/**
 * graph_neighbours() - Return a list of neighbour nodes.
 * @g: Graph to inspect.
 * @n: Node to get neighbours for.
 *
 * Returns: A pointer to a list of nodes. Note: The list must be
 * dlist_kill()-ed after use.
 */
dlist *graph_neighbours(const graph *g,const node *n)
{
    dlist *copyNeighbours = dlist_empty(NULL);
    dlist_pos pos = dlist_first(n->neighbors);
    while (!dlist_is_end(n->neighbors, pos))
    {
        node* current = dlist_inspect(n->neighbors, pos);
        dlist_insert(copyNeighbours, current, dlist_first(n->neighbors));
    }   

    return copyNeighbours;
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
    dlist_pos pos = dlist_first(g->nodes);
    while (!dlist_is_end(g->nodes, pos))
    {
        node* current = dlist_inspect(g->nodes, pos);
        dlist_kill(current->neighbors);
        free(current->label);
        free(current);
        pos = dlist_next(g->nodes, pos);
    }

    dlist_kill(g->nodes);
    free(g);
}

/**
 * graph_print() - Iterate over the graph elements and print their values.
 * @g: Graph to inspect.
 *
 * Iterates over the graph and prints its contents.
 *
 * Returns: Nothing.
 */
void graph_print(const graph *g)
{
    dlist_pos pos = dlist_first(g->nodes);
    while (!dlist_is_end(g->nodes, pos))
    {
        node* current = dlist_inspect(g->nodes, pos);
        printf("label: %s\n", current->label);
        if (!dlist_is_empty(current->neighbors))
        {
            dlist_pos posNeighbours = dlist_first(current->neighbors);
            int neighbourNum = 0;
            while (!dlist_is_end(current->neighbors, posNeighbours))
            {
                node* currentNeighbour = dlist_inspect(current->neighbors, posNeighbours);
                printf("   Neighbor %d: %s\n", neighbourNum, currentNeighbour->label);
                neighbourNum++;
                posNeighbours = dlist_next(current->neighbors, posNeighbours);
            }
        
        }
        pos = dlist_next(g->nodes, pos);
    }
}