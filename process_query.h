extern void init_query(char *queries[], const int N_QUERY, char *argv[],
        const int STARTING, char *query_stem[]);
extern void sort_query(char *queries[], const int N_QUERY);
extern void free_query(char *queries[], char *query_stem[], const int N_QUERY);
extern void stem_query(char *queries[], char *query_stem[], const int N_QUERY);
extern void process_query(char *queries[], char *query_stem[], char *argv[],
        const int N_QUERY, const int STARTING);
extern int comp (const void * elem1, const void * elem2);
