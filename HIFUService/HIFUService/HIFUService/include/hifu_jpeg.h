 
#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include "jpeglib.h" 

void write_bmp_header(j_decompress_ptr cinfo, char* output_data);

void write_bmp_header(j_decompress_ptr cinfo, FILE *output_file);

void write_pixel_data(j_decompress_ptr cinfo, unsigned char *output_buffer, FILE *output_file);

int read_jpeg_file(const char *input_filename, unsigned char *& output_data,int& size);

int read_jpeg_file(const char *input_filename, const char *output_filename);

int write_jpeg_file(char * filename, int quality);