#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include "stemmer.h"
#include "word_frequency.h"
#include "process_index.h"
#include "tmp_index.h"
#include "merge_index.h"

const unsigned int FILE_NAME_SIZE = 256;
const unsigned int BUFFER_SIZE = 4096;
const double BLOCK = 200;
static char *preffix;
static char *slash = "/";
unsigned int num_file;

/*
 *compute file name
 */
void compute_file_name(char *src, char *file_name, char *name)
{
    if(src == NULL){
        printf("no memory in compute_file_name for src");
        exit(0);
    }
    if(file_name == NULL){
        printf("no memory in compute_file_name for file_name");
        exit(0);
    }
    /* strcpy(name, src); */
    char c = *file_name;
    memmove(name, src, strlen(src) + 1);
    if (strcmp(&src[strlen(src) - 1], slash) == 0) {
        strcat(name, file_name);
    }else{
        strcat(name, slash);
        strcat(name, file_name);
    }
    /* if( c != *file_name){ */
    /*     printf("haha"); */
    /*     exit(0); */
    /* } */
}

/*
 *check if c_char is english charactor
 */
int isAlpha(char *c_char)
{
    if ( (*c_char >= 'a' && *c_char <= 'z') || (*c_char >= 'A' && *c_char <= 'Z')) {
        return 1;
    }
    return 0;
}

/*
 *compute flag file name
 */
char * compute_flag_file(char *dir)
{
    char *flag_file_name;
    char flag[] = "flag.fuckflag";

    flag_file_name = malloc(sizeof(char) * (strlen(dir) + strlen(flag) + 1));

    compute_file_name(dir, flag, flag_file_name);

    return flag_file_name;
}

/*
 *check if index exist
 */
int exist_index(char *flag_file_name)
{
    if( access( flag_file_name, F_OK ) != -1 ) {
        // file exists
        return 1;
    } else {
        // file doesn't exist
        return 0;
    }

}

/*
 *create or load index file
 */
void process_index(char *argv[])
{
    char *flag_file_name;
    flag_file_name = compute_flag_file(argv[2]);

    if (exist_index(flag_file_name)) {
        /*load_index()*/
        /*create_index(flag_file_name, argv);*/
        /*load_query(argv);*/
    } else {
        create_index(flag_file_name, argv);
        printf("finish create_index\n");
    }

    free(flag_file_name);
}

void sort_origin_files(struct dirent *dp, DIR *origin_dir, char *origin_files[])
{
    unsigned int i = 0;
    /*FILE *origin_file_pointer[num_file];*/

    rewinddir(origin_dir);
    while ((dp = readdir(origin_dir)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
            continue;
        }
        origin_files[i] = malloc(sizeof(char) * (strlen(dp->d_name) + 1));

        if (origin_files[i] == NULL) {
            printf("no memory in sort_origin_files\n");
        }

        strcpy(origin_files[i], dp->d_name);
        /* *(origin_files[i] + strlen(dp->d_name)) = '\0'; */
        i++;
    }

    qsort(origin_files, num_file, sizeof(char *), a_comp);
    /* printf("%s", origin_files[860]); */
}

/*
 *ascending sorting function
 */
int a_comp(const void * elem1, const void * elem2)
{
    const char** aa = (const char**)elem1;
    const char** bb = (const char**)elem2;
    return strcmp(*aa, *bb);
}

void count_file_number(DIR *origin_dir, struct dirent *dp)
{
    num_file = 0;

    while ((dp = readdir(origin_dir)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
            continue;
        }
        num_file++;
    }
}

/*
 *create index files
 */
void create_index(char *flag_file_name, char *argv[])
{
    FILE *index_flag_file;

    /*
     *add flag
     */
    index_flag_file = fopen(flag_file_name, "w");
    fclose(index_flag_file);

    /*
     *open origin dir
     */
    DIR *origin_dir;
    struct dirent *dp;

    origin_dir = opendir(argv[1]);

    FILE *origin_file;

    init_stemmer();

    count_file_number(origin_dir, dp);

    char *origin_files[num_file];
    sort_origin_files(dp, origin_dir, origin_files);

    printf("begin append_index\n");

    append_index(origin_files, argv);

    printf("finish append_index\n");
    /* exit(0); */


    free_origin_files(origin_files);


    delete_stemmer();

    merge_index(argv, num_file);

    printf("finish merge_index\n");

    closedir(origin_dir);
}

void merge_index(char *argv[], unsigned int num_file)
{
    unsigned int num_block = 0;
    unsigned int i;

    char *index_file_name;
    index_file_name = malloc(sizeof(char) * (strlen(argv[2]) + FILE_NAME_SIZE + 2));
    if (index_file_name == NULL) {
        printf("no memory for index_file_name\n");
    }

    if (num_file > BLOCK) {
        num_block = ceil(num_file / BLOCK);
        num_block = ceil(num_file / (double) num_block);
        /*num_block = BLOCK;*/
    } else {
        num_block = num_file;
    }
    char index_block[100];
    FILE *index_files[num_block];
    char *first_str[num_block];

    for (i = 0; i < num_block; ++i) {
        sprintf(index_block, "%d", i);
        compute_file_name(argv[2], index_block, index_file_name);
        index_files[i] = fopen(index_file_name, "rb");
        first_str[i] = malloc(sizeof(char) * 200);
    }
    
    FILE *final_index;
    FILE *final_lookup;
    char *final_index_name;
    char *final_lookup_name;

    final_index_name = malloc(sizeof(char) * (strlen(argv[2]) + FILE_NAME_SIZE + 2));
    final_lookup_name = malloc(sizeof(char) * (strlen(argv[2]) + FILE_NAME_SIZE + 2));

    compute_file_name(argv[2], "final_index.txt", final_index_name);
    compute_file_name(argv[2], "final_lookup.txt", final_lookup_name);
    final_index = fopen(final_index_name, "wb");
    final_lookup = fopen(final_lookup_name, "wb");

    save_to_one(num_block, index_files, first_str, final_index, final_lookup);

    printf("lal\n");
    for (i = 0; i < num_block; ++i) {
        fclose(index_files[i]);
        free(first_str[i]);
    }
    printf("haha\n");
    fclose(final_index);
    fclose(final_lookup);

    free(index_file_name);
    free(final_lookup_name);
    free(final_index_name);

}

void append_index(char *origin_files[], char *argv[])
{
    char *c_char;
    char *c_word;
    char *c_stem;

    char *buffer;
    unsigned int read_bytes = 0;
    unsigned int i, j;

    char *origin_file_name;
    char *index_file_name;

    FILE *origin_file;

    origin_file_name = malloc(sizeof(char) * (strlen(argv[1]) + FILE_NAME_SIZE + 2));
    if (origin_file_name == NULL) {
        printf("no memory for origin_file_name\n");
    }

    index_file_name = malloc(sizeof(char) * (strlen(argv[2]) + FILE_NAME_SIZE + 2));
    if (index_file_name == NULL) {
        printf("no memory for index_file_name\n");
    }

    buffer = malloc(sizeof(char) * BUFFER_SIZE);
    if (buffer == NULL) {
        printf("no memory for buffer\n");
    }

    c_char = malloc(sizeof(char) + 1);
    if (c_char == NULL) {
        printf("no memory for c_char\n");
    }
    c_char[1] = '\0';

    c_word = malloc(sizeof(char) * 1000);
    if (c_word == NULL) {
        printf("no memory for c_word\n");
    }
    c_word[0] = '\0';

    c_stem = malloc(sizeof(char) * 1000);
    if (c_stem == NULL) {
        printf("no memory for c_stem\n");
    }
    c_stem[0] = '\0';

    unsigned int block_size;
    if (num_file > BLOCK) {
        block_size = ceil(num_file / BLOCK);
        /*printf("%d\n", block_size);*/
        /*exit(0);*/
        /*block_size = 1;*/
    } else {
        block_size = 1;
    }

    /*
     *init look up
     */
    preffix = malloc(sizeof(char) * 3);

    char index_block[100];

    sprintf(index_block, "%d", 0);
    compute_file_name(argv[2], index_block, index_file_name);
    change_file_name(index_file_name);

    for (i = 0; i < num_file + 1; ++i) {
        /* printf("\t\t%d\n", i); */
        if (i == num_file) {
            clean_wordfrequency();
            free_filename();
            break;
        }
        if (i % block_size == 0) {
            if (i != 0) {
                clean_wordfrequency();
                free_filename();
                sprintf(index_block, "%d", i / block_size);

                /* printf("%s\n", index_block); */

                compute_file_name(argv[2], index_block, index_file_name);
                change_file_name(index_file_name);
            }
        }
        /* printf("\t\t%s", origin_files[i]); */
        *origin_file_name = '\0';
        compute_file_name(argv[1], origin_files[i], origin_file_name);
        /* printf("\t%s\n", origin_file_name); */
        origin_file = fopen(origin_file_name, "r");
        /* char test[] = "xzgrep.1.txt"; */

        /*
         *change file number in index file
         */
        init_filenumber(i);
        while ((read_bytes = fread(buffer, 1, BUFFER_SIZE, origin_file))) {
            for (j = 0; j < read_bytes; ++j) {
                *c_char = buffer[j];

                /* if(strcmp(test, origin_files[859]) != 0){ */
                /*     printf("haah"); */
                /*     exit(0); */
                /* } */

                if (isAlpha(c_char)) {
                    strcat(c_word, c_char);

                    /* if(strcmp(test, origin_files[859]) != 0){ */
                    /*     printf("papa"); */
                    /*     exit(0); */
                    /* } */

                }else{
                    /* TODO: check if add last word */
                    if (strlen(c_word) >= 3) {
                        /* if(strcmp(test, origin_files[859]) != 0){ */
                        /*     printf("huhu"); */
                        /*     exit(0); */
                        /* } */
                        get_stem(c_word, c_stem);
                        if (strlen(c_stem) == 0) {
                            printf("no stem\n");
                            exit(0);
                        }
                        /*printf("%s\t%s\n", c_word, c_stem);*/
                        /* if(strcmp(test, origin_files[859]) != 0){ */
                        /*     printf("lulu"); */
                        /*     exit(0); */
                        /* } */

                        update_wordfrequency(c_stem);
                    }
                    c_word[0] = '\0';
                }
            }
        }
        change_to_next();
        /* TODO: same to file */
        /*clean_wordfrequency();*/

        fclose(origin_file);
        /* printf("-------\n"); */
    }

    free(preffix);
    free(buffer);
    free(c_char);
    free(c_word);
    free(c_stem);
    free(origin_file_name);
    free(index_file_name);
}

void free_origin_files(char *origin_files[])
{
    unsigned int i;
    for (i = 0; i < num_file; ++i) {
        free(origin_files[i]);
    }
}
