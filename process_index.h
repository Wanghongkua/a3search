#include <dirent.h>
extern void process_index(char *argv[]);
extern void sort_origin_files(struct dirent *dp, DIR *origin_dir, char *origin_files[]);
extern void count_file_number(DIR *origin_dir, struct dirent *dp);
//extern void create_index(char *flag_file_name, char *argv[]);
extern void append_index(char *origin_files[], char *argv[]);
extern void free_origin_files(char *origin_files[]);
extern void create_index(char *argv[]);

extern void compute_file_name(char *src, char *file_name, char *name);
extern int isAlpha(char *c_char);
extern char * compute_flag_file(char *dir);
extern int exist_index(char *flag_file_name);
extern int a_comp(const void * elem1, const void * elem2);
extern void merge_index(char *argv[], unsigned int num_file);
extern const unsigned int FILE_NAME_SIZE;
