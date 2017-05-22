#include <iostream>
#include <string>
#include <map>
#include "word_frequency.h"

using namespace std;

std::map<std::string, int> word_frequency;

void update_wordfrequency(char *word)
{
    if (word == NULL) {
        cout << "no memory in update_wordfrequency for word" << endl;
        exit(0);
    }
    word_frequency[word] += 1;
}

void clean_wordfrequency()
{
    std::map<std::string, int>::iterator it;
    for (it = word_frequency.begin(); it != word_frequency.end(); ++it) {
        //cout << it->first << ", " << it->second << '\n';

    }
    word_frequency.clear();
}
