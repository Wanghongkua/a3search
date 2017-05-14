#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process_query.h"
#include "stemmer.h"

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
        queries[i] = malloc(sizeof(char) * (strlen(argv[STARTING + i]) + 1));
        strcpy(queries[i], argv[STARTING + i]);
    }
}

void sort_query(char *queries[], const int N_QUERY)
{
    qsort(queries, N_QUERY, sizeof(char *), comp);
}

void free_query(char *queries[], char *query_stem[], const int N_QUERY)
{
    unsigned int i;
    for (i = 0; i < N_QUERY; ++i) {
        /*printf("%s\n", queries[i]);*/
        free(queries[i]);
        free(query_stem[i]);
    }
}

void process_query(char *queries[], char *query_stem[], char *argv[],
        const int N_QUERY, const int STARTING)
{
    init_query(queries, N_QUERY, argv, STARTING);
    sort_query(queries, N_QUERY);

    get_stem(queries, query_stem, N_QUERY);
}
