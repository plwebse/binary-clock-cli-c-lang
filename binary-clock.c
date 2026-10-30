#include <stdio.h>
#include <time.h>

#if defined(_WIN32)
#include <windows.h>
#define sleep_ms(ms) Sleep(ms)
#else
#include <unistd.h>
#define sleep_ms(ms) usleep((ms) * 1000)
#endif

#define COLS 6
#define ROWS 4
#define BUFFER_SIZE (COLS + 1 * ROWS)
int binaryClockArray[COLS][ROWS];

char buffer[BUFFER_SIZE];

int updateColumnInArray(int column, int value)
{
    if (value % 2 != 0)
    {
        binaryClockArray[column][3] = 1;
    }
    else
    {
        binaryClockArray[column][3] = 0;
    }

    if (value == 2 || value == 3 || value == 6 || value == 7)
    {
        binaryClockArray[column][2] = 1;
    }
    else
    {
        binaryClockArray[column][2] = 0;
    }

    if (value == 4 || value == 5 || value == 6 || value == 7)
    {
        binaryClockArray[column][1] = 1;
    }
    else
    {
        binaryClockArray[column][1] = 0;
    }

    if (value == 8 || value == 9)
    {
        binaryClockArray[column][0] = 1;
    }
    else
    {
        binaryClockArray[column][0] = 0;
    }

    return 0;
}

int updateArray(int hours, int minutes, int seconds)
{
    updateColumnInArray(0, hours / 10);
    updateColumnInArray(1, hours % 10);
    updateColumnInArray(2, minutes / 10);
    updateColumnInArray(3, minutes % 10);
    updateColumnInArray(4, seconds / 10);
    updateColumnInArray(5, seconds % 10);
    return 0;
}

int printClockArray()
{
    unsigned long written = 0;
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            written += snprintf(buffer + written, sizeof(buffer), "%d", binaryClockArray[c][r]);
        }
        written += snprintf(buffer + written, sizeof(buffer), "\n");
    }    

    return written;
}

int main(void)
{

    for (size_t i = 0; i < 20; i++)
    {
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        updateArray(timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        printClockArray();
        printf("%s", buffer);
        printf("%s", "______\n");
        sleep_ms(1000);
    }

    return 0;
}