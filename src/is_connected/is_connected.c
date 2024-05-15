#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "graph.h"
#include "array_1d.h"
#include <string.h>
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
int load_map(map *map, FILE *fp) {
    
    char buffer[BUFSIZE];
    
    char buffernode1[MAXNODENAME];
    char buffernode2[MAXNODENAME];
    bool find_start = false;
    int edges = 0;
    int i = 0;
    char * my_copy;
    
    while(!find_start){
        
        fgets(buffer,BUFSIZE-1,fp);
        
        if(!line_is_blank(buffer) && !line_is_comment(buffer)){
            
            edges = atoi(buffer);
            find_start = true;
            
        }
        
    }
        
    
    if(edges == 0){
        fprintf(stderr,"File doesnt contain an integer for edges\n");
        exit(EXIT_FAILURE);
    }
    map->src = array_1d_create(0,edges*2,free);
    map->dest = array_1d_create(0,edges*2,free);
    while(i < edges){
        fgets(buffer,BUFSIZE-1,fp);
        
        if(parse_map_line(buffer,buffernode1,buffernode2) == 2){
            my_copy = malloc(sizeof(char) * MAXNODENAME);
            strcpy(my_copy,buffernode1);

            array_1d_set_value(map->src,my_copy,i);
            my_copy = malloc(sizeof(char) * MAXNODENAME);
            strcpy(my_copy,buffernode2);
            array_1d_set_value(map->dest,my_copy,i);
        }

        else {
            fprintf(stderr,"File doesnt follow specifications\n");
            exit(EXIT_FAILURE);
        }
        i++;
    }
    return edges;
}
void create_map(graph *graph, int edges,map *m){

    int i = 0;
    
    char *buffersrc;
    char *bufferdest;
    
    while(i < edges){
        buffersrc = array_1d_inspect_value(m->src,i);
        bufferdest = array_1d_inspect_value(m->dest,i);
        
        
        node *node1 = graph_find_node(graph, buffersrc);
        node *node2 = graph_find_node(graph, bufferdest);
        
        
        if(node1 == NULL){
            graph = graph_insert_node(graph, buffersrc);
            node1 = graph_find_node(graph, buffersrc);
            
        }
        if(node2 == NULL){
            graph = graph_insert_node(graph, bufferdest);
            node2 = graph_find_node(graph, bufferdest);
            
        }
        
        
        graph = graph_insert_edge(graph,node1, node2);
        
        i++;
    }
    
}

void check_file(const char *argv[], int argc, FILE **map_file){
    
    if(argc != 2) {
        
        fprintf(stderr,"Incorrect command line argument\n");
        exit(EXIT_FAILURE);
    }

    *map_file = fopen(argv[1], "r"); 

    if(*map_file == NULL) {
        
        fprintf(stderr,"Could not open file: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

}
int main(int argc, const char *argv[]){

    FILE *map_file = NULL;
    map *map = malloc(sizeof(*map));
    
    
    int edges;
    check_file(argv,argc,&map_file);
    
    edges = load_map(map,map_file);
    
    graph *graph = graph_empty(edges*2);
    
    create_map(graph,edges,map);
    
    graph_print(graph);
    

}
