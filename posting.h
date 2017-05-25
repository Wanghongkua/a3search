#ifdef __cplusplus
#include <map>
struct posting_t {
    std::map<unsigned int, unsigned int> post_list;
    unsigned int count;
    struct posting_t * next;
};

//void sort_posting(std::map<unsigned int, unsigned int> post_list, unsigned int count);
void sort_posting(std::map<unsigned int, unsigned int> post_list, unsigned int count, char *argv[]);
void free_postint_t(struct posting_t *next_post);
#endif

#ifdef __cplusplus
extern "C" {
#endif
    struct posting_s {
        unsigned int filenumber;
        unsigned int frequency;
    };
    void add_to_map(unsigned int filenumber, unsigned int frequency);
    void free_map();
    void init_map(const unsigned int N_QUERY);
    int change_next();
    void search_post(char *argv[]);
#ifdef __cplusplus
}
#endif
