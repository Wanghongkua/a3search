#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "word_frequency.h"
#include "tmp_index.h"
FILE *index_file;

void change_file_name(char *index_file_name)
{
    if (index_file_name == NULL) {
        printf("no memory for index_file_name in free_filename\n");
        exit(0);
    }
    index_file = fopen(index_file_name, "wb");
}
void print_to_file(const char *word, struct posting_list *first_posting)
{
    char null = 0b00000000;

    if (word == NULL) {
        printf("no memory for word in print_to_file\n");
        exit(0);
    }
    fwrite(&null, 1, 1, index_file);

    fwrite(word, 1, strlen(word), index_file);

    if (first_posting == NULL) {
        printf("no memory for first_posting in print_to_file\n");
        exit(0);
    }
    print_posting(first_posting);
}

void print_posting(struct posting_list *post)
{
    if (post == NULL) {
        return;
    } else if (post->used == 0) {
        return;
    }

    char c;
    unsigned int number = 0;

    c = 0b00000001;
    fwrite(&c, 1, 1, index_file);

    number = post->file_number;
    compress_print(number, index_file);

    c = 0b00000001;
    fwrite(&c, 1, 1, index_file);

    number = post->frequency;
    compress_print(number, index_file);

    /*while (number != 0) {*/
        /*c = number & 0b11111111;*/
        /*fwrite(&c, 1, 1, index_file);*/
        /*number >>= 8;*/
    /*}*/
    if (post->next != NULL) {
        print_posting(post->next);
    }
}

void compress_print(unsigned int number, FILE *index_file)
{
    char c;

    if (number == 0) {
        c = 0b10000000;
        fwrite(&c, 1, 1, index_file);
    }

    while (number != 0) {
        c = (number & 0b01111111) | (0b10000000);
        fwrite(&c, 1, 1, index_file);
        number >>= 7;
    }
    
}

void free_filename()
{
    fclose(index_file);
}
