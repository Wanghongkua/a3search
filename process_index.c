#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include "stemmer.h"
#include "word_frequency.h"
#include "process_index.h"

const unsigned int FILE_NAME_SIZE = 256;
const unsigned int BUFFER_SIZE = 4096;
static char *preffix;
static char *slash = "/";
unsigned int num_file;

/*
 *compute file name
 */
void compute_file_name(char *src, char *file_name, char *name)
{
    strcpy(name, src);
    if (strcmp(&src[strlen(src) - 1], slash) == 0) {
        strcat(name, file_name);
    }else{
        strcat(name, slash);
        strcat(name, file_name);
    }
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
    /*unsigned int c_num = *c_char;*/
    /*if (c_num >= 65 && c_num <=97) {*/
        /*return 1;*/
    /*} else if (c_num >= 90 && c_num <= 122) {*/
        /*return 1;*/
    /*}*/
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
        i++;
    }

    qsort(origin_files, num_file, sizeof(char *), a_comp);
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

    append_index(origin_files, argv);

    /*printf("finish append_index\n");*/

    free_origin_files(origin_files);


    delete_stemmer();

    /*merge_index_file(argv, origin_dir, index_file_name);*/

    closedir(origin_dir);
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

    double block = 200;
    unsigned int block_size;
    if (num_file > block) {
        block_size = ceil(num_file / block);
        /*block_size = 1;*/
    } else {
        block_size = 1;
    }

    /*
     *init look up
     */
    preffix = malloc(sizeof(char) * 3);

    for (i = 0; i < num_file + 1; ++i) {

        if (i == num_file) {
            break;
        }
        compute_file_name(argv[1], origin_files[i], origin_file_name);
        printf("%s\n", origin_file_name);
        origin_file = fopen(origin_file_name, "r");

        while ((read_bytes = fread(buffer, 1, BUFFER_SIZE, origin_file))) {
            for (j = 0; j < read_bytes; ++j) {
                *c_char = buffer[j];
                if (isAlpha(c_char)) {
                    strcat(c_word, c_char);
                }else{
                    /* TODO: check if add last word */
                    if (strlen(c_word) >= 3) {
                        get_stem(c_word, c_stem);
                        if (strlen(c_stem) == 0) {
                            printf("no stem\n");
                            exit(0);
                        }
                        /*printf("%s\t%s\n", c_word, c_stem);*/

                        update_wordfrequency(c_stem);
                    }
                    c_word[0] = '\0';
                }
            }
        }
        /* TODO: same to file */
        clean_wordfrequency();

        fclose(origin_file);
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
