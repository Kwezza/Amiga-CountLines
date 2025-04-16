#include <exec/types.h>
#include <exec/memory.h>
#include <exec/io.h>
#include <exec/ports.h>
#include <devices/timer.h>
#include <dos/dos.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <clib/exec_protos.h>
#include <clib/alib_protos.h>
#include <clib/timer_protos.h>
#include <stdio.h>

#define BUFSIZE 8192

struct Library *TimerBase = NULL;  // Needed for timer.device

LONG CountLinesBufferedUnrolled(STRPTR filename)
{
    BPTR fh;
    UBYTE *buffer;
    LONG bytesRead;
    LONG i, lines = 0;

    fh = Open(filename, MODE_OLDFILE);
    if (fh == (BPTR)0)
    {
        printf("Error: Unable to open file.\n");
        return -1;
    }

    buffer = (UBYTE *)AllocMem(BUFSIZE, MEMF_FAST);
    if (buffer == NULL)
    {
        printf("Error: Unable to allocate buffer.\n");
        Close(fh);
        return -1;
    }

    while ((bytesRead = Read(fh, buffer, BUFSIZE)) > 0)
    {
        for (i = 0; i < bytesRead - 1; i += 2)
        {
            if (buffer[i] == '\n') lines++;
            if (buffer[i + 1] == '\n') lines++;
        }

        // Handle the last byte if the file size is odd
        if (i < bytesRead)
        {
            if (buffer[i] == '\n') lines++;
        }
    }

    FreeMem(buffer, BUFSIZE);
    Close(fh);
    return lines;
}

int main(int argc, char **argv)
{
    struct MsgPort *timerPort;
    struct timerequest *timerIO;
    struct TimeVal start, end;
    LONG result;
    ULONG ms;

    if (argc < 2)
    {
        printf("Usage: CountLines <filename>\n");
        return RETURN_FAIL;
    }

    timerPort = CreatePort((UBYTE *)NULL, 0);
    if (timerPort == NULL)
    {
        printf("Error: Failed to create message port.\n");
        return RETURN_FAIL;
    }

    timerIO = (struct timerequest *)CreateExtIO(timerPort, sizeof(struct timerequest));
    if (timerIO == NULL)
    {
        printf("Error: Failed to create I/O request.\n");
        DeletePort(timerPort);
        return RETURN_FAIL;
    }

    if (OpenDevice("timer.device", UNIT_MICROHZ, (struct IORequest *)timerIO, 0) != 0)
    {
        printf("Error: Failed to open timer.device.\n");
        return RETURN_FAIL;
    }

    TimerBase = (struct Library *)timerIO->tr_node.io_Device;

    GetSysTime(&start);

    result = CountLinesBufferedUnrolled((STRPTR)argv[1]);

    GetSysTime(&end);

    if (result < 0)
    {
        CloseDevice((struct IORequest *)timerIO);
        DeleteExtIO((struct IORequest *)timerIO);
        DeletePort(timerPort);
        return RETURN_FAIL;
    }

    SubTime(&end, &start);
    ms = (end.tv_secs * 1000UL) + (end.tv_micro / 1000UL);

    printf("Lines: %ld\n", result);
    printf("Time: %lu ms\n", ms);

    CloseDevice((struct IORequest *)timerIO);
    DeleteExtIO((struct IORequest *)timerIO);
    DeletePort(timerPort);

    return result & 0xFF;
}
