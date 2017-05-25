#include <iostream>
#include <string>
#include <map>
#include "word_frequency.h"
#include "tmp_index.h"

using namespace std;

std::map<std::string, struct word_element *> word_frequency;
unsigned int filenumber = 0;

/*
 *update word frequency to map
 */
void update_wordfrequency(char *word)
{
    if (word == NULL) {
        cout << "no memory in update_wordfrequency for word" << endl;
    }
    if (word_frequency[word] == NULL) {
        word_frequency[word] = new struct word_element;
        word_frequency[word]->first_posting = new struct posting_list;

        word_frequency[word]->last_posting = word_frequency[word]->first_posting;
        word_frequency[word]->last_posting->used = 1;
        word_frequency[word]->last_posting->next = NULL;
        word_frequency[word]->last_posting->file_number = filenumber;
        word_frequency[word]->last_posting->frequency = 0;

    } else if (! word_frequency[word]->last_posting->used) {
        word_frequency[word]->last_posting->used = 1;
        word_frequency[word]->last_posting->file_number = filenumber;
    }
    word_frequency[word]->last_posting->frequency += 1;
}

/*
 *store file number to global variable
 */
void init_filenumber(unsigned int f_num)
{
    filenumber = f_num;
}

/*
 *change to next post
 */
void change_to_next()
{
    std::map<std::string, struct word_element *>::iterator it;
    for (it = word_frequency.begin(); it != word_frequency.end(); ++it) {
        if (it->second->last_posting->used) {

            it->second->last_posting->next = new struct posting_list;
            it->second->last_posting = it->second->last_posting->next;
            it->second->last_posting->next = NULL;
            it->second->last_posting->used = 0;
            it->second->last_posting->frequency = 0;
        }
    }
}

void clean_wordfrequency()
{
    std::map<std::string, struct word_element *>::iterator it;
    for (it = word_frequency.begin(); it != word_frequency.end(); ++it) {

        print_to_file(it->first.c_str(), it->second->first_posting);

        free_posting(it->second->first_posting);
        if (it->second == NULL) {
            cout << "no memory in clean_wordfrequency for it->second" << endl;
        }

        delete it->second;
    }
    word_frequency.clear();
}

void free_posting(struct posting_list *root)
{
    if (root != NULL) {
        //if (root->used) {
            //cout << "\t" << root->file_number << "\t" << root->frequency;
        //}
        if (root->next != NULL) {
            free_posting(root->next);
        }
        delete root;
    }
}
