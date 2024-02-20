#include <stdlib.h>
#include <getopt.h>
#include "nn.h"

int main(int argc,char **argv)
{
    int ch;
    int check = -1;
    network_t network;
    while((ch = getopt(argc, argv, "c:t"))!= -1) {
        switch(ch) {
        case 'c':
            printf("check number is:'%s'\n", optarg);
            check = atoi(optarg);
            break;
        case 't':
            printf("just run train and test.\n");
            break;
        default:
            break;
        }
    }
    if (-1 != check) {
        network_check(check);
        return 0;
    }
    network_init(&network);

    network_test(&network);
    for (int i = 0; i < TRAINING_EPOCHS; ++i) {
        printf("Training epoch %i/%i\n", i + 1, TRAINING_EPOCHS);
        network_train(&network);
        network_test(&network);
    }

    return 0;
}
