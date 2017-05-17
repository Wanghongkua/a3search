#include <dirent.h>
extern void process_index(char *argv[]);
extern void compute_file_name(char *src, char file_name[], char *name);
extern int exist_index(char *flag_file_name);
extern void create_index(char *flag_file_name, char *argv[]);
extern char * compute_flag_file(char *dir);
extern int isAlpha(char *c_char);

struct word_frequency {
    char *c_stem;
    unsigned int frequency;
    struct word_frequency *next;
    struct word_frequency *previous;
};

extern void add_to_root(char *c_stem, struct word_frequency **root);
extern void free_word_frequency(struct word_frequency *root);
extern void init_sorted_frequency(struct word_frequency *sorted_frequency[], struct word_frequency *root);
extern void save_to_file(struct word_frequency *root, char *dir_name, char *file_name, char *index_file_name);
extern void merge_index_file(char *argv[], DIR *origin_dir, char *index_file_name);
extern int a_comp (const void * elem1, const void * elem2);
extern void print_to_index_file(FILE *origin_file_pointer[], int stem_name_bitmap[], char *index_name);
extern void extract_stem(char *stem, FILE *file);
extern void append_to_file(struct word_frequency *root, char *dir_name, char *index_file_name, unsigned int i);
extern void compress_print(unsigned int b, FILE *wfp, int flag);
extern void count_file_number(DIR *origin_dir, struct dirent *dp);
extern void free_origin_files(char *origin_files[]);
extern void append_index(char *origin_files[], char *argv[]);
