#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sort_query.h"

int comp (const void * elem1, const void * elem2);

int comp (const void * elem1, const void * elem2) 
{
    const char** aa = (const char**)elem1;
    const char** bb = (const char**)elem2;
    return strcmp(*bb, *aa);
}

void init_query(char *queries[], const int N_QUERY, char *argv[], const int STARTING)
{
    unsigned int i;

    for (i = 0; i < N_QUERY; ++i) {
        queries[i] = malloc(sizeof(char) * (256 + 1));
        strcpy(queries[i], argv[STARTING + i]);
    }
}

void sort_query(char *queries[], const int N_QUERY)
{
    qsort(queries, N_QUERY, sizeof(char *), comp);
}

void free_query(char *queries[], const int N_QUERY)
{
    unsigned int i;
    for (i = 0; i < N_QUERY; ++i) {
        free(queries[i]);
    }
}
