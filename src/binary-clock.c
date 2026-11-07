#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define COLS 6
#define ROWS 4
#define BUFFER_SIZE 128

unsigned int binary_clock_arr[COLS][ROWS] = {0};

char buf[BUFFER_SIZE] = {0};

static volatile sig_atomic_t keep_running = 1;

static void handle_sigint(int sig)
{
    (void)sig;
    keep_running = 0;
}

void update_a_column_in_binary_clock_array(int column, int value)
{
    binary_clock_arr[column][3] = (value >> 0) & 1; // 1
    binary_clock_arr[column][2] = (value >> 1) & 1; // 2
    binary_clock_arr[column][1] = (value >> 2) & 1; // 4
    binary_clock_arr[column][0] = (value >> 3) & 1; // 8
}

void append_to_buffer(char *buffer, size_t buf_size, size_t *wrt_chars, const char *fmt, ...)
{
    if (*wrt_chars >= buf_size)
    {
        return;
    }

    va_list args;
    va_start(args, fmt);
    int res = vsnprintf(buffer + *wrt_chars, buf_size - *wrt_chars, fmt, args);
    va_end(args);

    if (res > 0 && (size_t)res < buf_size - *wrt_chars)
    {
        *wrt_chars += (size_t)res;
    }
    else if (res > 0)
    {
        *wrt_chars = buf_size - 1;
    }
}

void render_binary_clock_to_buffer(int hours, int minutes, int seconds)
{
    size_t wrt_chars = 0;

    append_to_buffer(buf, BUFFER_SIZE, &wrt_chars, "%02d:%02d:%02d\n", hours, minutes, seconds);
    append_to_buffer(buf, BUFFER_SIZE, &wrt_chars, "--------\n");
    append_to_buffer(buf, BUFFER_SIZE, &wrt_chars, "V HHMMSS\n");
    for (unsigned int r = 0; r < ROWS; r++)
    {
        append_to_buffer(buf, BUFFER_SIZE, &wrt_chars, "%d ", (1 << (3 - (int)r)));
        for (int c = 0; c < COLS; c++)
        {
            append_to_buffer(buf, BUFFER_SIZE, &wrt_chars, "%d", binary_clock_arr[c][r]);
        }
        append_to_buffer(buf, BUFFER_SIZE, &wrt_chars, "\n");
    }
}

void update_array_and_render_to_buffer(int hours, int minutes, int seconds)
{
    int clock[COLS] = {hours / 10, hours % 10, minutes / 10, minutes % 10, seconds / 10, seconds % 10};
    for (int i = 0; i < COLS; i++)
    {
        update_a_column_in_binary_clock_array(i, clock[i]);
    }
    render_binary_clock_to_buffer(hours, minutes, seconds);
}

int main(int argc, char *argv[])
{
    // Command option "-c" will run the clock continuous
    int continuous = (argc > 1 && strcmp(argv[1], "-c") == 0) ? 1 : 0;

    // Handle CTRL-C
    signal(SIGINT, handle_sigint);

    // Clear screen and hide cursor
    fputs("\033[?25l", stdout);
    fflush(stdout);

    do
    {
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        update_array_and_render_to_buffer(timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

        // Reposition cursor to top-left and render
        fputs("\033[H", stdout);
        fputs(buf, stdout);
        fflush(stdout);

        if (continuous)
        {
            sleep(1);
        }
    } while (keep_running && continuous);

    // Show the cursor
    fputs("\033[?25h\n", stdout);
    fflush(stdout);

    return 0;
}