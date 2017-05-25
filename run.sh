#!/bin/bash
rm -r index_files/
clear
make
#time ./a3search ./files/simple/ index_files/ "apple"
#time ./a3search ./files/simple/ ./index_files/ "apple" "investors"
time ./a3search ./files/man/ ./index_files/ "aaaa" "their"
#time ./a3search ./files/booknmail/ ./index_files/ "apple"
#time ./a3search ./files/books200m/ ./index_files/ "Jeffrey"
#time ./a3search ./files/test_d/ ./index_files/ "Jeffrey"
