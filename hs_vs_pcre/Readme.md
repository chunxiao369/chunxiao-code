./simple_grep ".*,123456,.*" sigfw.txt h 3

cat /dev/urandom | base64 | strings | head -n 1024 > 16_1024.txt
./simple_grep "explaina[a]+" 16_1024.txt 1000
./simple_grep "^[a-zA-Z0-9]+" 16_1024.txt p 100

lance@lance-T470s:~/Project/legacy_prj/hs_vs_pcre$ time ./simple_grep "^[a-zA-Z0-9]+" 16_1024.txt h 100000000
Get 1024 lines from file 16_1024.txt...
Scanning 100000000 times with hyperscan
match counter: 97167969.

real	0m5.754s
user	0m5.750s
sys	0m0.004s
lance@lance-T470s:~/Project/legacy_prj/hs_vs_pcre$ time ./simple_grep "^[a-zA-Z0-9]+" 16_1024.txt p 100000000
Get 1024 lines from file 16_1024.txt...
Scanning 100000000 times with libpcre
match counter: 97167969.

real	0m10.553s
user	0m10.544s
sys	0m0.008s

lance@lance-T470s:~/Project/legacy_prj/hs_vs_pcre$ time ./simple_grep "^[a-zA-Z0-9]+$" 16_1024.txt h 100000000
Get 1024 lines from file 16_1024.txt...
Scanning 100000000 times with hyperscan
match counter: 9863284.

real	0m12.494s
user	0m12.489s
sys	0m0.000s
lance@lance-T470s:~/Project/legacy_prj/hs_vs_pcre$ time ./simple_grep "^[a-zA-Z0-9]+$" 16_1024.txt p 100000000
Get 1024 lines from file 16_1024.txt...
Scanning 100000000 times with libpcre
match counter: 9863284.

real	0m49.218s
user	0m49.201s
sys	0m0.004s

lance@lance-T470s:~/Project/legacy_prj/hs_vs_pcre$ time ./simple_grep "^[a-zA-Z0-9]+" 16_1024.txt h 100000000
Get 1024 lines from file 16_1024.txt...
Scanning 100000000 times with hyperscan
match counter: 97167969.

real	0m7.132s
user	0m7.128s
sys	0m0.004s

lance@lance-T470s:~/Project/legacy_prj/hs_vs_pcre$ time ./simple_grep "^[a-zA-Z0-9]+" 16_1024.txt p 100000000
Get 1024 lines from file 16_1024.txt...
Scanning 100000000 times with libpcre
match counter: 97167969.

real	0m11.145s
user	0m11.137s
sys	0m0.008s

stream mode: ./multi_grep n
block  mode: ./multi_grep
