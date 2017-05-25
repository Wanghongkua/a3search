#include "posting.h"
extern void load_index(char *argv[], char *query_stem[], const int N_QUERY);
extern void find_word(long int middle, char *word, FILE *final_lookup, FILE *final_index);
extern int find_query(char *query, long int size, FILE *final_lookup, FILE *final_index);
extern void extract_post(FILE *final_index);
//extern int cmp_fun(const void *el1, const void *el2);
//extern int pre_cmp_fun(const void *el1, const void *el2);

#ifdef __cplusplus
extern "C" {
#endif
    void final_print(struct posting_s final_list[], unsigned int query_num, char *argv[]);
#ifdef __cplusplus
}
#endif
