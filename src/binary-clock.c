#include <stdio.h>
#include <time.h>

#define COLS 6
#define ROWS 4
#define BUFFER_SIZE ((COLS + 4) * (ROWS + 2))

unsigned int binaryClockArray[COLS][ROWS];

char buf[BUFFER_SIZE];

void updateColumnInArray(int column, int value)
{
    binaryClockArray[column][3] = (value >> 0) & 1;
    binaryClockArray[column][2] = (value >> 1) & 1;
    binaryClockArray[column][1] = (value >> 2) & 1;
    binaryClockArray[column][0] = (value >> 3) & 1;
}

void updateArray(int hours, int minutes, int seconds)
{
    int clock[COLS] = {hours / 10, hours % 10, minutes / 10, minutes % 10, seconds / 10, seconds % 10};
    for (int i = 0; i < COLS; i++)
    {
        updateColumnInArray(i, clock[i]);
    }
}

int printClockArray(void)
{
    size_t written = 0;
    int res = 0;
    res = snprintf(buf + written, sizeof(buf) - written, "V HHMMSS\n");
    written += (res > 0 && res < sizeof(buf) - written) ? (size_t)res : 0;
    
    for (unsigned int r = 0; r < ROWS; r++)
    {
        res = snprintf(buf + written, sizeof(buf) - written, "%d ", (1 << (3 - r)));
        written += (res > 0 && res < sizeof(buf) - written) ? (size_t)res : 0;
        for (int c = 0; c < COLS; c++)
        {
            res = snprintf(buf + written, sizeof(buf) - written, "%d", binaryClockArray[c][r]);
            written += (res > 0 && res < sizeof(buf) - written) ? (size_t)res : 0;
        }
        res = snprintf(buf + written, sizeof(buf) - written, "\n");
        written += (res > 0 && res < sizeof(buf) - written) ? (size_t)res : 0;
    }

    res = snprintf(buf + written, sizeof(buf) - written, "________\n");
    written += (res > 0 && res < sizeof(buf) - written) ? (size_t)res : 0;
    return written;
}

int main(void)
{
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    updateArray(timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    printClockArray();
    printf("%s", buf);

    return 0;
}