#ifdef __cplusplus
extern "C" {
#endif

    void change_file_name(char *index_file);
    void print_to_file(const char *word, struct posting_list *post);
    void free_filename();
    void print_posting(struct posting_list *post);
    void compress_print(unsigned int number, FILE *index_file);

#ifdef __cplusplus
}
#endif
