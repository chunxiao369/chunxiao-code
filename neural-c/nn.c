#include "nn.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static void layer_init(int nodes_number, int weights_number, layer_t * layer);
static void node_init(int weights_number, node_t * node);
static void feed_forward_layer(layer_t * previous_layer, layer_t * layer);
static void feed_forward(network_t * network, image_t * img);
static void node_update(layer_t * previous_layer, double back_prop_value, node_t * node);
static void back_propagate(network_t * network, int label);
static uint8_t get_classification(layer_t * layer);

void network_init(network_t * network)
{
    layer_init(IMAGE_SIZE, 0, &network->input_layer);
    layer_init(HIDDEN_LAYER_SIZE, IMAGE_SIZE, &network->hidden_layer);
    layer_init(OUTPUT_SIZE, HIDDEN_LAYER_SIZE, &network->output_layer);
}

void network_train(network_t * network)
{
    FILE *img_file;
    FILE *label_file;
    image_file_header_t img_file_hdr;
    img_file = open_image_file(TRAINING_SET_IMAGE_FILE_NAME, &img_file_hdr);
    label_file = open_label_file(TRAINING_SET_LABEL_FILE_NAME);

    for (int i = 0; i < img_file_hdr.max_images; i++) {
        image_t img;
        get_image(img_file, &img);
        uint8_t label = get_label(label_file);

        feed_forward(network, &img);
        back_propagate(network, label);
    }
}

void network_test(network_t * network)
{
    FILE *img_file;
    FILE *label_file;
    image_file_header_t img_file_hdr;
    img_file = open_image_file(TEST_SET_IMAGE_FILE_NAME, &img_file_hdr);
    label_file = open_label_file(TEST_SET_LABEL_FILE_NAME);

    int errCount = 0;
    for (int i = 0; i < img_file_hdr.max_images; i++) {
        image_t img;
        get_image(img_file, &img);
        uint8_t lbl = get_label(label_file);
        feed_forward(network, &img);

        uint8_t classification = get_classification(&network->output_layer);
        if (classification != lbl) {
            errCount++;
        }
    }
    fclose(img_file);
    fclose(label_file);

    printf("Test Accuracy: %0.2f%%\n",
           ((double)(img_file_hdr.max_images - errCount) / img_file_hdr.max_images) * 100);
}

static void layer_init(int nodes_number, int weights_number, layer_t * layer)
{
    node_t *nodes = malloc(nodes_number * sizeof(node_t));
    for (int hn = 0; hn < nodes_number; ++hn) {
        node_t *node = &nodes[hn];
        node_init(weights_number, node);
    }

    layer->nodes_number = nodes_number;
    layer->nodes = nodes;
}

static void node_init(int weights_number, node_t * node)
{
    //Initialize weights between -0.7 and 0.7
    double *weights = malloc(weights_number * sizeof(double));

    for (int w = 0; w < weights_number; ++w) {
        weights[w] = 0.7 * (rand() / (double)(RAND_MAX));
        if (w % 2) {
            weights[w] = -weights[w];
        }
    }

    node->weights_number = weights_number;
    node->weights = weights;
    node->bias = rand() / (double)(RAND_MAX);
}

/* sigmoid function */
static double sigmoid(double value)
{
    return 1.0 / (1.0 + exp(-value));
}

static double sigmoid_derivative(double value)
{
    return sigmoid(value) * (1 - sigmoid(value));
}

static double sigmoid_derivative_opt(double output)
{
    return output * (1 - output);
}

/* hyperbolic tangent function
 *
 *   exp(value) - exp(-value)
 *   _______________________
 *
 *   exp(value) + exp(-value)
 */
static double tanh_act(double value)
{
    return (tanh(value) + 1)/2;
}

static double tanh_derivative(double value)
{
    return 1 / (2 * cosh(value) * cosh(value));
}

//#define SIGMOID 1
//#define PERF_OPT 1
#define ATANACT 1
static double act_func(double value)
{
#ifdef SIGMOID
    return sigmoid(value);
#elif  ATANACT
    return tanh_act(value);
#endif
}

static double act_derivative(double value)
{
#ifdef SIGMOID
    return sigmoid_derivative(value);
#elif  ATANACT
    return tanh_derivative(value);
#endif
}

static double act_derivative_opt(double value)
{
#ifdef SIGMOID
    return sigmoid_derivative_opt(value);
#elif  ATANACT
    return tanh_derivative(value);
#endif
}

static void feed_forward_layer(layer_t * previous_layer, layer_t * layer)
{
    for (int hn = 0; hn < layer->nodes_number; ++hn) {
        node_t *node = &layer->nodes[hn];
        node->output = node->bias;

        for (int w = 0; w < previous_layer->nodes_number; ++w) {
            node->output += previous_layer->nodes[w].output * node->weights[w];
        }
        node->real_output = node->output;
        node->output = act_func(node->output);
    }
}

static void feed_forward(network_t * network, image_t * img)
{
    //Populate the input layer with normalized input
    for (int i = 0; i < IMAGE_SIZE; ++i) {
        network->input_layer.nodes[i].output = (double)(img->pixels[i] / 255.0);
    }

    feed_forward_layer(&network->input_layer, &network->hidden_layer);
    feed_forward_layer(&network->hidden_layer, &network->output_layer);
}

static void node_update(layer_t * previous_layer, double back_prop_value, node_t * node)
{
    for (int hn = 0; hn < previous_layer->nodes_number; ++hn) {
        node_t *previous_layerNode = &previous_layer->nodes[hn];
        node->weights[hn] += LEARNING_RATE * previous_layerNode->output * back_prop_value;
    }
    node->bias += LEARNING_RATE * back_prop_value;
}

static void back_propagate(network_t * network, int label)
{
    layer_t *hidden_layer = &network->hidden_layer;
    layer_t *output_layer = &network->output_layer;

    for (int on = 0; on < output_layer->nodes_number; ++on) {
        node_t *output_node = &output_layer->nodes[on];

        int node_target = (on == label) ? 1 : 0;
        double delta = node_target - output_node->output;
#ifdef PERF_OPT
        double back_prop_value = delta * act_derivative_opt(output_node->output);
#else
        double back_prop_value = delta * act_derivative(output_node->real_output);
#endif

        output_node->back_prop_value = back_prop_value;
        node_update(&network->hidden_layer, output_node->back_prop_value, output_node);
    }

    for (int hn = 0; hn < hidden_layer->nodes_number; ++hn) {
        node_t *hidden_node = &hidden_layer->nodes[hn];

        double output_nodes_back_prop_sum = 0;

        for (int on = 0; on < output_layer->nodes_number; ++on) {
            node_t *output_node = &output_layer->nodes[on];
            output_nodes_back_prop_sum += output_node->back_prop_value * output_node->weights[hn];
        }

#ifdef PERF_OPT
        double hidden_node_back_prop_value = output_nodes_back_prop_sum * act_derivative_opt(hidden_node->output);
#else
        double hidden_node_back_prop_value = output_nodes_back_prop_sum * act_derivative(hidden_node->real_output);
#endif
        node_update(&network->input_layer, hidden_node_back_prop_value, hidden_node);
    }
}

static uint8_t get_classification(layer_t * layer)
{
    double max_output = 0;
    int max_index = 0;

    for (int on = 0; on < layer->nodes_number; ++on) {
        double node_output = layer->nodes[on].output;
        if (node_output > max_output) {
            max_output = node_output;
            max_index = on;
        }
    }
    return (uint8_t) max_index;
}

void img_print(image_t *img)
{
    int i = 0;
    int j = 0;
    for (i = 0; i < 28; i++) {
        for (j = 0; j < 28; j++) {
            if (img->pixels[i * 28 + j] == 0) {
                printf("  ");
            } else {
                printf("%02u", img->pixels[i * 28 + j]);
            }
        }
        printf("\n");
    }
}

void network_check(int num)
{
    FILE *img_file;
    FILE *label_file;
    image_file_header_t img_file_hdr;
    img_file = open_image_file(TEST_SET_IMAGE_FILE_NAME, &img_file_hdr);
    label_file = open_label_file(TEST_SET_LABEL_FILE_NAME);

    for (int i = 0; i < img_file_hdr.max_images; i++) {
        image_t img;
        get_image(img_file, &img);
        uint8_t lbl = get_label(label_file);
        if (i == num) {
            img_print(&img);
            printf("label is: %u.\n", lbl);
        }
    }
    fclose(img_file);
    fclose(label_file);

}
