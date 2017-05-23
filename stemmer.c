#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  /* for isupper, tolower */
#include "libstemmer_c/include/libstemmer.h"

struct sb_stemmer * stemmer;
sb_symbol * b;

#define INC 10
int lim = INC;

void init_stemmer()
{
    char *language = "english";
    char *charenc = NULL;

    stemmer = sb_stemmer_new(language, charenc);

    b = (sb_symbol *) malloc(lim * sizeof(sb_symbol));
    if (b == NULL) {
        printf("terminate in init_stemmer\n");
    }
}

void delete_stemmer()
{
    sb_stemmer_delete(stemmer);
    free(b);
}

void get_stem(char *src, char *dest)
{
    int i = 0;
    while(1) {
        if (i == strlen(src)) {
            const sb_symbol * stemmed = sb_stemmer_stem(stemmer, b, i);
            /*free(b);*/
            /* dest = (char *) stemmed; */
            memmove(dest, (char *)stemmed, strlen((char *)stemmed) + 1);
            return;
        }
        int ch = src[i];
        if (i == lim) {
            sb_symbol * newb;
            newb = (sb_symbol *) realloc(b, (lim + INC) * sizeof(sb_symbol));
            b = newb;
            lim = lim + INC;
        }
        /* force lower case: */
        if (isupper(ch)) ch = tolower(ch);
        b[i] = ch;
        i++;
    }
    /* int j; */
    /* strcpy((char *)b, src); */
    /* memmove((char *)b, src, strlen(src) + 1); */
    /* for (j = 0; j < strlen((char *)b); ++j) { */
    /*     b[j] = tolower(b[j]); */
    /* } */
    /* j = strlen(src); */
    /* const sb_symbol * stemmed = sb_stemmer_stem(stemmer, b, j); */

    /* if (stemmed == NULL) { */
    /*     fprintf(stderr, "Out of memory"); */
    /*     exit(1); */
    /* } */
    /* else { */
    /*     strcpy(dest, (char *)stemmed); */
    /*     memmove(dest, (char *)stemmed, strlen((char *)stemmed) + 1); */
    /* } */
}
    /* sb_symbol * b = (sb_symbol *) malloc(lim * sizeof(sb_symbol)); */


