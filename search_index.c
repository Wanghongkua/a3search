#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process_index.h"
#include "search_index.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

extern unsigned int num_file;

/*
 *load index file and search for each query
 */
void load_index(char *argv[], char *query_stem[], const int N_QUERY)
{
    FILE *final_index;
    FILE *final_lookup;
    char *final_index_name;
    char *final_lookup_name;

    long int size;
    int ret = 0;
    int find = 1;

    /*
     *open index file and index lookup file
     */
    final_index_name = malloc(sizeof(char) * (strlen(argv[2]) + FILE_NAME_SIZE + 2));
    final_lookup_name = malloc(sizeof(char) * (strlen(argv[2]) + FILE_NAME_SIZE + 2));

    compute_file_name(argv[2], "final_index.txt", final_index_name);
    compute_file_name(argv[2], "final_lookup.txt", final_lookup_name);

    final_index = fopen(final_index_name, "rb");
    final_lookup = fopen(final_lookup_name, "rb");
    fseek(final_lookup, 0L, SEEK_END);
    size = ftell(final_lookup) / 4;

    /*
     *prepare for finding post in hash table
     */
    init_map(N_QUERY);

    unsigned int i;
    for (i = 0; i < N_QUERY; ++i) {
        if (find_query(query_stem[i], size, final_lookup, final_index)) {
            ret = change_next();
            if (ret) {
                printf("error in change next \n");
                exit(0);
            }
            extract_post(final_index);

        } else {
            /*printf("\n");*/
            find = 0;
        }
    }
    /*
     *if find all query then do the search
     */
    if (find) {
        search_post(argv);
    }
    fclose(final_index);
    fclose(final_lookup);

    /*
     *free hash table
     */
    free_map();
}

/*
 *extract the filenumber and frequency from index file
 */
void extract_post(FILE *final_index)
{
    char *c;
    char *filenum;
    char *freq;
    char *tmp;
    short int flag = 0;
    short int count = 0;

    c = malloc(sizeof(char) * 5);
    *(unsigned int*)c = 0;

    filenum = malloc(sizeof(char) * 5);
    *(filenum+4) = '\0';
    freq = malloc(sizeof(char) * 5);
    *(freq+4) = '\0';
    tmp = malloc(sizeof(char) * 5);
    *(tmp+4) = '\0';
    *(unsigned int*)tmp = 0;

    while (1) {
        *c = getc(final_index);
        /*printf(BYTE_TO_BINARY_PATTERN"\t", BYTE_TO_BINARY((int)*(c)));*/

        if (*c == EOF) {
            break;
        } else if (*c == 0b00000000) {
            strncpy(freq, tmp, sizeof(char) * 4);

            add_to_map(*(unsigned int*)filenum, *(unsigned int*)freq);

            *(unsigned int*)tmp = 0;
            break;
        } else if (*c == 0b00000001) {
            count = 0;
            if (flag == 0) {
                strncpy(filenum, tmp, sizeof(char) * 4);
                flag += 1;
            } else if (flag == 1) {
                strncpy(freq, tmp, sizeof(char) * 4);

                add_to_map(*(unsigned int*)filenum, *(unsigned int*)freq);
                flag = 0;
            }
            *(unsigned int*)tmp = 0;
        } else {
            *c = *c & 0b00111111;

            *(unsigned int*)tmp += *(unsigned int*)c << (6*count);

            count += 1;
        }
    }

    free(c);
    free(filenum);
    free(freq);
}


/*
 * find the posting list, return 0 if not found
 */
int find_query(char *query, long int size, FILE *final_lookup, FILE *final_index)
{
    unsigned int start;
    unsigned int middle;
    unsigned int stop;
    char *word;
    long int pos = 0;
    word = malloc(sizeof(char) * 200);

    start = 0;
    stop = size;

    /*
     *perform binary search on lookup file
     */
    short int result = 0;
    while (stop - start > 1) {
        middle = (stop + start) / 2;
        pos = (middle - 1) * 4;

        find_word(pos, word, final_lookup, final_index);
        result = strcmp(query, word);

        if (result == 0) {
            break;
        } else if (result < 0) {
            stop = middle;
        } else if (result > 0) {
            start = middle;
        }
    }
    if (result != 0 && start == 0) {
        middle = (stop + start) / 2;
        pos = (middle - 1) * 4;
        find_word(pos, word, final_lookup, final_index);
        result = strcmp(query, word);
    }
    if (result == 0) {
        return 1;
    } else {
        free(word);
        return 0;
    }
}

/*
 *find word base on lookup table
 */
void find_word(long int lookup_pos, char *word, FILE *final_lookup, FILE *final_index)
{
    char *c;
    unsigned int pos = 0;
    short int i;

    c = malloc(sizeof(char) * 5);
    *(c+4) = '\0';

    /*
     *find the position in index_file
     */
    if (lookup_pos < 0) {
        pos = 1;
    } else {
        fseek(final_lookup, lookup_pos, SEEK_SET);
        fread(c, 4, 1, final_lookup);
        pos = *(unsigned int *)c;
    }

    *word = '\0';
    fseek(final_index, pos, SEEK_SET);

    /*
     *get the word
     */
    while (1) {
        *c = getc(final_index);
        if (*c == 0b00000000) {
            continue;
        }
        if (*c == 0b00000001) {
            break;
        } else {
            strncat(word, c, 1);
        }
    }
    free(c);
}

void final_print(struct posting_s final_list[], unsigned int count, char *argv[])
{
    unsigned int i;

    DIR *origin_dir;
    struct dirent *dp;

    origin_dir = opendir(argv[1]);

    FILE *origin_file;

    count_file_number(origin_dir, dp);

    char *origin_files[num_file];
    sort_origin_files(dp, origin_dir, origin_files);

    for (i = 0; i < count; ++i) {
        printf("%s\n", origin_files[final_list[i].filenumber]);
    }
    free_origin_files(origin_files);
}

/*void get_filename()*/
/*{*/
/*}*/
