#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  /* for isupper, tolower */
#include "libstemmer_c/include/libstemmer.h"

struct sb_stemmer * stemmer;
sb_symbol * b;

#define INC 10
int lim = INC;

/*
 *create stemmer
 */
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

/*
 *free up stemmer
 */
void delete_stemmer()
{
    sb_stemmer_delete(stemmer);
    free(b);
}

/*
 *get stemmed word
 */
void get_stem(char *src, char *dest)
{
    int i = 0;
    while(1) {
        if (i == strlen(src)) {
            const sb_symbol * stemmed = sb_stemmer_stem(stemmer, b, i);

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
}
