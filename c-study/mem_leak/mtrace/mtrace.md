# FIRST_TEST----------------------

    #make
    #export MALLOC_TRACE=mtrace.log
    #./mtrace_test
    #mtrace mtrace_test mtrace.log 

Memory not freed:
-----------------
           Address     Size     Caller
0x00000000006d6460     0x14  at 0x400473

# SECOND_TEST----------------------

    #make
    #export MALLOC_TRACE=mtrace.log
    #./mtrace_test
    #mtrace mtrace_test mtrace.log 

Memory not freed:
-----------------
           Address     Size     Caller
0x000000000130e460     0x14  at 0x4004af
0x000000000130e480     0x14  at 0x4004bf
0x000000000130e4a0     0x14  at 0x4004bf
0x000000000130e4c0     0x14  at 0x4004bf
0x000000000130e4e0     0x14  at 0x4004bf
0x000000000130e500     0x14  at 0x4004bf
0x000000000130e520     0x14  at 0x4004bf
0x000000000130e540     0x14  at 0x4004bf
0x000000000130e560     0x14  at 0x4004bf
0x000000000130e580     0x14  at 0x4004bf

