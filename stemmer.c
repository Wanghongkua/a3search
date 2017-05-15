#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  /* for isupper, tolower */
#include "libstemmer_c/include/libstemmer.h"

struct sb_stemmer * stemmer;
sb_symbol * b;

void init_stemmer()
{
    char *language = "english";
    char *charenc = NULL;

    stemmer = sb_stemmer_new(language, charenc);

    #define INC 10
    int lim = INC;

    b = (sb_symbol *) malloc(lim * sizeof(sb_symbol));

}

void delete_stemmer()
{
    sb_stemmer_delete(stemmer);
    free(b);
}

void get_stem(char *src, char *dest)
{
    int j;
    strcpy((char *)b, src);
    for (j = 0; j < strlen((char *)b); ++j) {
        b[j] = tolower(b[j]);
    }
    j = strlen(src);
    const sb_symbol * stemmed = sb_stemmer_stem(stemmer, b, j);
    if (stemmed == NULL) {
        fprintf(stderr, "Out of memory");
        exit(1);
    }
    else {
        strcpy(dest, (char *)stemmed);
    }
}

/*void get_stem(char *queries[], char *query_stem[], const int N_QUERY)*/
/*{*/
    /*struct sb_stemmer * stemmer;*/

    /*char *language = "english";*/
    /*char *charenc = NULL;*/

    /*stemmer = sb_stemmer_new(language, charenc);*/

    /*unsigned int i;*/

    /*#define INC 10*/
    /*int lim = INC;*/
    /*sb_symbol * b = (sb_symbol *) malloc(lim * sizeof(sb_symbol));*/

    /*for (i = 0; i < N_QUERY; ++i) {*/

        /*int j;*/
        /*strcpy((char *)b, queries[i]);*/
        /*j = strlen(queries[i]);*/
        /*const sb_symbol * stemmed = sb_stemmer_stem(stemmer, b, j);*/
        /*if (stemmed == NULL) {*/
            /*fprintf(stderr, "Out of memory");*/
            /*exit(1);*/
        /*}*/
        /*else {*/
            /*query_stem[i] = malloc(sizeof(char) * (strlen((char *)stemmed) + 1));*/
            /*strcpy(query_stem[i], (char *)stemmed);*/
        /*}*/
    /*}*/

    /*free(b);*/
    /*sb_stemmer_delete(stemmer);*/
/*}*/
