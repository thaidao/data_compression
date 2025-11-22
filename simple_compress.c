#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define VALIDATE_MSB_ENABLED 0

#if (VALIDATE_MSB_ENABLED)
/**
 * Validate input data to ensure all bytes have MSB = 0 (values < 128)
 * 
 * @param data: Input data array to validate
 * @param len: Length of input data
 * @return: 0 if valid (all bytes < 128), -1 if invalid
 */
int simple_validate(uint8_t *data, int len) {
    if (data == NULL || len <= 0) {
        return -1;
    }
    
    for (int i = 0; i < len; i++) {
        if (data[i] >= 128) {
            printf("Validation failed at index %d: value 0x%02X >= 128 (MSB=1)\n", i, data[i]);
            return -1;
        }
    }
    
    return 0;
}
#endif

/**
 * Simple compression using run-length encoding
 * @param data: Input/output array (data will be overwritten with compressed data)
 * @param len: Length of input data
 * @return: Length of compressed data
 */
int simple_compress(uint8_t *data, int len) {
    if (data == NULL || len <= 0) {
        return 0;
    }

#if (VALIDATE_MSB_ENABLED)
    // Validate input data first
    if (simple_validate(data, len) != 0) {
        printf("Error: Input data validation failed\n");
        return -1;
    }
#endif
    
    int read_pos = 0;   // Position for reading original data
    int write_pos = 0;  // Position for writing compressed data
    
    while (read_pos < len) {
        uint8_t current_byte = data[read_pos];
        int count = 1;
        
        // Count consecutive occurrences of the same byte
        while (read_pos + count < len && 
               data[read_pos + count] == current_byte && 
               count < 127) {
            count++;
        }
        
        if (count == 1) {
            // Single byte, no compression needed
            data[write_pos++] = current_byte;
        } else {
            // Multiple consecutive bytes - use RLE encoding
            data[write_pos++] = 0x80 | count;  // MSB=1 + count
            data[write_pos++] = current_byte;   // The repeated byte
        }
        
        read_pos += count;
    }
    
    return write_pos;
}

/**
 * Decompress data that was compressed with the simple_compress() function
 * 
 * @param compressed: Compressed input data
 * @param compressed_len: Length of compressed data
 * @param output: Output buffer for decompressed data
 * @param output_max: Maximum size of output buffer
 * @return: Length of decompressed data, or -1 on error
 */
int simple_decompress(uint8_t *compressed, int compressed_len, uint8_t *output, int output_max) {
    if (compressed == NULL || output == NULL || compressed_len <= 0) {
        return -1;
    }
    
    int read_pos = 0;
    int write_pos = 0;
    
    while (read_pos < compressed_len) {
        uint8_t byte = compressed[read_pos++];
        
        if (byte & 0x80) {
            // MSB = 1: This is a run-length encoded sequence
            int count = byte & 0x7F;  // Get the count (lower 7 bits)
            
            if (read_pos >= compressed_len) {
                return -1;  // Error: missing data byte
            }
            
            uint8_t data_byte = compressed[read_pos++];
            
            // Check if we have enough space in output
            if (write_pos + count > output_max) {
                return -1;  // Buffer overflow
            }
            
            // Write the repeated byte
            for (int i = 0; i < count; i++) {
                output[write_pos++] = data_byte;
            }
        } else {
            // MSB = 0: Regular byte, copy as-is
            if (write_pos >= output_max) {
                return -1;  // Buffer overflow
            }
            output[write_pos++] = byte;
        }
    }
    
    return write_pos;
}

// Test function
int main() {
    // Test case 1: Data with consecutive repetitions
    uint8_t data1[] = {1, 1, 1, 1, 1, 2, 3, 3, 3, 4};
    int len1 = sizeof(data1);
    
//=========================================================================
    printf("=== Test 1: Small data with repetitions ===\n");
    printf("Original data 1: ");
    for (int i = 0; i < len1; i++) {
        printf("%02X ", data1[i]);
    }
    printf("(length: %d)\n", len1);
    
    int compressed_len1 = simple_compress(data1, len1);
    
    printf("Compressed data 1: ");
    for (int i = 0; i < compressed_len1; i++) {
        printf("%02X ", data1[i]);
    }
    printf("(length: %d)\n", compressed_len1);
    
    // Decompress to verify
    uint8_t decompressed1[100];
    int decompressed_len1 = simple_decompress(data1, compressed_len1, decompressed1, 100);
    
    printf("Decompressed data 1: ");
    for (int i = 0; i < decompressed_len1; i++) {
        printf("%02X ", decompressed1[i]);
    }
    printf("(length: %d)\n\n", decompressed_len1);

//=========================================================================    
    // Test case 2: Data without repetitions
    uint8_t data2[] = {1, 2, 3, 4, 5};
    int len2 = sizeof(data2);
    
    printf("=== Test 2: Data without repetitions ===\n");
    printf("Original data 2: ");
    for (int i = 0; i < len2; i++) {
        printf("%02X ", data2[i]);
    }
    printf("(length: %d)\n", len2);
    
    int compressed_len2 = simple_compress(data2, len2);
    
    printf("Compressed data 2: ");
    for (int i = 0; i < compressed_len2; i++) {
        printf("%02X ", data2[i]);
    }
    printf("(length: %d)\n", compressed_len2);
    
    // Decompress to verify
    uint8_t decompressed2[100];
    int decompressed_len2 = simple_decompress(data2, compressed_len2, decompressed2, 100);
    
    printf("Decompressed data 2: ");
    for (int i = 0; i < decompressed_len2; i++) {
        printf("%02X ", decompressed2[i]);
    }
    printf("(length: %d)\n\n", decompressed_len2);
	
	
//=========================================================================
    // Test case 3: Large data with 132 consecutive identical bytes
    printf("=== Test 3: Large data (1024 bytes) with 132 consecutive identical bytes ===\n");
    uint8_t *data3 = (uint8_t *)malloc(1024);
    if (data3 == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    // Fill with pattern: 132 bytes of 0x7F (127), then varying data
    for (int i = 0; i < 132; i++) {
        data3[i] = 0x7F;
    }
    // Fill rest with pattern
    for (int i = 132; i < 1024; i++) {
        data3[i] = (i % 100);  // Values 0-99, all < 127
    }
    
    printf("Original data 3 (first 140 bytes): ");
    for (int i = 0; i < 140; i++) {
        if (i % 20 == 0 && i > 0) printf("\n                                     ");
        printf("%02X ", data3[i]);
    }
    printf("\n... (length: %d)\n", 1024);
    
    int compressed_len3 = simple_compress(data3, 1024);
    
    printf("Compressed data 3 (first 140 bytes): ");
    for (int i = 0; i < (compressed_len3 < 140 ? compressed_len3 : 140); i++) {
        if (i % 20 == 0 && i > 0) printf("\n                                      ");
        printf("%02X ", data3[i]);
    }
    printf("\n... (length: %d)\n", compressed_len3);
    printf("Compression ratio: %.2f%%\n", (float)compressed_len3 / 1024 * 100);
    
    // Explain the encoding of 132 consecutive bytes
    printf("\nExplanation of 132 consecutive 0x7F bytes:\n");
    printf("  - Max count per RLE token = 127 (7 bits)\n");
    printf("  - 132 = 127 + 5\n");
    printf("  - Encoded as: [FF 7F] [85 7F]\n");
    printf("    * FF = 0xFF = 1111_1111 (MSB=1, count=127)\n");
    printf("    * 7F = 0x7F = 0111_1111 (MSB=0, data byte = 127)\n");
    printf("    * 85 = 0x85 = 1000_0101 (MSB=1, count=5)\n");
    printf("    * 7F = 0x7F = 0111_1111 (MSB=0, data byte = 127)\n\n");
    
    // Decompress to verify
    uint8_t *decompressed3 = (uint8_t *)malloc(2048);
    if (decompressed3 == NULL) {
        printf("Memory allocation failed\n");
        free(data3);
        return 1;
    }
    
    int decompressed_len3 = simple_decompress(data3, compressed_len3, decompressed3, 2048);
    
    printf("Decompressed data 3 (first 140 bytes): ");
    for (int i = 0; i < 140; i++) {
        if (i % 20 == 0 && i > 0) printf("\n                                        ");
        printf("%02X ", decompressed3[i]);
    }
    printf("\n... (length: %d)\n", decompressed_len3);
    
    // Verify correctness
    int match = 1;
    if (decompressed_len3 != 1024) {
        match = 0;
    } else {
        for (int i = 0; i < 132; i++) {
            if (decompressed3[i] != 0x7F) {
                match = 0;
                break;
            }
        }
        for (int i = 132; i < 1024; i++) {
            if (decompressed3[i] != (i % 100)) {
                match = 0;
                break;
            }
        }
    }
    printf("Verification: %s\n", match ? "PASSED" : "FAILED");
    
    free(data3);
    free(decompressed3);
    
    return 0;
}
