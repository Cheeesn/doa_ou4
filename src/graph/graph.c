#include <graph.h>
#include <dlist.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node 
{
    char* label;//The nodes name, numeric string
    bool seen;//A bool corresponding if it has been seen
    dlist* neighbors;//A list including all neighbours to the node
} node;

typedef struct graph 
{
    dlist* nodes;//A list of the nodes inside the graph
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
    if (strcmp(n1->label, n2->label) == 0)
    {//To be considered the same node the name and neighbour list must also be the same
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
    dlist* nodes = dlist_empty(NULL);//Create an empty list of the nodes
    graph* graph = malloc(sizeof(*graph));//Allocate to heap the graph struct
    graph->nodes = nodes;//Give graph the node list
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
    return dlist_is_empty(g->nodes);//If the list of nodes is empty, then graph is empty
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
        if (dlist_is_empty(currentNode->neighbors))//If there isnt any neighbours we have found an edge
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

    strcpy(inputNode->label, s);//Copy input string into label

    inputNode->seen = false;//Not seen as standard
    inputNode->neighbors = dlist_empty(NULL);//Create neighbour list

    
    dlist_insert(g->nodes, inputNode, dlist_first(g->nodes));//Insert node into graph
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
        if (strcmp(current->label, s) == 0)
        {//If match at label and search string (s) we have found the right node
            return current;
        }
        pos = dlist_next(g->nodes, pos);
    }

    return NULL;//No node with the label of search string (s)
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
        node* current = dlist_inspect(g->nodes, pos);//Go through every node in graph
        current->seen = false;//Reset seen bool
        pos = dlist_next(g->nodes, pos);
    }

    return g;//Return a graph will all seen bools reset.
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
    dlist_insert(n1->neighbors, n2, dlist_first(n1->neighbors));//Add a neighbour to n1
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
        if (nodes_are_equal(n, current))//Find node to remove
        {
            //Remove all memory associated with the node
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
        {//Find match, and then remove the position from the neighbour list
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
    dlist *copyNeighbours = dlist_empty(NULL);//Create a copy of current neighbour list
    dlist_pos pos = dlist_first(n->neighbors);
    while (!dlist_is_end(n->neighbors, pos))
    {
        //Get out all nodes in the neighbour list, and then add it to the copyNeighbours list
        
        node* current = dlist_inspect(n->neighbors, pos);
        dlist_insert(copyNeighbours, current, dlist_first(copyNeighbours));
        pos = dlist_next(n->neighbors, pos);
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
        //Increment through every node in the graph and remove all of the memory assosiated with
        //that struct (node)

        node* current = dlist_inspect(g->nodes, pos);
        dlist_kill(current->neighbors);
        free(current->label);
        free(current);
        pos = dlist_next(g->nodes, pos);
    }

    dlist_kill(g->nodes);//Destroy node list
    free(g);//free the last part of the graph
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