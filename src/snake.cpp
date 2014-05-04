#include "snake.h"

int main()
{
	return 0;
}

void SetPos(COORD a)// set cursor 
{
    HANDLE out=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(out, a);
}

void SetPos(int i, int j)// set cursor
{
    COORD pos={i, j};
    SetPos(pos);
}

void HideCursor()
{
    CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

//把第y行，[x1, x2) 之间的坐标填充为 ch
void drawRow(int y, int x1, int x2, char ch)
{
    SetPos(x1,y);
    for(int i = 0; i <= (x2-x1); i++)
        cout<<ch;
}

//在a, b 纵坐标相同的前提下，把坐标 [a, b] 之间填充为 ch
void drawRow(COORD a, COORD b, char ch)
{
    if(a.Y == b.Y)
        drawRow(a.Y, a.X, b.X, ch);
    else
    {
        SetPos(0, 25);
        cout<<"error code 01：无法填充行，因为两个坐标的纵坐标(x)不相等";
        system("pause");
    }
}

//把第x列，[y1, y2] 之间的坐标填充为 ch
void drawCol(int x, int y1, int y2, char ch)
{
    int y=y1;
    while(y!=y2+1)
    {
        SetPos(x, y);
        cout<<ch;
        y++;
    }
}

//在a, b 横坐标相同的前提下，把坐标 [a, b] 之间填充为 ch
void drawCol(COORD a, COORD b, char ch)
{
    if(a.X == b.X)
        drawCol(a.X, a.Y, b.Y, ch);
    else
    {
        SetPos(0, 25);
        cout<<"error code 02：无法填充列，因为两个坐标的横坐标(y)不相等";
        system("pause");
    }
}

//左上角坐标、右下角坐标、用row填充行、用col填充列
void drawFrame(COORD a, COORD  b, char row, char col)
{
    drawRow(a.Y, a.X+1, b.X-1, row);
    drawRow(b.Y, a.X+1, b.X-1, row);
    drawCol(a.X, a.Y+1, b.Y-1, col);
    drawCol(b.X, a.Y+1, b.Y-1, col);
}

void drawFrame(int x1, int y1, int x2, int y2, char row, char col)
{
    COORD a={x1, y1};
    COORD b={x2, y2};
    drawFrame(a, b, row, col);
}

void drawFrame(Frame frame, char row, char col)
{
    COORD a = frame.position[0];
    COORD b = frame.position[1];
    drawFrame(a, b, row, col);
}

int random(int a, int b)
{
    int c=(rand() % (a-b))+ a;
    return c;
}

//在两个坐标包括的矩形框内随机产生一个坐标
COORD random(COORD leftup, COORD rightdown)
{
    int x=random(leftup.X, rightdown.X);
    int y=random(leftup.Y, rightdown.Y);
    COORD c={x, y};
    return c;
}