#include <stdio.h>
#include <string.h>
#include "concept_search.h"
#include "regular_search.h"

int main(int argc, char *argv[])
{
    if (strcmp(argv[3], "-c") == 0) {
        concept_search(argc, argv);
    }else {
        regular_search(argc, argv);
    }
    return 0;
}
