/**
 * @file data_load.h
 * @brief  data load from MNIST data
 * @author xuchunxiao369@163.com
 * @version V0.9.x
 * @date 2023-10-18
 */

#include <stdint.h>
#include <stdio.h>
#include "config.h"

typedef struct _image {
    uint8_t pixels[IMAGE_SIZE];
} image_t;

typedef struct _image_file_header {
    uint32_t magic_num;
    uint32_t max_images;
    uint32_t img_width;
    uint32_t img_height;
} image_file_header_t;

typedef struct label_file_header {
    uint32_t magic_num;
    uint32_t max_labels;
} label_file_header_t ;

void get_image(FILE * image_file, image_t * img);
uint8_t get_label(FILE * label_file);
FILE *open_image_file(char *fileName, image_file_header_t * img_hdr);
FILE *open_label_file(char *fileName);
