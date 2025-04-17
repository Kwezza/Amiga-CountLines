#include <stdio.h>
#include "linecounter.h"

int main(int argc, char **argv)
{
    LONG count;

    if (argc < 2)
    {
        printf("Usage: LineCount <filename>\n");
        return RETURN_FAIL;
    }

    count = CountLines((STRPTR)argv[1]);

    if (count < 0)
    {
        printf("Error: Failed to count lines or file not found\n");
        return RETURN_FAIL;
    }

    printf("Number of lines: %ld\n", count);
}