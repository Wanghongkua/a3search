Struct
=======

Search Method
----------------
Example Search Query: ./a3search files/books200m index_files "apple"
  - The first argument is the file folder needed to be searched
  - The second argument is the name of the folder storing the generated indexes
  - The third argument is the word needed to be searched

Program Architecture
----------------
  - **a3search.c**  
      determine perform regular search or concept search
  - **regular_search.c**  
      sort the query for each search and call process_index() to create index files and call load_index() to load corresponding posting list
  - **process_query.c**  
      sort the queries and do stem
  - **process_index.c**  
      create index if not exist:  
      1. first create temporary index files with max number of 200 blocks.  
      2. merge those blocks to 1 file "final_index.txt", and 1 lookup file for binary search "final_lookup.txt"  
      3. delete temporary files
  - **merge_index.c**  
      merge tmp index files to 1 file
  - **search_index.c**  
      perform binary search based on "final_lookup.txt" file to search in "final_index" file. If found matches, call search_post() to search for result.
  - **posting.cpp**  
      use cpp library map to sort posting and perform searching inverted index
  - **stemmer.c**  
      stem the word by external libray "libstemmer_c"
  - **word_frequency.cpp**  
      use cpp library map to pre-process posting list to sort posting based on filenumber

Index Architecture
----------------
  - **Over View**  
      word | 0x01 | filenumber | 0x01 | frequency | 0x01 | ... | frequency | 0x00 | word
  - **Filenumber and Frequency**  
      using little endin and for each byte stored in index file only store 6 bits each byte.
      for each byte, says: 0b10XXXXXX, "X" is for storing actual content.  
      Due To "0xff" is for "EOF" !!!!
  - **Lookup**  
      for each word in index file, store it's position in index file to lookup file, fixed in 4 bytes.

TODO
----------------
Add concept search in the future
