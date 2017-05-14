#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "process_index.h"

const unsigned int FILE_NAME_SIZE = 256;
static char *slash;

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

    free(origin_file_name);
    free(index_file_name);
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
 *compute file name
 */
void compute_file_name(char *src, char file_name[], char *name)
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
 *create or load index file
 */
void process_index(char *argv[])
{
    char *flag_file_name;
    flag_file_name = malloc(sizeof(char) * (strlen(argv[2]) + 6));

    slash = malloc(sizeof(char) * 2);
    slash[0] = '/';
    slash[1] = '\0';

    compute_file_name(argv[2], "flag", flag_file_name);
    printf("%s\n", flag_file_name);

    if (exist_index(flag_file_name)) {
        /*load_index()*/
        /*printf("haha\n");*/
    } else {
        create_index(flag_file_name, argv);
    }

    free(flag_file_name);
    free(slash);
}
