#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include "process_index.h"
#include "stemmer.h"

const unsigned int FILE_NAME_SIZE = 256;
const unsigned int BUFFER_SIZE = 4096;
static char *slash = "/";

int isAlpha(char *c_char)
{
    unsigned int c_num = *c_char;
    if (c_num >= 65 && c_num <=97) {
        return 1;
    } else if (c_num >= 90 && c_num <= 122) {
        return 1;
    }
    return 0;
}

/*
 *create index files
 */
void create_index(char *flag_file_name, char *argv[])
{
    FILE *index_flag_file;

    char *origin_file_name;
    char *index_file_name;

    origin_file_name = malloc(sizeof(char) * (strlen(argv[1]) + FILE_NAME_SIZE + 2));
    index_file_name = malloc(sizeof(char) * (strlen(argv[2]) + FILE_NAME_SIZE + 2));

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

    FILE *origin_file;

    char *buffer;
    char *c_char;
    char *c_word;
    char *c_stem;
    unsigned int read_bytes = 0;
    unsigned int i;

    buffer = malloc(sizeof(char) * BUFFER_SIZE);

    c_char = malloc(sizeof(char) + 1);
    c_char[1] = '\0';

    c_word = malloc(sizeof(char) * 1000);
    c_word[0] = '\0';

    c_stem = malloc(sizeof(char) * 1000);
    c_stem[0] = '\0';

    init_stemmer();
    origin_dir = opendir(argv[1]);
    while ((dp = readdir(origin_dir)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
            continue;
        }
        compute_file_name(argv[1], dp->d_name, origin_file_name);
        /*printf("%s\n", origin_file_name);*/
        origin_file = fopen(origin_file_name, "r");
        while ((read_bytes = fread(buffer, 1, BUFFER_SIZE, origin_file))) {
            for (i = 0; i < read_bytes; ++i) {
                *c_char = buffer[i];
                if (isAlpha(c_char)) {
                    strcat(c_word, c_char);
                }else{
                    if (strlen(c_word) >= 3) {
                        get_stem(c_word, c_stem);
                        printf("%s\t\t%s\n", c_word, c_stem);
                        /* TODO: append_index */
                        /*append_index(c_stem);*/
                        /*c_stem[0] = '\0';*/
                    }
                    c_word[0] = '\0';
                }
            }
        }
        fclose(origin_file);
    }

    delete_stemmer();

    closedir(origin_dir);

    free(buffer);
    free(c_char);
    free(c_word);
    free(c_stem);
    free(origin_file_name);
    free(index_file_name);
}

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
 *compute flag file name
 */
char * compute_flag_file(char *dir)
{
    char *flag_file_name;
    flag_file_name = malloc(sizeof(char) * (strlen(dir) + 6));

    char flag[] = "flag";

    compute_file_name(dir, flag, flag_file_name);

    return flag_file_name;
}

/*
 *create or load index file
 */
void process_index(char *argv[])
{
    char *flag_file_name;
    flag_file_name = compute_flag_file(argv[2]);
    /*printf("%s\n", flag_file_name);*/

    if (exist_index(flag_file_name)) {
        /*load_index()*/
        create_index(flag_file_name, argv);
    } else {
        create_index(flag_file_name, argv);
    }

    free(flag_file_name);
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
