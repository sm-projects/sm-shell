#include<stdio.h>

/**
 * Contains all utility functions.
 */
/**
 * <ESC>[{attr};{fg};{bg}m
 * attr} needs to be one of the following:
 * 0    Reset All Attributes (return to normal mode)
 * 1    Bright (usually turns on BOLD)
 * 3    Underline
 * 5    Blink
 * 7    Reverse
 * 8    Hidden
 */
#define   RESET  0
#define   BRIGHT 1
#define   DIM    2
#define   UNDERLINE 3
#define   BLINK  4
#define   REVERSE  7
#define   HIDDEN  8

//{fg} and {bg} needs to be one of the following:
#define BLACK   0
#define RED     1
#define GREEN   2
#define YELLOW  3
#define BLUE    4
#define MAGENTA 5
#define CYAN    6
#define WHITE   7

/**
 * Draw  chars in the terminal, set on attr,bg and fg chosen from above.
 */
void textcolor(int attr, int fg, int bg);


void textcolor(int attr,int bg,int fg){

   char command[24]; //control command sent to the terminal.
   sprintf(command,"%c[%d;%d;%dm",0X1B,attr,fg + 30, bg + 40);
   printf("%s",command);

}
