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
unsigned int num_token;
unsigned int num_file;

/*
 *check if c_char is english charactor
 */
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

void sort_origin_files(struct dirent *dp, DIR *origin_dir, char *origin_files[]);

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
        strcpy(origin_files[i], dp->d_name);
        i++;
    }

    qsort(origin_files, num_file, sizeof(char *), a_comp);
}

void free_origin_files(char *origin_files[])
{
    unsigned int i;
    for (i = 0; i < num_file; ++i) {
        free(origin_files[i]);
    }
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

void append_index(char *origin_files[], char *argv[])
{
    char *c_char;
    char *c_word;
    char *c_stem;

    char *buffer;
    unsigned int read_bytes = 0;
    unsigned int i, j;

    struct word_frequency *root;

    char *origin_file_name;
    char *index_file_name;

    FILE *origin_file;

    origin_file_name = malloc(sizeof(char) * (strlen(argv[1]) + FILE_NAME_SIZE + 2));
    index_file_name = malloc(sizeof(char) * (strlen(argv[2]) + FILE_NAME_SIZE + 2));

    buffer = malloc(sizeof(char) * BUFFER_SIZE);

    c_char = malloc(sizeof(char) + 1);
    c_char[1] = '\0';

    c_word = malloc(sizeof(char) * 1000);
    c_word[0] = '\0';

    c_stem = malloc(sizeof(char) * 1000);
    c_stem[0] = '\0';

    for (i = 0; i < num_file; ++i) {
        compute_file_name(argv[1], origin_files[i], origin_file_name);
        origin_file = fopen(origin_file_name, "r");
        root = malloc(sizeof(struct word_frequency));
        root->next = NULL;
        root->previous = NULL;
        root->c_stem = NULL;
        root->frequency = 0;

        while ((read_bytes = fread(buffer, 1, BUFFER_SIZE, origin_file))) {
            for (j = 0; j < read_bytes; ++j) {
                *c_char = buffer[j];
                if (isAlpha(c_char)) {
                    strcat(c_word, c_char);
                }else{
                    /* TODO: check if add last word */
                    if (strlen(c_word) >= 3) {
                        get_stem(c_word, c_stem);
                        /*printf("%s\t%s\n", c_word, c_stem);*/

                        add_to_root(c_stem, &root);
                    }
                    c_word[0] = '\0';
                }
            }
        }
        append_to_file(root, argv[2], index_file_name, i);

        free_word_frequency(root);

        fclose(origin_file);
    }




    free(buffer);
    free(c_char);
    free(c_word);
    free(c_stem);
    free(origin_file_name);
    free(index_file_name);
}
        /*append_to_file(root, argv[2], index_file_name);*/
void append(FILE *index_file, unsigned int file_number, unsigned int frequency);
void append_to_file(struct word_frequency *root, char *dir_name, char *index_file_name, unsigned int file_number)
{
    struct word_frequency *next_word;
    FILE *index_file;

    next_word = root;
    while (next_word->c_stem != NULL) {
        compute_file_name(dir_name, next_word->c_stem, index_file_name);
        index_file = fopen(index_file_name, "ab");
        append(index_file, file_number, next_word->frequency);
        fclose(index_file);

        next_word = next_word->next;
        if (next_word == NULL) {
            break;
        }
    }
}

void append(FILE *index_file, unsigned int file_number, unsigned int frequency)
{
    /* TODO: do the print */
    compress_print(file_number, index_file, 1);
    compress_print(frequency, index_file, 0);
}


/*
 *create index files
 */
void create_index(char *flag_file_name, char *argv[])
{
    FILE *index_flag_file;

    /*char *origin_file_name;*/
    /*char *index_file_name;*/

    /*origin_file_name = malloc(sizeof(char) * (strlen(argv[1]) + FILE_NAME_SIZE + 2));*/
    /*index_file_name = malloc(sizeof(char) * (strlen(argv[2]) + FILE_NAME_SIZE + 2));*/

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

    /*char *c_char;*/
    /*char *c_word;*/
    /*char *c_stem;*/

    /*char *buffer;*/
    /*unsigned int read_bytes = 0;*/
    /*unsigned int i;*/

    /*struct word_frequency *root;*/

    /*buffer = malloc(sizeof(char) * BUFFER_SIZE);*/

    /*c_char = malloc(sizeof(char) + 1);*/
    /*c_char[1] = '\0';*/

    /*c_word = malloc(sizeof(char) * 1000);*/
    /*c_word[0] = '\0';*/

    /*c_stem = malloc(sizeof(char) * 1000);*/
    /*c_stem[0] = '\0';*/

    init_stemmer();

    count_file_number(origin_dir, dp);

    char *origin_files[num_file];
    sort_origin_files(dp, origin_dir, origin_files);

    append_index(origin_files, argv);

    
    free_origin_files(origin_files);

    /*rewinddir(origin_dir);*/
    /*while ((dp = readdir(origin_dir)) != NULL) {*/
        /*if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {*/
            /*continue;*/
        /*}*/
        /*num_file += 1;*/

        /*compute_file_name(argv[1], dp->d_name, origin_file_name);*/

        /*root = malloc(sizeof(struct word_frequency));*/
        /*root->next = NULL;*/
        /*root->previous = NULL;*/
        /*root->c_stem = NULL;*/
        /*root->frequency = 0;*/

        /*num_token = 0;*/

        /*origin_file = fopen(origin_file_name, "r");*/

        /*while ((read_bytes = fread(buffer, 1, BUFFER_SIZE, origin_file))) {*/
            /*for (i = 0; i < read_bytes; ++i) {*/
                /**c_char = buffer[i];*/
                /*if (isAlpha(c_char)) {*/
                    /*strcat(c_word, c_char);*/
                /*}else{*/
                    /*[> TODO: check if add last word <]*/
                    /*if (strlen(c_word) >= 3) {*/
                        /*get_stem(c_word, c_stem);*/

                        /*add_to_root(c_stem, &root);*/
                        /*[>c_stem[0] = '\0';<]*/
                    /*}*/
                    /*c_word[0] = '\0';*/
                /*}*/
            /*}*/
        /*}*/
        /*save_to_file(root, argv[2], dp->d_name, index_file_name);*/

        /*free_word_frequency(root);*/
        /*fclose(origin_file);*/
    /*}*/

    /*free(dp);*/

    delete_stemmer();

    /*merge_index_file(argv, origin_dir, index_file_name);*/

    closedir(origin_dir);

    /*free(buffer);*/
    /*free(c_char);*/
    /*free(c_word);*/
    /*free(c_stem);*/
    /*free(origin_file_name);*/
    /*free(index_file_name);*/
}

void merge_index_file(char *argv[], DIR *origin_dir, char *index_file_name)
{
    struct dirent *dp;

    char *origin_files[num_file];
    FILE *origin_file_pointer[num_file];
    unsigned int i = 0;

    char *index_name;
    char index[] = "index.fuckindex";

    int stem_name_bitmap[num_file];

    rewinddir(origin_dir);
    while ((dp = readdir(origin_dir)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
            continue;
        }
        origin_files[i] = malloc(sizeof(char) * (strlen(dp->d_name) + 1));
        strcpy(origin_files[i], dp->d_name);
        i++;
    }

    qsort(origin_files, num_file, sizeof(char *), a_comp);

    for (i = 0; i < num_file; ++i) {
        compute_file_name(argv[2], origin_files[i], index_file_name);
        /*printf("%s\n", index_file_name);*/
        origin_file_pointer[i] = fopen(index_file_name, "rb");
        stem_name_bitmap[i] = 1;
    }

    compute_file_name(argv[2], index, index_name);

    /* TODO: merge the file */
    print_to_index_file(origin_file_pointer, stem_name_bitmap, index_name);
    /*finial_step()*/

    for (i = 0; i < num_file; ++i) {
        free(origin_files[i]);
        fclose(origin_file_pointer[i]);
    }
    free(dp);
}

void print_to_index_file(FILE *origin_file_pointer[], int stem_name_bitmap[], char *index_name)
{
    FILE *final_index;
    char *stem_name[num_file];
    unsigned int i;

    final_index = fopen(index_name, "wb");
    for (i = 0; i < num_file; ++i) {
        /*stem_length;*/
        extract_stem(stem_name[i], origin_file_pointer[i]);
    }

}

void extract_stem(char *stem, FILE *file)
{
    unsigned int stem_length = 20;
    
}

/*
 *ascending sorting function
 */
int a_comp (const void * elem1, const void * elem2)
{
    const char** aa = (const char**)elem1;
    const char** bb = (const char**)elem2;
    return strcmp(*aa, *bb);
}


/*
 *save sorted_frequency to index file
 */
void save_to_file(struct word_frequency *root, char *dir_name, char *file_name, char *index_file_name)
{
    struct word_frequency *sorted_frequency[num_token];
    unsigned int i;

    init_sorted_frequency(sorted_frequency, root);
    compute_file_name(dir_name, file_name, index_file_name);

    FILE *index_file;
    index_file = fopen(index_file_name, "wb");

    for (i = 0; i < num_token; ++i) {
        fwrite(sorted_frequency[i]->c_stem, strlen(sorted_frequency[i]->c_stem), 1, index_file);
        /*write frequency*/
        compress_print(sorted_frequency[i]->frequency - 1, index_file, 1);
    }
    fclose(index_file);
}

/*
 *print frequency
 */
void compress_print(unsigned int b, FILE *wfp, int flag)
{
    char c;
    if (b == 0) {
        if (flag == 1) {
            c = 0b10000000;
        } else if (flag == 0) {
            c = 0b00000000;
        }
        fwrite(&c, 1, 1, wfp);
    }
    while (b != 0) {
        if (flag == 1) {
            c = (b & 0b01111111) | 0b10000000;
        } else if (flag == 0) {
            c = (b & 0b01111111) | 0b00000000;
        }
        fwrite(&c, 1, 1, wfp);
        b >>= 7;
    }
}

/*
 *init sorted_frequency
 */
void init_sorted_frequency(struct word_frequency *sorted_frequency[], struct word_frequency *root)
{
    unsigned int i = 0;
    struct word_frequency *next_word;
    next_word = root;

    while (next_word != NULL) {
        sorted_frequency[i] = next_word;
        next_word = next_word->next;
        i++;
    }
}

/*
 *free word frequency linked list
 */
void free_word_frequency(struct word_frequency *root)
{
    /*if (root->c_stem != NULL) {*/
        /*printf("%s\n", root->c_stem);*/
    /*}*/
    if (root->next != NULL) {
        free_word_frequency(root->next);
    }
    free(root);
}

/*
 *add token to word frequency
 */
void add_to_root(char *c_stem, struct word_frequency **root)
{
    struct word_frequency *next_word;
    struct word_frequency *new_word;

    int current = 0;

    next_word = *root;
    while (1) {
        if (next_word->c_stem == NULL) {

            next_word->c_stem = malloc(sizeof(char) * (strlen(c_stem) + 1));
            strcpy(next_word->c_stem, c_stem);

            next_word->frequency = 1;
            next_word->next = NULL;
            next_word->previous = NULL;

            num_token += 1;
            break;
        }else{
            current = strcmp(c_stem, next_word->c_stem);
            if (current == 0) {
                next_word->frequency += 1;
                break;
            } else if (current < 0) {
                new_word = malloc(sizeof(struct word_frequency));
                new_word->c_stem = malloc(sizeof(char) * (strlen(c_stem) + 1));
                new_word->frequency = 1;
                strcpy(new_word->c_stem, c_stem);

                /*swap*/
                new_word->next = next_word;

                new_word->previous = next_word->previous;
                if (next_word->previous != NULL) {
                    next_word->previous->next = new_word;
                } else {
                    *root = new_word;
                }
                next_word->previous = new_word;

                num_token += 1;

                break;

            } else if (current > 0) {
                if (next_word->next == NULL) {
                    /*swap*/
                    new_word = malloc(sizeof(struct word_frequency));
                    new_word->c_stem = malloc(sizeof(char) * (strlen(c_stem) + 1));
                    new_word->frequency = 1;
                    strcpy(new_word->c_stem, c_stem);

                    new_word->previous = next_word;
                    new_word->next = NULL;

                    next_word->next = new_word;

                    num_token += 1;
                    break;
                }
                next_word = next_word->next;
            }

            /*if*/
        }
        /*if (strcmp(next_word->c_stem, c_stem) == 0) {*/
            /*break;*/
        /*}*/
        /*if (next_word->next == NULL) {*/
            /*next_word->next = malloc(sizeof(struct word_frequency));*/
            /*next_word = next_word->next;*/

            /*next_word->next = NULL;*/
            /*next_word->frequency = 0;*/
            /*next_word->c_stem = malloc(sizeof(char) * (strlen(c_stem + 1)));*/
            /*strcpy(next_word->c_stem, c_stem);*/

            /*num_token += 1;*/
            /*break;*/
        /*}*/
        /*next_word = next_word->next;*/
    }
    /*next_word->frequency += 1;*/
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
    char flag[] = "flag.fuckflag";

    flag_file_name = malloc(sizeof(char) * (strlen(dir) + strlen(flag) + 1));

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

    if (exist_index(flag_file_name)) {
        /*load_index()*/
        /*create_index(flag_file_name, argv);*/
        /*load_query(argv);*/
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
