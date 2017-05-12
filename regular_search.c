#include <stdio.h>
#include <stdlib.h>
#include "regular_search.h"
#include "sort_query.h"

void regular_search(int argc, char *argv[])
{
    const int STARTING = 3;
    const int N_QUERY = argc - STARTING;

    unsigned int i;
    char *queries[N_QUERY];

    init_query(queries, N_QUERY, argv, STARTING);

    sort_query(queries, N_QUERY);

    free_query(queries, N_QUERY);
}
