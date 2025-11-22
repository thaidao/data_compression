#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define VALIDATE_MSB_ENABLED 0

#if (VALIDATE_MSB_ENABLED)
/**
 * Validate input data to ensure all bytes have MSB = 0 (values < 128)
 */
int simple_validate(uint8_t *data, int len) {
    return 0;
}
#endif

/**
 * Simple compression using run-length encoding
 */
int simple_compress(uint8_t *data, int len) {

    int read_pos = 0;   // Position for reading original data
    int write_pos = 0;  // Position for writing compressed data
    
    while (read_pos < len) {
		//@todo
    }
    
    return write_pos;
}

/**
 * Decompress data that was compressed with the simple_compress() function
 */
int simple_decompress(uint8_t *compressed, int compressed_len, uint8_t *output, int output_max) {
	
    if (compressed == NULL || output == NULL || compressed_len <= 0) {
        return -1;
    }
    
    int read_pos = 0;
    int write_pos = 0;
    
    while (read_pos < compressed_len) {
		//@todo
    }
    
    return write_pos;
}

// Test function
int main() {
    // Test case 1: Data with consecutive repetitions
    uint8_t data1[] = {1, 1, 1, 1, 1, 2, 3, 3, 3, 4};
    int len1 = sizeof(data1);
    
    int compressed_len1 = simple_compress(data1, len1);
    printf("Compressed data 1: ");

    // Decompress to verify
	//@todo
    return 0;
}
