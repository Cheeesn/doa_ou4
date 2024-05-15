#include <graph.h>
#include <dlist.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    graph *g = graph_empty(10);
    if (!graph_is_empty(g))
    {
        fprintf(stderr, "graph_empty returns wrong\n");
        return EXIT_FAILURE;
    }
    if (graph_has_edges(g))
    {
        fprintf(stderr, "graph has edges at creation\n");
        return EXIT_FAILURE;
    }

    char *label0 = "hej";
    char *label1 = "hejdå";

    g = graph_insert_node(g, label0);
    g = graph_insert_node(g, label1);

    node *node0 = graph_find_node(g, label0);
    node *node1 = graph_find_node(g, label1);
    if (node0 == NULL || node1 == NULL)
    {
        fprintf(stderr, "find node, cannot find nodes in graph\n");
        return EXIT_FAILURE;
    }

    g = graph_node_set_seen(g, node0, true);
    if (!graph_node_is_seen(g, node0))
    {
        fprintf(stderr, "node cannot be set as seen\n");
        return EXIT_FAILURE;
    }

    g = graph_reset_seen(g);
    if (graph_node_is_seen(g, node0))
    {
        fprintf(stderr, "graph_reset_seen doesnt work\n");
        return EXIT_FAILURE;
    }

    g = graph_insert_edge(g, node0, node1);
    dlist *neighboursNode0 = graph_neighbours(g, node0);
    if (neighboursNode0 == NULL)
    {
        fprintf(stderr, "cannot insert edges(Neighbours)\n");
        return EXIT_FAILURE;
    }

    graph_print(g);

    graph_kill(g);

    fprintf(stderr, "SUCCESS\n");
    return 0;
}