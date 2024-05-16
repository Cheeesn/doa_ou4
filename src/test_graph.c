#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "graph.h"
#include "array_1d.h"
#include "dlist.h"

struct node {
    char *label;      // Name of the node 
    bool is_seen;     // Flag indicating if the node has been seen 
    dlist *neighbours; // List of neighbouring nodes 
}; 

struct graph {
    array_1d *nodes;  // Array containing the nodes 
    int numNodes;     // Number of nodes in the graph 
    int numEdges;     // Number of edges in the graph 
};

// Function to print neighbors of a given node recursively
void print_neighbors_recursive( graph *g, const node *n) {

    // Get the neighbors of the current node
    dlist *neighbors = graph_neighbours(g, n);

    dlist_pos pos = dlist_first(neighbors);

    if(dlist_is_empty(neighbors)){
        printf("no neighbors for %s \n", n->label);
        dlist_kill(neighbors);
        return;
    }
    else{
        
        printf("neighbors of: %s \n", n->label);

    }   


    // Iterate through each neighbor and print their label recursively
    while (!dlist_is_end(neighbors, pos)) {
        node *neighbor = dlist_inspect(neighbors, pos);
        printf("%s\n", neighbor->label);
        
        if (!graph_node_is_seen(g, neighbor)) {
            print_neighbors_recursive(g,neighbor);
        }
        graph_node_set_seen(g, neighbor, true);
        pos = dlist_next(neighbors, pos);
    }
    dlist_kill(neighbors);
}

int main() {
    // Create an empty graph with a maximum of 10 nodes
    graph *g = graph_empty(10);

    // Check if the graph is empty
    if (graph_is_empty(g)) {
        printf("The graph is empty.\n");
    } else {
        printf("The graph is not empty.\n");
    }

    // Insert nodes into the graph
    g = graph_insert_node(g, "Node1");
    g = graph_insert_node(g, "Node2");
    g = graph_insert_node(g, "Node3");
    g = graph_insert_node(g, "Node4");

   

    // Check if the graph is empty
    if (graph_is_empty(g)) {
        printf("The graph is empty.\n");
    } else {
        printf("The graph is not empty.\n");
    }

    node *node1 = graph_find_node(g,"Node1");
    node *node2 = graph_find_node(g,"Node2");
    node *node3 = graph_find_node(g,"Node3");
    node *node4 = graph_find_node(g,"Node4");
    // Print the graph
    printf("Graph after inserting nodes:\n");
    graph_print(g);

    
    graph_insert_edge(g, node1, node3);
    graph_insert_edge(g, node1, node2);
    graph_insert_edge(g, node1, node4);
    graph_insert_edge(g, node2, node3);
    
    printf("Graph after inserting edges:\n");
    graph_print(g);

    //Print neighbors of Node1 recursively
    print_neighbors_recursive(g, node1);
    graph_reset_seen(g);
    print_neighbors_recursive(g, node1);

    // Set the "seen" status of a node
    graph_node_set_seen(g, node1, true);
  
    if (graph_node_is_seen(g, node1)) {
        printf("node 1 is marked as seen.\n");
    } else {
        printf("node 1 is not marked as seen.");
        exit(EXIT_FAILURE);
    }

    // Reset the "seen" status of all nodes
    graph_reset_seen(g);
    if (!graph_node_is_seen(g, node1)) {
        printf("All nodes are reset to unseen.\n");
    }

    // Deallocate the graph
    graph_kill(g);
    printf("SUCCESS\n");

    return 0;
}
