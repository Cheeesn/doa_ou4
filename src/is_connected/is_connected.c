#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "graph.h"
#include "array_1d.h"
#include <string.h>
#include "queue.h"

#define MAXNODENAME 40
#define BUFSIZE 400
typedef struct map{
    array_1d *src;
    array_1d *dest;
}map;
/**
 * @brief Find position of first non-whitespace character.
 *
 * @param s - string to search.
 * @return int - Returns the position of the first non-whitespace character, or -1 if not found.
 */
int first_non_white_space(const char *s)
{
    int i = 0; // Start at first char.

    // Advance as long as we're loooking at white-space until we hit EOL.
    while (s[i] && isspace(s[i])) {
        i++;
    }
    // We found a non-whitespace char.
    if (s[i]) {
        // It was a proper characted. Return its position.
        return i;
    } else {
        // It was the null terminator. Return fail.
        return -1;
    }
}

/**
 * @brief Determine if the string is blank.
 *
 * @param s - string to search.
 * @return true if the line contains only whitespace chars.
 * @return false if the line contains at least one non-whitespace char.
 */

bool line_is_blank(const char *s)
{
    // Line is blank if it only contained white-space chars.
    return first_non_white_space(s) < 0;
}

/**
 * @brief Determina if the string is a comment line.
 *
 * @param s - string to search.
 * @return true if the line is a comment line.
 * @return false if the line is not a comment line.
 *v (ARGument Vector) is an array of character pointers listing all the arguments.
 * A comment line has a hash sigelse {
            
        }n '#' as the first non-whitespace char on the line.
 */
bool line_is_comment(const char *s)
{
    int i = first_non_white_space(s);
    return (i >= 0 && s[i] == '#');
}

/**
 * @brief Extract node names from a line from the map file.
 *
 * @param buf - Input buffer.
 * @param n1 - Output buffer for the first node name. Must be at least MAXNODENAME+1 long.
 * @param n2 - Ditto for the second node name.
 * @return int - Returns the number of correctly parsed node names. If the return value is 2, both n1
 * and n2 contain node names. If the return value is less than 2, parsing of at least one node name
 * failed, in which case the content of n1 and n2 are undefined.
 */
int parse_map_line(const char *buf, char *n1, char *n2)
{
    // Create a format string the will do the work.
    char fmt[20];
    // This will generate the format string " %40s %40s" if MAXNODENAME is 40.
    snprintf(fmt, sizeof(fmt), " %%%ds %%%ds", MAXNODENAME, MAXNODENAME);

    // sscanf does all the necessary parsing.
    // Node names must be separated by whitespace.
    // Whitespace before the first node name is allowed.
    // Anything after the second node name is ignored.
    int n = sscanf(buf, fmt, n1, n2);

    // The return value from sscanf contains the number of properly parsed format codes, i.e. the
    // number of node names.
    return n;
}

/**
 * @brief Load a map from a file.
 *
 * This function reads a map from a file and stores it in a data structure.
 *
 * @param map - Pointer to the map structure to be filled.
 * @param fp - File pointer to the input file.
 * @return int - Number of edges loaded from the file.
 */
int load_map(map *map, FILE *fp) {
    
    char buffer[BUFSIZE];
    char buffernode1[MAXNODENAME];
    char buffernode2[MAXNODENAME];
    
    bool find_start = false;
    int edges = 0;
    int i = 0;
    char * my_copy;
    //Bool that changes when the first non comment/blank is found
    while(!find_start){
        
        fgets(buffer,BUFSIZE-1,fp);
        //Checks that the line is not a comment or blank
        if(!line_is_blank(buffer) && !line_is_comment(buffer)){
            //converting char to int to get the amound of edges
            edges = atoi(buffer);
            find_start = true;
        }
        
    }
        
    if(edges == 0){
        fprintf(stderr,"File doesnt contain an integer for edges\n");
        exit(EXIT_FAILURE);
    }
    //Creating arrays to save the nodes in
    map->src = array_1d_create(0,edges*2,free);
    map->dest = array_1d_create(0,edges*2,free);
    //looping untill EOF
    while(fgets(buffer,BUFSIZE-1,fp)){
        
        //Checking if the nodes are correctly formated in the file
        if(parse_map_line(buffer,buffernode1,buffernode2) == 2){
            //Allocating memory for array element
            my_copy = malloc(sizeof(char) * MAXNODENAME);
            strcpy(my_copy,buffernode1);
            //setting source array
            array_1d_set_value(map->src,my_copy,i);
            //Allocating memory for array element again
            my_copy = malloc(sizeof(char) * MAXNODENAME);
            strcpy(my_copy,buffernode2);
            //Setting destination array
            array_1d_set_value(map->dest,my_copy,i);
        }
        //Ignoring comments or blanks in the middle of the textfile
        else if(line_is_blank(buffer) || line_is_comment(buffer)){
            continue;
        }
        else{
            fprintf(stderr,"File doesnt follow specifications\n");
            exit(EXIT_FAILURE);
        }
        i++;
    }
    //Closing file because it doesnt need to open anymore
    fclose(fp);
    return edges;
}

/**
 * @brief Create a graph from loaded map data.
 *
 * This function creates a graph based on the loaded map data.
 *
 * @param graph - Pointer to the graph structure to be created.
 * @param edges - Number of edges in the map.
 * @param m - Pointer to the map structure containing the data.
 */
void create_map(graph *graph, int edges,map *m){

    int i = 0;
    
    char *buffersrc;
    char *bufferdest;
    //Looping through the whole array
    while(i < edges){
        //saving src/dest to a variable for inserting to graph
        buffersrc = array_1d_inspect_value(m->src,i);
        bufferdest = array_1d_inspect_value(m->dest,i);
        
        //Check if a node with the name in buffersrc exists
        node *node1 = graph_find_node(graph, buffersrc);
        
        //Checking if the src node already exists and adding it to the graph if it does'nt
        if(node1 == NULL){
            graph = graph_insert_node(graph, buffersrc);
            //finding node again otherwise it's undefined
            node1 = graph_find_node(graph, buffersrc);
            
        }
        
        //Check if a node with the name in bufferdest exists
        node *node2 = graph_find_node(graph, bufferdest);

        //The same check but for the destination node
        if(node2 == NULL){
            graph = graph_insert_node(graph, bufferdest);
            //finding node again otherwise it's undefined
            node2 = graph_find_node(graph, bufferdest);
            
        }
        //Insterting the dest node as a neighbour to the source node
        graph = graph_insert_edge(graph,node1, node2);
        
        i++;
    }
    //Freeing memory for the arrays because they are no longer needed
    array_1d_kill(m->src);
    array_1d_kill(m->dest);
    //Freeing the rest of the struct
    free(m);
}

/**
 * @brief Check if the file is valid and open it.
 *
 * This function checks if the file is valid and opens it for reading.
 *
 * @param argv - Array of command-line arguments.
 * @param argc - Number of command-line arguments.
 * @param map_file - Pointer to the file pointer to be set.
 */
void check_file(const char *argv[], int argc, FILE **map_file){
    //Checking if its the right amount of commandline arguments
    if(argc != 2) {
        
        fprintf(stderr,"Incorrect command line argument\n");
        exit(EXIT_FAILURE);
    }

    *map_file = fopen(argv[1], "r"); 
    //Map file is unable to open
    if(*map_file == NULL) {
        
        fprintf(stderr,"Could not open file: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

}

/**
 * @brief Find a path between two nodes in a graph.
 *
 * This function finds a path between two nodes in a graph using breadth-first search.
 *
 * @param g - Pointer to the graph structure.
 * @param src - Pointer to the source node.
 * @param dest - Pointer to the destination node.
 * @return bool - True if a path is found, false otherwise.
 */
bool find_path(graph *g, node *src, node *dest){
    
    queue *q  = queue_empty(NULL);
    
    g = graph_node_set_seen(g,src,true);
    q = queue_enqueue(q, src);
    //Looping through the whole que
    while(!queue_is_empty(q)){
        //taking out the top element of the queue for later use
        node *currentnode = queue_front(q);
        //Removing the top element of the queue
        q = queue_dequeue(q);
        dlist *neighbourlist = graph_neighbours(g,currentnode);
        dlist_pos pos = dlist_first(neighbourlist);
        
        //Looping through the whole list of neighbours
        while(!dlist_is_end(neighbourlist,pos)){
            //The current neighbour that gets compared to the dest node
            node *neighbour = dlist_inspect(neighbourlist,pos);
            //If a match is found in the neighbour list a path is found
            if(nodes_are_equal(neighbour, dest)){
                //Resetting seen status and freeing memory
                g = graph_reset_seen(g);
                queue_kill(q);
                dlist_kill(neighbourlist);
                return true;
            }
            //If the neighbour has not been seen yet mark as seen and then queue to search their neighbours
            if(!graph_node_is_seen(g,neighbour)){
                
                g = graph_node_set_seen(g,neighbour,true);
                q = queue_enqueue(q,neighbour);
            }
            //Incrementing list
            pos = dlist_next(neighbourlist, pos);
            
        }
    //Freeing the list
    dlist_kill(neighbourlist);
    
    }

    //Freeing the queue and resetting seen status
    queue_kill(q);
    graph_reset_seen(g);
    return false;

}

/**
 * @brief User input loop for finding paths in a graph.
 *
 * This function prompts the user for input and finds paths between nodes in the graph.
 *
 * @param g - Pointer to the graph structure.
 */
void Userinput(graph *g){
    //Buffers for input and nodes
    char input[MAXNODENAME*2+1];
    char node1buffer[MAXNODENAME];
    char node2buffer[MAXNODENAME];
    //Variable for checking if the input is correct
    int checkinput = 0;

    do{
        printf("Enter origin and destination (quit to exit): ");
        
        fgets(input,sizeof(input),stdin);
        //Variable to check if the input is correctly formatted
        checkinput = parse_map_line(input,node1buffer,node2buffer);
        //Finding both nodes in the graph
        node *src = graph_find_node(g,node1buffer);
        node *dest = graph_find_node(g,node2buffer);
        //If input is quit exit program
        if(strcmp(node1buffer, "quit") == 0){
            printf("Normal exit.\n");
            graph_kill(g);
            return;
        }
        
        if(checkinput == 2){
            //Checking if origin exists
            if(graph_find_node(g,node1buffer) == NULL){
                printf("origin does not exist\n");
            
            }
            //Checking if destination exists
            else if(graph_find_node(g,node2buffer) == NULL){
                printf("destination does not exist\n");
            }
            //If find path returns true there is a path between src and node
            else if(find_path(g,src,dest)){
                
                printf("There is a path from %s to %s.\n", node1buffer, node2buffer);
            }
            else{
                printf("There is no path from %s to %s.\n", node1buffer, node2buffer);
            }
        }
        else{
            printf("Invalid input separate location by a space\n");
        }
        
    }while(1);
}

int main(int argc, const char *argv[]){
    
    FILE *map_file = NULL;
    //Allocating map struct to save the nodes in
    map *map = malloc(sizeof(*map));
    int edges;
    //Checking if the file is readable and the if its the correct amount of arguments
    check_file(argv,argc,&map_file);
    //loading the text file into the struct
    edges = load_map(map,map_file);
    graph *graph = graph_empty(edges*2);
    //Creating graph
    create_map(graph,edges,map);
    //Starting user input
    Userinput(graph);

    return 0;
}
