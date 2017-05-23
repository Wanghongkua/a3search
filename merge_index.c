#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merge_index.h"

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

unsigned int block_num;

void save_to_one(unsigned int num_block, FILE *index_files[], char *first_str[], FILE *final_index, FILE *final_lookup)
{
    /*long int file_size[num_block];*/
    short unsigned int block_mask[num_block];
    block_num = num_block;

    /*find_max_size(index_files, file_size);*/

    init_block_mask(block_mask);

    FILE *final;
    int flag = 0;

    while (! isEmpty(block_mask)) {
        unsigned int i;
        for (i = 0; i < num_block; ++i) {
            if (block_mask[i]) {
                /*printf("%d\n", i);*/
                flag = add_to_first_str(index_files[i], first_str[i]);
                /* TODO: change here */
                if (flag) {
                    block_mask[i] = 0;
                    strcpy(first_str[i], "~");
                    /**first_str[i] = '~';*/
                    /**(first_str[i] + 1) = '\0';*/
                }
            }
        }
        /* for (i = 0; i < num_block; ++i) { */
        /*     printf("%s\t%d\n", first_str[i], block_mask[i]); */
        /* } */
        /* printf("---------\n"); */

        find_smallest(first_str, block_mask);

        /* for (i = 0; i < num_block; ++i) { */
            /* if(block_mask[i]){ */
            /*     printf("%d\t%s\n",strlen(first_str[i]), first_str[i]); */
            /*     break; */
            /* } */
            /* printf("%s\t%d\n", first_str[i], block_mask[i]); */
        /* } */
        /* exit(0); */
        /* printf("---------\n"); */

        save_to_file(first_str, block_mask, index_files, final_index);

        /* printf("**********\n"); */
        /* for (i = 0; i < num_block; ++i) { */
        /*     printf("%s\t%d\n", first_str[i], block_mask[i]); */
        /* } */
        /* printf("---------\n"); */

        save_lookup(ftell(final_index), final_lookup);
    }
}

void save_lookup(int pos, FILE *final_lookup)
{
    short int i;
    char *c;
    c = malloc(sizeof(char) * 2);
    *(c+ 1) = '\0';
    for (i = 0; i < 4; ++i) {
        if(pos != 0){
            *c = pos & 0b11111111;
            fwrite(c, 1, 1, final_lookup);
            pos >>= 8;
        } else {
            *c = 0b00000000;
            fwrite(c, 1, 1, final_lookup);
        }
    }
    free(c);
}

void save_to_file(char *first_str[], short unsigned int block_mask[], FILE *index_files[], FILE *final_index)
{
    short unsigned int i;
    char c;
    /*char null = '\0';*/
    /*short int state;*/

    short int printed = 1;
    short int readbyte = 0;
    for (i = 0; i < block_num; ++i) {
        if (block_mask[i]) {
            if (first_str[i] == NULL) {
                printf("no memory for first_str[i] in save_to_file\n");
                exit(0);
            }

            /*state = 0;*/

            if (printed == 1) {
                c = 0b00000000;
                fwrite(&c, 1, 1, final_index);
                fwrite(first_str[i], 1, strlen(first_str[i]), final_index);
                printed = 0;
            }
            c = 0b00000001;
            fwrite(&c, 1, 1, final_index);

            while (1) {
                /* printf("%d--------%d\n", i, block_num); */
                /* printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(c)); */
                /* c = getc(index_files[i]); */
                readbyte = fread(&c, 1, 1, index_files[i]);
                if(readbyte == 0){
                    break;
                }
                /* printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(c)); */
                /* printf("\n%ld========\n", ftell(index_files[i])); */
                if (c == EOF) {
                    break;
                } else if (c == 0b00000000) {
                    break;
                } else if (c == 0b00000001) {
                    fwrite(&c, 1, 1, final_index);
                } else {
                    fwrite(&c, 1, 1, final_index);
                }
            }
        }
    }
}

void find_smallest(char *first_str[], short unsigned int block_mask[])
{
    char *c = "~";
    short unsigned int i, count = 0, num = 0;
    short unsigned int ret[block_num];

    int result = 0;
    for (i = 0; i < block_num; ++i) {
        if (*first_str[i] == '~') {
            continue;
        }
        result = strcmp(first_str[i], c);
        /*printf("%s\t%s\t%d\n", first_str[i], c, result);*/
        if (result == 0) {
            ret[count] = i;
            count++;
        } else if (result < 0) {
            c = first_str[i];
            count = 0;
            ret[count] = i;
            count++;
        }
    }
    /*printf("+++++++++++\n");*/
    for (i = 0; i < block_num; ++i) {
        if (num == count) {
            block_mask[i] = 0;
        } else if (i == ret[num]) {
            block_mask[i] = 1;
            num++;
        } else {
            block_mask[i] = 0;
        }
        /*printf("%d\n", block_mask[i]);*/
    }
    /*printf("+++++++++++\n");*/
}

int add_to_first_str(FILE *indexfile, char *first_str)
{
    short unsigned int i;
    char *c;
    c = malloc(sizeof(char) * 2);
    *(c + 1) = '\0';
    if (first_str == NULL) {
        printf("no memory in add_to_first_str for first_str\n");
        exit(0);
    }
    /*printf("this is th pos: %ld\n", ftell(indexfile));*/
    /*printf("*\n");*/
    *first_str = '\0';

    while (1) {
        *c = getc(indexfile);
        if (*c == EOF) {
            /*printf("this is eof: %s\n", first_str);*/
            return 1;
        }
        if (*c == 0b00000001) {
            /*printf("%s\t", first_str);*/
            break;
        } else {
            strcat(first_str, c);
        }
    }
    free(c);
    return 0;
}

int isEmpty(short unsigned int block_mask[])
{
    unsigned int i;
    for (i = 0; i < block_num; ++i) {
        if (block_mask[i] == 1) {
            return 0;
        }
    }
    return 1;
}

void init_block_mask(short unsigned int block_mask[])
{
    short unsigned int i;
    for (i = 0; i < block_num; ++i) {
        block_mask[i] = 1;
    }
}

void find_max_size(FILE *index_files[], long int file_size[])
{
    unsigned int i;
    for (i = 0; i < block_num; ++i) {
        fseek(index_files[i], 0L, SEEK_END);
        file_size[i] = ftell(index_files[i]);
        fseek(index_files[i], 1, SEEK_SET);
    }
}


