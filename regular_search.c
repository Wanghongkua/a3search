#include <stdio.h>
#include <stdlib.h>
#include "regular_search.h"
#include "process_query.h"
#include "process_index.h"
#include "search_index.h"

void regular_search(int argc, char *argv[])
{
    const int STARTING = 3;
    const int N_QUERY = argc - STARTING;

    /*unsigned int i;*/
    char *queries[N_QUERY];
    char *query_stem[N_QUERY];

    /*
     *stem the query
     */
    process_query(queries, query_stem, argv, N_QUERY, STARTING);

    /*
     *create or load the index file
     */
    process_index(argv);

    printf("finish print index\n");
    /*exit(0);*/
    load_index(argv, query_stem, N_QUERY);

    free_query(queries, query_stem, N_QUERY);

}
