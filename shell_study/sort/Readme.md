'''
cat a.txt | sort -k 1 -k 2 -t ',' -n
cat a.txt | sort -k 1,2  -t ',' -n
cat a.txt | sort -k 1,1 -k 2,2 -t ',' -n
'''
