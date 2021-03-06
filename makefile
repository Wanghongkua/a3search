include mkinc.h
objects = a3search.o concept_search.o regular_search.o process_query.o process_index.o stemmer.o libstemmer.o word_frequency.o tmp_index.o merge_index.o search_index.o posting.o

object = a3search.o concept_search.o regular_search.o process_query.o process_index.o stemmer.o word_frequency.o tmp_index.o merge_index.o
program_C_SRCS := $(wildcard *.c)

ALL: $(objects)
	g++ -o a3search -O3 $(objects)

a3search.o: a3search.c concept_search.h regular_search.h
	tar xf *.tar
	gcc -c a3search.c

concept_search.o: concept_search.c concept_search.h
	gcc -c concept_search.c

regular_search.o: regular_search.c regular_search.h process_query.h stemmer.h search_index.h
	gcc -c regular_search.c

process_query.o: process_query.c process_query.h
	gcc -c process_query.c

process_index.o: process_index.c process_index.h word_frequency.h tmp_index.h merge_index.h
	gcc -c process_index.c

stemmer.o: stemmer.c stemmer.h
	gcc -c stemmer.c

word_frequency.o: word_frequency.cpp tmp_index.h
	g++ -c word_frequency.cpp

tmp_index.o: tmp_index.c tmp_index.h
	gcc -c tmp_index.c

merge_index.o: merge_index.c merge_index.h
	gcc -c merge_index.c

search_index.o: search_index.c search_index.h posting.h
	gcc -c search_index.c

posting.o: posting.cpp posting.h
	g++ -c posting.cpp

libstemmer.o: $(snowball_sources:.c=.o)
	$(AR) -cru $@ $^

debug: $(object)
	g++ -o a3search -g $(object)

clean:
	rm -f *.o a3search libstemmer_c/src_c/*.o libstemmer_c/runtime/*.o libstemmer_c/libstemmer/*.o
