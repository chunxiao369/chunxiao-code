#include <stdlib.h>
#include "data_load.h"

static void read_image_file_header(FILE * img_file, image_file_header_t * ifh);
static void read_label_file_header(FILE * img_file, label_file_header_t * lfh);
static uint32_t filp_bytes(uint32_t n);

void get_image(FILE * img_file, image_t * img)
{
    size_t result;
    result = fread(img, sizeof(*img), 1, img_file);
    if (result != 1) {
        printf("\nError when reading IMAGE file! Abort!\n");
        exit(1);
    }
}

uint8_t get_label(FILE * labelFile)
{
    uint8_t label;
    size_t result;
    result = fread(&label, sizeof(uint8_t), 1, labelFile);
    if (result != 1) {
        printf("\nError when reading LABEL file! Abort!\n");
        exit(1);
    }

    return label;
}

FILE *open_image_file(char *fileName, image_file_header_t * img_fileHeader)
{
    FILE *img_file;
    img_file = fopen(fileName, "rb");
    if (img_file == NULL) {
        printf("Abort! Could not fine MNIST IMAGE file: %s\n", fileName);
        exit(0);
    }

    read_image_file_header(img_file, img_fileHeader);
    return img_file;
}

FILE *open_label_file(char *fileName)
{
    FILE *labelFile;
    labelFile = fopen(fileName, "rb");
    if (labelFile == NULL) {
        printf("Abort! Could not find MNIST LABEL file: %s\n", fileName);
        exit(0);
    }

    label_file_header_t labelFileHeader;
    read_label_file_header(labelFile, &labelFileHeader);
    return labelFile;
}

static void read_image_file_header(FILE * img_file, image_file_header_t * ifh)
{
    size_t result;
    ifh->magic_num = 0;
    ifh->max_images = 0;
    ifh->img_width = 0;
    ifh->img_height = 0;

    result = fread(&ifh->magic_num, 4, 1, img_file);
    ifh->magic_num = filp_bytes(ifh->magic_num);

    result = fread(&ifh->max_images, 4, 1, img_file);
    ifh->max_images = filp_bytes(ifh->max_images);

    result = fread(&ifh->img_width, 4, 1, img_file);
    ifh->img_width = filp_bytes(ifh->img_width);

    result = fread(&ifh->img_height, 4, 1, img_file);
    ifh->img_height = filp_bytes(ifh->img_height);
    if (0 == result) {
        printf("file err!!!\n");
    }
}

static void read_label_file_header(FILE * img_file, label_file_header_t * lfh)
{
    size_t result;
    lfh->magic_num = 0;
    lfh->max_labels = 0;

    result = fread(&lfh->magic_num, 4, 1, img_file);
    lfh->magic_num = filp_bytes(lfh->magic_num);

    result = fread(&lfh->max_labels, 4, 1, img_file);
    if (0 == result) {
        printf("file err!!!\n");
        return;
    }
    lfh->max_labels = filp_bytes(lfh->max_labels);
}

static uint32_t filp_bytes(uint32_t n)
{
    uint32_t b0, b1, b2, b3;

    b0 = (n & 0x000000ff) << 24u;
    b1 = (n & 0x0000ff00) << 8u;
    b2 = (n & 0x00ff0000) >> 8u;
    b3 = (n & 0xff000000) >> 24u;

    return (b0 | b1 | b2 | b3);
}
