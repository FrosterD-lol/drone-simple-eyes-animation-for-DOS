#include <stdio.h>
#include <dos.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void print_colored_text(int row, int col, int color, const char* text)
{
    unsigned short far* screen = (unsigned short far*)0xB8000000;
    int offset = row * 80 + col;

    while (*text) 
    {
        screen[offset++] = (*text++ | (color << 8));
    }
}


struct ColorIndex
{
    int color_num;
    char color_name[11];
};

struct ColorIndex colors_index[] = {
    {5, "uzi"},   // Purple 
    {4, "doll"},  // Red
    {7, "white"}, // DOS
    {14, "dd"},    // Yellow (Default)
    {1, "blue"}   // Blue
};

char* eyebrows = "________  ________";

char* eyes[7] = {
  /* 0 */ "   **        **",
  /* 1 */ "  ****      ****",
  /* 2 */ " ******    ******",
  /* 3 */ "********  ********",
  /* 4 */ " ******    ******",
  /* 5 */ "  ****      ****",
  /* 6 */ "   **        **"
};

int start_row = 9;
int total_lines = 7;
int center_col = 30;
int repeats = 5;
int delay_num = 100;
int color = 14;
int moving_eyes = 0;


int main(int argc, char *argv[])
{
    int i;
    int j;
    
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-r") == 0 && i + 1 < argc)
        {
            repeats = atoi(argv[++i]);
        } 
        else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc)
        {
            delay_num = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-m") == 0 && i + 1 < argc)
        {
            int val = atoi(argv[++i]);

            if (val == 0 || val == 1)
            {
                moving_eyes = val;
            }
            else
            {
                printf("Invalid value for -m: must be 0 or 1\n");
                return 1;
            }
        }
        else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc)
        {
            char* input_color = argv[++i];
            bool found = false;
            int ci;
            
            for (ci = 0; ci < sizeof(colors_index); ci++)
            {
                if (strcmp(input_color, colors_index[ci].color_name) == 0)
                {
                    color = colors_index[ci].color_num;
                    found = true;
                }
            }

            if (!found)
            {
                color = 7;
            }
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) 
        {
            printf("Usage: drone.exe [options]\n");
            printf("\n");
            printf("Options:\n");
            printf("-r [number] : Number of repeat loops (e.g. -r 5)\n");
            printf("-d [number] : Delay in milliseconds (e.g. -d 100 [default])\n");
            printf("-c [color]  : Text color of drone eyes (e.g. -c uzi, doll, dd [default], white)\n");
            printf("-m [0|1]    : Enable or disable eye sidemovement (0 = true [default], 1 = false)\n");
            printf("-h, --help  : Show this help message\n");
            return 0;
        }
    }

    while (repeats > 0)
    {
        float r = (rand() % 10 + 1) / 10.0f;
        delay(delay_num / r);

        for (i = 0; i < 5; i++)
        {
            system("cls");
            print_colored_text(start_row + (i - 2), center_col, color, eyebrows);

            for (j = 0 + i; j < 7; j++)
            {
                print_colored_text(start_row + j, center_col, color, eyes[j]);
            }

            delay(delay_num);
        }

        delay(delay_num / r);

        for (i = 4; i >= 0; i--)
        {
            system("cls");
            print_colored_text(start_row + (i - 2), center_col, color, eyebrows);

            for (j = 0 + i; j < 7; j++)
            {
                print_colored_text(start_row + j, center_col, color, eyes[j]);
            }

            delay(delay_num);
        }
        
        if (moving_eyes == 0)
        {
            eyes_sidemoving_anim();
        }

        repeats--;
    }

    return 0;
}

void eyes_sidemoving_anim()
{
    int i;
    int j;

    int side_moving[4] = {-2, 0, 2, 0};

    for (i = 0; i < 4; i++)
    {
        delay(delay_num + delay_num / 1.5f);
        system("cls");
        print_colored_text(start_row - 2, center_col, color, eyebrows);

        for (j = 0; j < 7; j++)
        {
            print_colored_text(start_row + j, center_col + side_moving[i], color, eyes[j]);
        }

        delay(delay_num);
    }
}