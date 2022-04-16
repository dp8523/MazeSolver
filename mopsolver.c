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

typedef struct cords_s
{
    int row;
    int col;
} cords;

char ** make_maze(char *filename, int *num_rows, int *num_cols)
{
    FILE *infile = fopen(filename, "r");
    char *line = NULL;
    size_t size = 0;
    QueueADT all_elements = que_create(NULL); // queue grows as elements are read in
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

void print_maze(int num_rows, int num_cols, char** maze)
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

int main(int argc, char *argv[])
{
    (void) argc;
    char *filename = argv[1];
    int num_rows = 0; 
    int num_cols = 0;
    char **maze = make_maze(filename, &num_rows, &num_cols);
    print_maze(num_rows, num_cols, maze);

    for(int i = 0; i < num_rows; i++)
    {
        free(maze[i]);
    }
    free(maze);

    return EXIT_SUCCESS;
}







