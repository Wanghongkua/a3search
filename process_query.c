#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process_query.h"
#include "stemmer.h"
#include "libstemmer_c/include/libstemmer.h"

int comp (const void * elem1, const void * elem2);

int comp (const void * elem1, const void * elem2) 
{
    const char** aa = (const char**)elem1;
    const char** bb = (const char**)elem2;
    return strcmp(*bb, *aa);
}

void init_query(char *queries[], const int N_QUERY, char *argv[],
        const int STARTING, char *query_stem[])
{
    unsigned int i;

    for (i = 0; i < N_QUERY; ++i) {
        queries[i] = malloc(sizeof(char) * (strlen(argv[STARTING + i]) + 1));
        strcpy(queries[i], argv[STARTING + i]);
        query_stem[i] = malloc(sizeof(char) * (strlen((char *)queries[i]) + 1));
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
        free(queries[i]);
        free(query_stem[i]);
    }
}

void stem_query(char *queries[], char *query_stem[], const int N_QUERY)
{
    init_stemmer();

    unsigned int i;
    for (i = 0; i < N_QUERY; ++i) {
        get_stem(queries[i], query_stem[i]);
    }

    delete_stemmer();
}

void process_query(char *queries[], char *query_stem[], char *argv[],
        const int N_QUERY, const int STARTING)
{
    init_query(queries, N_QUERY, argv, STARTING, query_stem);

    sort_query(queries, N_QUERY);

    stem_query(queries, query_stem, N_QUERY);
}
