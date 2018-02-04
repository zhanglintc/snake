/*
Some frequency use function here.   http://zhanglintc.co
(C) COPYRIGHT 2018.02.04 zhanglintc.
*/

#include "snake.h"

/*******************************************************
Function: display a single icon
Argument: int, int, char[]
Return  : void
*******************************************************/
void drawOne(int x, int y, char ch[])
{
    SetPos(x, y);
    #ifdef _LINUX_MODE_
    addstr(ch);
    refresh();
    #else
    cout<<ch;
    #endif
}

/*******************************************************
Function: set cursor's position
Argument: COORD
Return  : void
*******************************************************/
void SetPos(COORD position)// set cursor 
{
    move(position.Y, position.X);
}

/*******************************************************
Function: set cursor's position
Argument: int, int
Return  : void
*******************************************************/
void SetPos(int x, int y)// set cursor
{
    COORD position={x, y};
    SetPos(position);
}

/*******************************************************
Function: hide consoler's cursor
Argument: none
Return  : void
*******************************************************/
void HideCursor() // hide cursor
{
    #ifndef _LINUX_MODE_
    CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
    #else
    // nothing here
    #endif
}

/*******************************************************
Function: set y, [x1, x2) as ch
Argument: int, int, int, char[]
Return  : void
*******************************************************/
void drawRow(int y, int x1, int x2, char ch[]) //zhanglin
{
    for(int i = 0; i <= (x2-x1); i+=2)
    {
        SetPos(i,y);
        addstr(ch);
    }
    refresh();
}

/*******************************************************
Function: when left, right y equal, set [left, right] as ch
Argument: COORD, COORD, char[]
Return  : void
*******************************************************/
void drawRow(COORD left, COORD right, char ch[]) //zhanglin
{
    if(left.Y == right.Y)
        drawRow(left.Y, left.X, right.X, ch);
    else
    {
        SetPos(0, 25);
        cout<<"error code 01: \'x\' not equaled";
        system("pause");
    }
}

/*******************************************************
Function: set x, [y1, y2] as ch
Argument: int, int, int, char[]
Return  : void
*******************************************************/
void drawCol(int x, int y1, int y2, char ch[]) //zhanglin
{
    int y=y1;
    while(y!=y2)
    {
        SetPos(x, y);
        #ifdef _LINUX_MODE_
        addstr(ch);
        refresh();
        #else
        cout<<ch;
        #endif
        y++;
    }
}

/*******************************************************
Function: when left, right x equal, set [left, right] as ch
Argument: COORD, COORD, char[]
Return  : void
*******************************************************/
void drawCol(COORD left, COORD right, char ch[]) //zhanglin
{
    if(left.X == right.X)
        drawCol(left.X, left.Y, right.Y, ch);
    else
    {
        SetPos(0, 25);
        cout<<"error code 02: \'y\' not equaled";
        system("pause");
    }
}

/*******************************************************
Function: from left-up to right-down, set row using row, set col using col
Argument: COORD, COORD, char[]
Return  : void
*******************************************************/
void drawFrame(COORD leftup, COORD  rightdown, char row[], char col[])
{
    drawRow(   leftup.Y, leftup.X, rightdown.X, row);       //first row
    drawRow(rightdown.Y, leftup.X, rightdown.X, row);       //last row
    drawCol(   leftup.X, leftup.Y, rightdown.Y, col);       //first col
    drawCol(rightdown.X, leftup.Y, rightdown.Y, col);       //last col
}

/*******************************************************
Function: draw a rectangle frame
Argument: int, int, int, int, char[], char[]
Return  : void
*******************************************************/
void drawFrame(int x1, int y1, int x2, int y2, char row[], char col[])
{
    COORD leftup={x1, y1};
    COORD rightdown={x2, y2};
    drawFrame(leftup, rightdown, row, col);
}

/*******************************************************
Function: draw a rectangle frame
Argument: Frame, char[], char[]
Return  : void
*******************************************************/
void drawFrame(Frame frame, char row[], char col[])
{
    COORD leftup = frame.position[0];
    COORD rightdown = frame.position[1];
    drawFrame(leftup, rightdown, row, col);
}

/*******************************************************
Function: generate a random number
Argument: int, int
Return  : int
*******************************************************/
int random(int min, int max)
{
    if(max<min)swap(&min,&max);
    if(min==max)return min;
    else return(rand() % (min-max))+ min;
}

/*******************************************************
Function: generate a random position
Argument: COORD, COORD
Return  : COORD
*******************************************************/
COORD random(COORD leftup, COORD rightdown)
{
    int x=random(leftup.X, rightdown.X);
    int y=random(leftup.Y, rightdown.Y);
    COORD c={x, y};
    return c;
}

/*******************************************************
Function: snake hit wall?
Argument: Node
Return  : bool
*******************************************************/
bool hitWall(Node head)
{
    return head.x > RIGHT-2 || head.x < LEFT+2 || head.y > BOTTOM-1 || head.y < TOP+1;
}

/*******************************************************
Function: snake hit it's body?
Argument: Node, Snake*
Return  : bool
*******************************************************/
bool hitBody(Node head, Snake *snake)
{
    bool hit = false;

    for(int i=3; i < snake->getsnakelength(); i++)
    {
        if(head.x == snake->node[i].x && head.y == snake->node[i].y)
        {
            hit = true;
        }
    }

    return hit;
}

/*******************************************************
Function: initialize, set color, hide cursor, get system language, copy icon data
Argument: none
Return  : void
*******************************************************/
void Initialize()
{
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    nonl();
    noecho();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    curs_set(0);
    refresh();

    g_const_circ_b = (const unsigned char *)"●";
    g_const_rect_b = (const unsigned char *)"■";
    g_const_rect_w = (const unsigned char *)"□";
    g_const_star_b = (const unsigned char *)"★";
    g_const_star_w = (const unsigned char *)"※";
}

/*******************************************************
Function: draw the game frame and relevant informations
Argument: none
Return  : void
*******************************************************/
void drawGame()
{
    g_score=0;
    g_eaten=0;

    drawFrame(LEFT, TOP, RIGHT, BOTTOM, (char *)g_const_rect_w, (char *)g_const_rect_w);  //outside frame
    // drawFrame(RIGHT+2, TOP, 78, BOTTOM, (char *)g_const_star_w, (char *)g_const_star_w);  //dashboard frame
}

/*******************************************************
Function: swap two numbers
Argument: int, int
Return  : void
*******************************************************/
void swap(int *a, int *b)
{
    int m;
    m = *a;
    *a = *b;
    *b = m;
}

/*******************************************************
Function: compare two strings, true for equal, false for not equal
Argument: char[], char[]
Return  : bool
*******************************************************/
bool Equal(char command_in[], char command_require[])
{
    if(strcmp(command_in,command_require)==0)return true;
    return false;
}

/*******************************************************
Function: judge the two directions is in same line or not
Argument: int, int
Return  : bool
*******************************************************/
bool isSameLine(int direciton1st, int direction2nd)
{
    if(direciton1st==direction2nd || direciton1st+direction2nd==OPPOSITE_DIRECT)
    {
        return true;
    }
    return false;
}

/*******************************************************
Function: judge the input is one of the valid directions or not
Argument: int
Return  : bool
*******************************************************/
bool isDirection(int input)
{
    if((input==CTRL_UP || input==CTRL_DOWN || input==CTRL_RIGHT || input==CTRL_LEFT))
    {
        return true;
    }
    return false;
}
