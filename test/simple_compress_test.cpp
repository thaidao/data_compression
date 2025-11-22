#include <gtest/gtest.h>

extern "C" {
    int simple_validate(uint8_t *data, int len);
    int simple_compress(uint8_t *data, int len);
    int simple_decompress(uint8_t *data, int len, uint8_t *output, int max_len);
}

// Test 1: Short data (10 bytes) with compression
TEST(SimpleCompressTest, ShortDataWithCompression) {
    uint8_t original[] = {65, 65, 65, 66, 67, 67, 68, 69, 69, 69};
    uint8_t compressed[20];
    uint8_t decompressed[20];
    
    // Copy to compressed buffer
    for (int i = 0; i < 10; i++) {
        compressed[i] = original[i];
    }
    
    // Compress
    int compressed_len = simple_compress(compressed, 10);
    EXPECT_GT(compressed_len, 0);
    EXPECT_LT(compressed_len, 10); // Should be smaller
    
    // Decompress
    int decompressed_len = simple_decompress(compressed, compressed_len, decompressed, 20);
    EXPECT_EQ(decompressed_len, 10);
    
    // Verify data matches
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(decompressed[i], original[i]);
    }
}

// Test 2: Long data (2000 bytes) with 130 consecutive bytes and 10 consecutive bytes
TEST(SimpleCompressTest, LongDataWithLongRuns) {
    uint8_t original[2000];
    uint8_t compressed[2500];
    uint8_t decompressed[2500];
    
    // Fill with pattern: 130 'A's, then varied data, then 10 'B's, then varied data
    int pos = 0;
    
    // 130 consecutive 'A's
    for (int i = 0; i < 130; i++) {
        original[pos++] = 65; // 'A'
    }
    
    // Some varied data
    for (int i = 0; i < 1850; i++) {
        original[pos++] = (i % 50) + 20; // Varied values
    }
    
    // 10 consecutive 'B's
    for (int i = 0; i < 10; i++) {
        original[pos++] = 66; // 'B'
    }
    
    // Remaining varied data
    for (int i = pos; i < 2000; i++) {
        original[i] = (i % 30) + 30;
    }
    
    // Copy to compressed buffer
    for (int i = 0; i < 2000; i++) {
        compressed[i] = original[i];
    }
    
    // Compress
    int compressed_len = simple_compress(compressed, 2000);
    EXPECT_GT(compressed_len, 0);
    EXPECT_LT(compressed_len, 2000); // Should be smaller
    
    // Decompress
    int decompressed_len = simple_decompress(compressed, compressed_len, decompressed, 2500);
    EXPECT_EQ(decompressed_len, 2000);
    
    // Verify data matches
    for (int i = 0; i < 2000; i++) {
        EXPECT_EQ(decompressed[i], original[i]);
    }
}

// Test 3: Short data with no repetition
TEST(SimpleCompressTest, ShortDataNoRepetition) {
    uint8_t original[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint8_t compressed[20];
    uint8_t decompressed[20];
    
    // Copy to compressed buffer
    for (int i = 0; i < 10; i++) {
        compressed[i] = original[i];
    }
    
    // Compress
    int compressed_len = simple_compress(compressed, 10);
    EXPECT_EQ(compressed_len, 10); // Should be same size (no compression)
    
    // Verify compressed data is unchanged
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(compressed[i], original[i]);
    }
    
    // Decompress
    int decompressed_len = simple_decompress(compressed, compressed_len, decompressed, 20);
    EXPECT_EQ(decompressed_len, 10);
    
    // Verify data matches
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(decompressed[i], original[i]);
    }
}
