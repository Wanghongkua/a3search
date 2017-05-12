FILES=

ALL:a3search

a3search: a3search.c concept_search.c regular_search.c sort_query.c
	gcc -o a3search -O3 a3search.c concept_search.c regular_search.c sort_query.c -I.


debug: a3search.c concept_search.c regular_search.c sort_query.c
	gcc -o a3search -g a3search.c concept_search.c regular_search.c sort_query.c -I.

clean:
	rm -f *.o a3search
