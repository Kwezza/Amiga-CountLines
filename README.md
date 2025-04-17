# Amiga Line Counter

A fast and efficient line counter utility for Amiga OS, written in C and optimised for performance using buffered reads and loop unrolling (Slight speed up on my 68030).

## Description

This utility counts the number of lines in a text file by scanning for newline characters ('\n'). It's designed to be fast and memory-efficient, using buffered reads and optimised memory allocation in FAST RAM (if available).  Just include the linecounter.c and linecounter.h in your project and you should be good to go.

## Features

- Fast line counting using buffered reads
- Memory efficient (doesn't load entire file into memory)
- Optimized for performance with loop unrolling
- Uses FAST RAM allocation for better performance
- Simple command-line interface

## Requirements

- Amiga OS (tested on Workbench 3.2, but should be compatible with Workbench 1,2 & 3)
- SAS/C compiler
- At least 8KB of free memory for the read buffer

## Installation

1. Clone this repository to your Amiga
2. Compile using SAS/C:
   ```
   sc main.c
   ```

## Usage

```
LineCount <filename>
```

Example:
```
LineCount testfile.txt
```

The program will output the number of lines found in the specified file.

## Technical Details

- Uses a buffer size of 8192 bytes for efficient reading
- Implements 2x loop unrolling for performance optimisation (slight speedup on my 68030, but still works fine on a 68000)
- Allocates memory in FAST RAM when available
- Returns -1 on error (file not found, memory allocation failed, etc.)

## Notes

- Only counts lines based on the newline character '\n'
- Files using carriage return '\r' or CRLF (\r\n) line endings may not count correctly unless pre-processed
- The program is optimized for Amiga OS and may not work correctly on other systems

## License

This project is open source and available under the MIT License.

## Author

Kwezza 