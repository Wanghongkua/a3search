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
        unsigned int smallest_count = root->count;
        struct posting_t *smallest_post = root;

        struct posting_t *next_post = root;

        std::map<unsigned int, unsigned int> post_list;

        for (i = 0; i < query_num; ++i) {
            if (next_post == NULL) {
                printf("error in search_post for next_post\n");
            } else if (next_post->count < smallest_count) {
                smallest_post = next_post;
                smallest_count = next_post->count;
            }
        }
        std::map<unsigned int, unsigned int>::iterator it;
        for (it = smallest_post->post_list.begin(); it != smallest_post->post_list.end(); ++it) {
            post_list[it->first] = it->second;
        }

        next_post = root;
        for (i = 0; i < query_num; ++i) {
            if (next_post == NULL) {
                break;
            }
            if (post_list.size() == 0) {
                break;
            }
            if (next_post == smallest_post) {
                if (next_post->next == NULL) {
                    break;
                }
                next_post = next_post->next;
                continue;
            }
            map<unsigned int, unsigned int>::iterator it = post_list.begin();
            while(it != post_list.end())
            {
                if (next_post->post_list[it->first] == 0) {
                    post_list.erase(it++);
                }
                else {
                    post_list[it->first] += next_post->post_list[it->first];
                    ++it;
                }
            }

            if (next_post->next == NULL) {
                break;
            }
            next_post = next_post->next;
        }
        sort_posting(post_list, post_list.size(), argv);
    }
}

/*
 *comparision function for stable sort
 */
bool operator<(const struct posting_s &lhs, const struct posting_s &rhs) {
    return lhs.frequency > rhs.frequency;
}

/*
 * stable sort post_list by frequency
 */
void sort_posting(std::map<unsigned int, unsigned int> post_list, unsigned int count, char *argv[])
{
    unsigned int i = 0;
    struct posting_s final_list[count];

    std::map<unsigned int, unsigned int>::iterator it;
    for (it = post_list.begin(); it != post_list.end(); ++it) {
        final_list[i].filenumber = it->first;
        final_list[i].frequency = it->second;
        i++;
    }
    std::stable_sort(&final_list[0], &final_list[count]);

    final_print(final_list, count, argv);
}

/*
 *free map
 */
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
