#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include "search_index.h"


using namespace std;

struct posting_t *root = NULL;
struct posting_t *current = NULL;
//struct posting_t *previous = NULL;
unsigned int query_num = 0;

void init_map(const unsigned int N_QUERY)
{
    unsigned int i;
    query_num = N_QUERY;

    root = new struct posting_t;
    root->next = NULL;
    root->count = 0;

    struct posting_t *next_post;
    next_post = root;

    for (i = 0; i < N_QUERY - 1; ++i) {
        next_post->next = new struct posting_t;
        next_post->next->next = NULL;
        next_post->next->count = 0;
        next_post = next_post->next;
    }
}

void add_to_map(unsigned int filenumber, unsigned int frequency)
{
    current->post_list[filenumber] = frequency;
    current->count += 1;
}

int change_next()
{
    if (root == NULL) {
        return 1;
    }
    if (current == NULL) {
        current = root;
        return 0;
    } else if (current->next == NULL) {
        return 1;
    }
    current = current->next;
    return 0;
}

void search_post(char *argv[])
{
    unsigned int i;
    if (query_num == 0) {
        printf("not inited or no query\n");
        exit(0);
    } else if (query_num == 1) {
        sort_posting(root->post_list, root->count, argv);
    } else {
        /* TODO:  */
        unsigned int post_list[query_num];
        for (i = 0; i < query_num; ++i) {
            
        }
        //std::map<unsigned int, unsigned int> post_list;
        //for (i = 0; i < query_num; ++i) {

        //}
    }
}

bool operator<(const struct posting_s &lhs, const struct posting_s &rhs) {
    return lhs.frequency > rhs.frequency;
}

void sort_posting(std::map<unsigned int, unsigned int> post_list, unsigned int count, char *argv[])
{
    unsigned int i = 0;
    struct posting_s final_list[count];

    std::map<unsigned int, unsigned int>::iterator it;
    for (it = post_list.begin(); it != post_list.end(); ++it) {
        final_list[i].filenumber = it->first;
        final_list[i].frequency = it->second;
        printf("%u\t%u\n", it->first, it->second);
        i++;
    }
    printf("finish sort_posting\n");
    /* TODO:  */
    printf("%d\n", count);
    std::stable_sort(&final_list[0], &final_list[count]);

    final_print(final_list, count, argv);
}

////////////////////////////
 
//bool operator<(const Employee &lhs, const Employee &rhs) {
    //return lhs.age < rhs.age;
//}

////////////////////////////

void free_map()
{
    free_postint_t(root);
}

void free_postint_t(struct posting_t *next_post)
{
    if (next_post == NULL) {
        return;
    } else if (next_post->next != NULL) {
        free_postint_t(next_post->next);
    } else if (next_post->next == NULL) {
        next_post->post_list.clear();
        delete next_post;
    }
}
