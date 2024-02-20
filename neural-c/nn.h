/**
 * @file nn.h
 * @brief simple neutral
 * @author xuchunxiao369@163.com
 * @version V0.9.x
 * @date 2023-10-18
 */

#include "config.h"
#include "data_load.h"

typedef struct _node {
    double bias;
    double output;
    double real_output;
    double back_prop_value;
    int weights_number;
    double *weights;
} node_t;

typedef struct _layer {
    int nodes_number;
    node_t *nodes;
} layer_t;

typedef struct _network {
    layer_t  input_layer;
    layer_t hidden_layer;
    layer_t output_layer;
} network_t;

void network_init(network_t *network);
void network_train(network_t *network);
void network_test(network_t *network);
void network_check(int num);
