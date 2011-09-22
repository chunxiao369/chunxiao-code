#include <stdio.h>
#define GOTO_TOP  "\033[1;1H"       /* ESC[1;1H begins output at the top of the terminal (line 1) */
#define GOTO_BOTTOM "\033[100;1H"   /* ESC[1;1H begins output at the bottom of the terminal (actually line 100) */
#define GOTO_BOL  "\033[100D"       /* Go to beginning of line */
#define ERASE_EOL "\033[0K"         /* Erase to end of line */
#define ERASE_EOS "\033[0J"         /* Erase to end screen */
#define ERASE_WIN "\033[2J"         /* Erase the window */
#define REVERSE   "\033[7m"         /* Reverse the display */
#define NORMAL    "\033[0m"         /* Normal display */
#define SCROLL_FULL "\033[1;r"      /* Normal full-screen scrolling for statistics region */
#define SET_SCROLL_REGION(row) printf("\033[%d;r",row)   
                                    /* for command region */
#define CURSOR_ON  "\033[?25h"      /* Turn on cursor */
#define CURSOR_OFF "\033[?25l"      /* Turn off cursor */
#define GOTO_BOTTOM_x "\033[100;%dH"/* go to the bottom of the screen and posion x (supply to printf) */

int main()
{
    printf("Hello World!\n");

    char d = '\r';
    char c = '\n';
    printf("%d, %d.\n", d, c);
    printf("name %.*s\n", 6, "appleleaf");
    /*
    printf(REVERSE"\n");
    printf(ERASE_WIN"\n");
    printf(GOTO_TOP"\n");
    printf("Hello World!\n");
    printf(GOTO_BOTTOM_x, 10);
    */
    return 0;
}

