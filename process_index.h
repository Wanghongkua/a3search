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
extern void compress_print(unsigned int b, FILE *wfp);
