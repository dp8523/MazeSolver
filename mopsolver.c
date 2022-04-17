//
// File: mopsolver.c
// Description: 
//
// @author dp8523 Dara Prak
// 
// @date 04/15/2022
//
// // // // // // // // // // // // // // // // // // // // // // // // // //

#define _DEFAULT_SOURCE
#include <stdlib.h>
#include <stdio.h>

#include "QueueADT.h"

/// groups coordinate pairs for a cell

struct node_s
{
    int row;
    int col;
    char from;
};

typedef struct node_s *Node;

Node make_node(int row, int col, char from)
{
    Node node = (Node) malloc(sizeof(struct node_s));
    node->row = row;
    node->col = col;
    node->from = from;
    return node;
}

void get_neighbors(Node current, Node neighbors[])
{
    neighbors[0] = make_node(current->row - 1, current->col, 's');
    neighbors[1] = make_node(current->row + 1, current->col, 'n');
    neighbors[2] = make_node(current->row, current->col + 1, 'w');
    neighbors[3] = make_node(current->row, current->col - 1, 'e');
}

bool valid_cords(Node node, int num_rows, int num_cols)
{
    if(node->row > num_rows - 1 || node->row < 0)
    {
        return false;
    }
    if(node->col > num_cols - 1 || node->col < 0)
    {
        return false;
    }
    return true;
}

bool is_wall(Node node, char** maze)
{
    if(maze[node->row][node->col] == '#')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_solution(Node node, int num_rows, int num_cols)
{
    if(node->row == num_rows - 1 && node->col == num_cols - 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Node * solve_BFS(char **maze, int num_rows, int num_cols, int *steps)
{
    QueueADT to_visit = que_create(NULL);
    Node visited[num_rows][num_cols];
    for(int row = 0; row < num_rows; row++)
    {
        for(int col = 0; col < num_cols; col++)
        {
            visited[row][col] = NULL;
        }
    }
    const int MAX_NEIGHBORS = 4;
    int visited_count = 0;

    Node start = make_node(0, 0, '0');
    que_insert(to_visit, (void*) start);

    Node current;
    while(!que_empty(to_visit))
    {
        current = (Node) que_remove(to_visit);
        visited[current->row][current->col] = current;
        visited_count++;
        if(is_solution(current, num_rows, num_cols))
        {
            break;
        }
        Node neighbors[MAX_NEIGHBORS];
        get_neighbors(current, neighbors);

        for(int i = 0; i < MAX_NEIGHBORS; i++)
        {
            if(!valid_cords(neighbors[i], num_rows, num_cols) ||
                is_wall(neighbors[i], maze))
            {
                free(neighbors[i]);
                continue;
            }
            if(visited[neighbors[i]->row][neighbors[i]->col] != NULL)
            {
                continue;
            }
            que_insert(to_visit, (void*) neighbors[i]);
        }
    }
    que_destroy(to_visit);
    if(!(current->row == num_rows - 1 && current->col == num_cols - 1))
    {
        return NULL;
    }
    else
    {
        Node *path = (Node*) calloc(sizeof(Node), visited_count);
        path[*steps] = current;
        (*steps)++;
        while(!(current->row == 0 && current->col == 0))
        {
            int prev_row;
            int prev_col;
            if(current->from == 'n')
            {
                prev_row = current->row - 1;
                prev_col = current->col;
            }
            if(current->from == 's')
            {
                prev_row = current->row + 1;
                prev_col = current->col;
            }
            if(current->from == 'e')
            {
                prev_row = current->row;
                prev_col = current->col + 1;
            }
            if(current->from == 'w')
            {
                prev_row = current->row;
                prev_col = current->col - 1;
            }
            
            current = visited[prev_row][prev_col];
            path[*steps] = current;
            (*steps)++;
        }
        return path;
    }
}

char ** make_maze(char *filename, int *num_rows, int *num_cols)
{
    FILE *infile = fopen(filename, "r");
    char *line = NULL;
    size_t size = 0;
    QueueADT all_elements = que_create(NULL); // queue grows as elements are read
    while(getline(&line, &size, infile) != EOF)
    {
        int i = 0;
        while(line[i] != '\n')
        {
            if(line[i] == '0')
            {
                que_insert(all_elements, (void*) '.');
                if(*num_rows == 0)
                {
                    (*num_cols)++;
                }
            }
            if(line[i] == '1')
            {
                que_insert(all_elements, (void*) '#');
                if(*num_rows == 0)
                {
                    (*num_cols)++;
                }
            }
            i++;
        }
        (*num_rows)++;
    }
    free(line);
    fclose(infile);

    char **maze = (char**) calloc(sizeof(char*), *num_rows);
    for(int i = 0; i < *num_rows; i++)
    {
        maze[i] = (char*) calloc(sizeof(char), *num_cols);
    }

    for(int row = 0; row < *num_rows; row++)
    {
        for(int col = 0; col < *num_cols; col++)
        {
            maze[row][col] = (char) (long int) que_remove(all_elements);
        }
    }
    que_destroy(all_elements);
    return maze;
}

void print_maze(char** maze, int num_rows, int num_cols)
{
    printf("|");
    for(int i = 0; i < (num_cols * 2) + 1; i++)
    {
        printf("-");
    }
    printf("|\n");
    for(int row = 0; row < num_rows; row++)
    {
        if(row == 0)
        {
            printf("  ");
        }
        else
        {
            printf("| ");
        }
        for(int col = 0; col < num_cols; col++)
        {
            printf("%c ", maze[row][col]);
        }
        if(row == num_rows - 1)
        {
            printf(" \n");
        }
        else
        {
            printf("|\n");
        }
    }
    printf("|");
    for(int i = 0; i < (num_cols * 2) + 1; i++)
    {
        printf("-");
    }
    printf("|\n");
}

void usage(FILE *output)
{
    fprintf(output, "Usage:\nmopsolver [-hdsp] [-i INFILE] [-o OUTFILE]\n");
}

void help(FILE *output)
{
    usage(output);
    fprintf(output, "Options:\n");
    fprintf(output, "  -h          Print usage and options list to stdout ");
    fprintf(output, "only.    (Default: off)\n");
    fprintf(output, "  -d          Pretty-print (display) the maze after ");
    fprintf(output, "reading.  (Default: off)\n");
    fprintf(output, "  -s          Print length of shortest path or 'No ");
    fprintf(output, "solution'. (Default: off)\n");
    fprintf(output, "  -p          Pretty-print maze with the path, if one ");
    fprintf(output, "exists. (Default: off)\n");
    fprintf(output, "  -i infile   Read maze from infile.");
    fprintf(output, "                          (Default: stdin)\n");
    fprintf(output, "  -o outfile  Write all output to outfile.");
    fprintf(output, "                    (Default: stdout)\n");
}

int main(int argc, char *argv[])
{
    (void) argc;
    char *filename = argv[1];
    int num_rows = 0; 
    int num_cols = 0;
    char **maze = make_maze(filename, &num_rows, &num_cols);
/*
    int opt;
    while((opt = getopt(argc, argv, "hdspi:o:")) != -1)
    {
        switch(opt)
        {
        case 'h':
            help();
            return EXIT_FAILURE;
            break;
        }
    }
*/
    help(stdout);
    print_maze(maze, num_rows, num_cols);
    int steps = 0;
    Node *path = solve_BFS(maze, num_rows, num_cols, &steps);

    if(path == NULL)
    {
        printf("No solution");
    }
    else
    {
        printf("%d\n", steps);
        for(int i = 0; i < steps; i++)
        {
            maze[path[i]->row][path[i]->col] = '+';
        }
    }
    print_maze(maze, num_rows, num_cols);

    for(int i = 0; i < num_rows; i++)
    {
        free(maze[i]);
    }
    free(maze);

    return EXIT_SUCCESS;
}







