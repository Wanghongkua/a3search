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

void load_index(char *argv[], char *query_stem[], const int N_QUERY)
{
    FILE *final_index;
    FILE *final_lookup;
    char *final_index_name;
    char *final_lookup_name;

    long int size;
    int ret = 0;
    int find = 1;

    final_index_name = malloc(sizeof(char) * (strlen(argv[2]) + FILE_NAME_SIZE + 2));
    final_lookup_name = malloc(sizeof(char) * (strlen(argv[2]) + FILE_NAME_SIZE + 2));

    compute_file_name(argv[2], "final_index.txt", final_index_name);
    compute_file_name(argv[2], "final_lookup.txt", final_lookup_name);

    final_index = fopen(final_index_name, "rb");
    final_lookup = fopen(final_lookup_name, "rb");
    fseek(final_lookup, 0L, SEEK_END);
    size = ftell(final_lookup) / 4;
    /*printf("%ld\n", ftell(final_lookup));*/

    /*
     *prepare for finding post in hash table
     */
    init_map(N_QUERY);

    unsigned int i;
    for (i = 0; i < N_QUERY; ++i) {
        /*find_query()*/
        if (find_query(query_stem[i], size, final_lookup, final_index)) {
            ret = change_next();
            if (ret) {
                printf("error in change next \n");
                exit(0);
            }
            extract_post(final_index);

        } else {
            /* TODO: delete this line*/
            printf("\n");
            find = 0;
            /*break;*/
        }
    }
    if (find) {
        search_post(argv);
    }
    fclose(final_index);
    fclose(final_lookup);
    free_map();
}

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

    /*printf("%ld\n", ftell(final_index));*/
    /*fseek(final_index, ftell(final_index) - 1, SEEK_SET);*/
    /**c = getc(final_index);*/
    /*printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY((int)*(c)));*/
    while (1) {
        *c = getc(final_index);
        printf(BYTE_TO_BINARY_PATTERN"\t", BYTE_TO_BINARY((int)*(c)));
        /*printf("%ld\n", ftell(final_index));*/
        if (*c == EOF) {
            break;
        } else if (*c == 0b00000000) {
            /*printf("%u\n", *(unsigned int*)tmp);*/
            strncpy(freq, tmp, sizeof(char) * 4);

            printf("%u\t", *(unsigned int*)filenum);
            printf("%u\n", *(unsigned int*)freq);
            add_to_map(*(unsigned int*)filenum, *(unsigned int*)freq);
            *(unsigned int*)tmp = 0;
            break;
        } else if (*c == 0b00000001) {
            count = 0;
            /*printf("%u\n", *(unsigned int*)tmp);*/
            if (flag == 0) {
                strncpy(filenum, tmp, sizeof(char) * 4);
                flag += 1;
            } else if (flag == 1) {
                strncpy(freq, tmp, sizeof(char) * 4);

                printf("%u\t", *(unsigned int*)filenum);
                printf("%u\n", *(unsigned int*)freq);
                add_to_map(*(unsigned int*)filenum, *(unsigned int*)freq);
                flag = 0;
            }
            *(unsigned int*)tmp = 0;
        } else {
            /*printf("%d\n", count);*/
            /*printf(BYTE_TO_BINARY_PATTERN"\t", BYTE_TO_BINARY((int)*(c)));*/
            *c = *c & 0b01111111;
            /*printf("%u\t", *(unsigned int*)(c));*/
            *(unsigned int*)tmp += *(unsigned int*)c << (7*count);
            /*printf("%u\n", *(unsigned int*)tmp);*/
            /**(unsigned int*)tmp*/
            /**tmp = *c;*/
            /*short int i;*/
            /**(unsigned int*)tmp = 1;*/
            /*for (i = 0; i < 4; ++i) {*/
                /*printf(BYTE_TO_BINARY_PATTERN"\t", BYTE_TO_BINARY((int)*(tmp+i)));*/
            /*}*/
            /*printf("\n");*/

            /**(unsigned int*)tmp += 1;*/
            /*for (i = 0; i < 4; ++i) {*/
                /*printf(BYTE_TO_BINARY_PATTERN"\t", BYTE_TO_BINARY((int)*(tmp+i)));*/
            /*}*/
            /*printf("\n");*/
            /*exit(0);*/
            /*((*c & 0b01111111) << 7) & 0b11111110;*/
            /**(unsigned int*)tmp >>= 7;*/
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

    short int result = 0;
    while (stop - start > 1) {
        /*printf("%d\n", middle);*/
        middle = (stop + start) / 2;
        pos = (middle - 1) * 4;

        find_word(pos, word, final_lookup, final_index);
        result = strcmp(query, word);
        /*printf("%d\t", middle);*/
        /*printf("%s\t%s\t%d\t%ld\n", query, word, result, ftell(final_index));*/

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
        /*printf("%d\t", middle);*/
        /*printf("%s\t%s\t%d\t%ld\n", query, word, result, ftell(final_index));*/
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
        /*fread(c, sizeof(unsigned int), 1, final_lookup);*/
        fread(c, 4, 1, final_lookup);
        pos = *(unsigned int *)c;
        /*for (i = 0; i < 4; ++i) {*/
            /*printf(BYTE_TO_BINARY_PATTERN"\t", BYTE_TO_BINARY((int)*(c+i)));*/
        /*}*/
        /*printf("\n");*/
    }

    *word = '\0';
    /*for (i = 0; i < 200; ++i) {*/
        /**(word + i) = '\0';*/
    /*}*/
    fseek(final_index, pos, SEEK_SET);
    /*printf("%u\t", pos);*/
    /*
     *get the word
     */
    /*printf("\n");*/
    /*printf("\n");*/
    while (1) {
        /*printf("%lu\t", strlen(word));*/
        *c = getc(final_index);
        /*printf(BYTE_TO_BINARY_PATTERN"\t", BYTE_TO_BINARY((int)*(c)));*/
        if (*c == 0b00000000) {
            continue;
        }
        if (*c == 0b00000001) {
            /*following are frequency and file number*/
            break;
        } else {
            strncat(word, c, 1);
        }
    }
    /*printf("\n");*/
    /*printf("%lu\n", strlen(word));*/
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

    /*for (i = 0; i < num_file; ++i) {*/
        /*printf("%s\n", origin_files[i]);*/
    /*}*/

    for (i = 0; i < count; ++i) {
        printf("%d\t%d\t", final_list[i].frequency, final_list[i].filenumber);
        printf("%s\n", origin_files[final_list[i].filenumber]);
    }
    free_origin_files(origin_files);
}

/*void get_filename()*/
/*{*/
/*}*/
