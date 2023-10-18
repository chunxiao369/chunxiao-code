#include "nn.h"

int main()
{
    network_t network;
    network_init(&network);

    network_test(&network);
    for (int i = 0; i < TRAINING_EPOCHS; ++i) {
        printf("Training epoch %i/%i\n", i + 1, TRAINING_EPOCHS);
        network_train(&network);
        network_test(&network);
    }

    return 0;
}
