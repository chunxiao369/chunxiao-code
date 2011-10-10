/*
    cpc.c
    use c library to copy file
*/

#include <stdio.h>
#include <errno.h>

#define BUF_SIZE    256

int main(int argc, char *argv[])
{
    FILE *in_file, *out_file;
    char data[BUF_SIZE];
    size_t bytes_in, bytes_out;
    long len = 0;

    if (argc != 3) {
        printf("Usage: %s file1 file2\n", argv[0]);
        return 1;
    }

    if ((in_file = fopen(argv[1], "rb")) == NULL) {
        perror(argv[1]);
        return 2;
    }
    if ((out_file = fopen(argv[2], "wb")) == NULL) {
        perror(argv[2]);
        return 3;
    }

    while ((bytes_in = fread(data, 1, BUF_SIZE, in_file)) > 0) {
        bytes_out = fwrite(data, 1, bytes_in, out_file);
        if (bytes_in != bytes_out) {
            perror("Fatal write error.\n");
            return 4;
        }
        len += bytes_out;
        printf("copying file .... %d bytes copy\n", len);
    }

    fclose(in_file);
    fclose(out_file);

    return 0;
}
