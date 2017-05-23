#ifdef __cplusplus
extern "C" {
#endif
    struct posting_list {
        unsigned int file_number;
        unsigned int frequency;
        unsigned int used;

        struct posting_list *next;
    };

    void update_wordfrequency(char *word);
    void clean_wordfrequency();
    void change_to_next();
    void init_filenumber(unsigned int f_num);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
//struct posting_list {
//unsigned int file_number;
//unsigned int frequency;
//unsigned int used;

//struct posting_list *next;
//};
struct word_element {
    struct posting_list *first_posting;
    struct posting_list *last_posting;
};
void free_posting(struct posting_list *root);
#endif
