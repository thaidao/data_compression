# Simple Compress - Release Notes

## Version 1.0.0

### Features
- **Run-Length Encoding (RLE) compression** with MSB flag
  - Uses MSB=1 for compressed sequences, MSB=0 for data
  - Maximum run length: 127 bytes per token
  - In-place compression (overwrites input array)

- **Core Functions:**
  - `simple_compress()` - Compress data using RLE
  - `simple_decompress()` - Decompress RLE data
  - `simple_validate()` - Validate input data (all bytes < 128)

### Technical Details
- Input constraint: All data bytes must have MSB=0 (values < 128)
- Encoding format:
  - Single byte: `[data]` (MSB=0)
  - Multiple bytes: `[0x80|count] [data]` (MSB=1 for count)
- Compression ratio: Up to 50% for highly repetitive data

### Examples
```
Input:  [0x7F × 132 times] → Output: [0xFF 0x7F 0x85 0x7F]
        (132 bytes)                   (4 bytes)

Input:  [0x01 0x01 0x01 0x01 0x01] → Output: [0x85 0x01]
        (5 bytes)                            (2 bytes)
```

### Validation
- Automatically validates input before compression
- Rejects data with MSB=1 (values ≥ 128)
- Returns -1 on validation failure

---
**Author:** Thomas
**Date:** November 21, 2025  
**Language:** C (C99+)
