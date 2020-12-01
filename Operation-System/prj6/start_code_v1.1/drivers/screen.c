#include "screen.h"
#include "common.h"
#include "string.h"

int screen_cursor_x;
int screen_cursor_y;

/* screen buffer */
char new_screen[SCREEN_HEIGHT * SCREEN_WIDTH] = {0};
char old_screen[SCREEN_HEIGHT * SCREEN_WIDTH] = {0};

/* cursor position */
void vt100_move_cursor(int x, int y)
{
    // \033[y;xH
    printk("%c[%d;%dH", 27, y, x);
}

/* clear screen */
static void vt100_clear()
{
    // \033[2J
    printk("%c[2J", 27);
}

/* hidden cursor */
static void vt100_hidden_cursor()
{
    // \033[?25l
    printk("%c[?25l", 27);
}

// scroll screen range(line1, line2)
static void screen_scroll(int line1, int line2)
{
    int i, j;

    for (i = line1; i <= line2; i++)
    {
        for (j = 0; j < SCREEN_WIDTH; j++)
        {
            old_screen[i * SCREEN_WIDTH + j] = 0;
        }
    }

    for (i = line1; i <= line2; i++)
    {
        for (j = 0; j < SCREEN_WIDTH; j++)
        {
            if (i == line2)
            {
                new_screen[i * SCREEN_WIDTH + j] = ' ';
            }
            else
            {
                new_screen[i * SCREEN_WIDTH + j] = new_screen[(i + 1) * SCREEN_WIDTH + j];
            }
        }
    }
}

/* write a char */
void screen_write_ch(char ch)
{
    if (ch == '\n')
    {
        screen_cursor_x = 0;
        screen_cursor_y++;
    }
    else if (ch == 8) // backspace
    {
        if (screen_cursor_x > 0)
        {
            screen_cursor_x--;
            new_screen[screen_cursor_y * SCREEN_WIDTH + screen_cursor_x] = ' ';
        }
    }
    else if (ch == 13) // enter
    {
        screen_cursor_x = 0;
        screen_cursor_y++;
    }
    else
    {
        new_screen[screen_cursor_y * SCREEN_WIDTH + screen_cursor_x] = ch;
        screen_cursor_x++;
    }

    if (screen_cursor_x < 0)
    {
        screen_cursor_x = 0;
    }

    if (screen_cursor_x >= SCREEN_WIDTH)
    {
        screen_cursor_y++;
        screen_cursor_x = 0;
    }

    if (screen_cursor_y < 0)
    {
        screen_cursor_y = 0;
    }

    if (screen_cursor_y >= SCREEN_HEIGHT)
    {
        screen_scroll(SCREEN_HEIGHT / 2 + 1, SCREEN_HEIGHT - 1);
        screen_cursor_x = 0;
        screen_cursor_y = SCREEN_HEIGHT - 1;
    }
}

void init_screen(void)
{
    vt100_hidden_cursor();
    vt100_clear();
    screen_clear(0, SCREEN_HEIGHT - 1);
}

// clear line1 to line2
void screen_clear(int line1, int line2)
{
    int i, j;

    for (i = line1; i < line2; i++)
    {
        for (j = 0; j < SCREEN_WIDTH; j++)
        {
            new_screen[i * SCREEN_WIDTH + j] = ' ';
            old_screen[i * SCREEN_WIDTH + j] = 0;
        }
    }

    screen_cursor_x = 0;
    screen_cursor_y = 0;
    screen_reflush();
}

void screen_move_cursor(int x, int y)
{
    screen_cursor_x = x;
    screen_cursor_y = y;
}

void screen_write(char *buff)
{
    int i = 0;
    int l = strlen(buff);

    for (i = 0; i < l; i++)
    {
        screen_write_ch(buff[i]);
    }
}

/* 
 * This function is used to print the serial port when the clock 
 * interrupt is triggered. However, we need to pay attention to 
 * the fact that in order to speed up printing, we only refresh 
 * the characters that have been modified since this time.
 */
void screen_reflush(void)
{
    int i, j;

    /* here to reflush screen buffer to serial port */
    for (i = 0; i < SCREEN_HEIGHT; i++)
    {
        for (j = 0; j < SCREEN_WIDTH; j++)
        {
            /* We only print the data of the modified location. */
            if (new_screen[i * SCREEN_WIDTH + j] != old_screen[i * SCREEN_WIDTH + j])
            {
                // -----screen-----
                // | ^ y          |
                // | |            |
                // | |            |
                // |    ---->x    |
                // ----------------
                vt100_move_cursor(j + 1, i + 1);
                port_write_ch(new_screen[i * SCREEN_WIDTH + j]);
                old_screen[i * SCREEN_WIDTH + j] = new_screen[i * SCREEN_WIDTH + j];
            }
        }
    }

    /* recover cursor position */
    vt100_move_cursor(screen_cursor_x, screen_cursor_y);
}
