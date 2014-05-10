/*
    Some frequency use function here.
(C) COPYRIGHT 2014-5-6 zhanglin. Nothing reserved.
*/

#include "snake.h"


void drawOne(int x, int y, char ch[])
{
    SetPos(x, y);
    cout<<ch;
}

void SetPos(COORD position)// set cursor 
{
    HANDLE out=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(out, position);
}

void SetPos(int x, int y)// set cursor
{
    COORD position={x, y};
    SetPos(position);
}

void HideCursor() // hide cursor
{
    CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

//set y, [x1, x2) as ch
void drawRow(int y, int x1, int x2, char ch[]) //zhanglin
{
    SetPos(x1,y);
    for(int i = 0; i <= (x2-x1); i+=2)
        cout<<ch;
}

//when left, right y equal, set [left, right] as ch
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

//set x, [y1, y2] as ch
void drawCol(int x, int y1, int y2, char ch[]) //zhanglin
{
    int y=y1;
    while(y!=y2)
    {
        SetPos(x, y);
        cout<<ch;
        y++;
    }
}

//when left, right x equal, set [left, right] as ch
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

//from lu to rd, set row using row, set col using col
void drawFrame(COORD leftup, COORD  rightdown, char row[], char col[])
{
    drawRow(   leftup.Y, leftup.X, rightdown.X, row);       //first row
    drawRow(rightdown.Y, leftup.X, rightdown.X, row);       //last row
    drawCol(   leftup.X, leftup.Y, rightdown.Y, col);       //first col
    drawCol(rightdown.X, leftup.Y, rightdown.Y, col);       //last col
}

void drawFrame(int x1, int y1, int x2, int y2, char row[], char col[])
{
    COORD leftup={x1, y1};
    COORD rightdown={x2, y2};
    drawFrame(leftup, rightdown, row, col);
}

void drawFrame(Frame frame, char row[], char col[])
{
    COORD leftup = frame.position[0];
    COORD rightdown = frame.position[1];
    drawFrame(leftup, rightdown, row, col);
}

int random(int min, int max)
{
    if(max<min)swap(&min,&max);
	if(min==max)return min;
    else return(rand() % (min-max))+ min;
}

COORD random(COORD leftup, COORD rightdown)
{
    int x=random(leftup.X, rightdown.X);
    int y=random(leftup.Y, rightdown.Y);
    COORD c={x, y};
    return c;
}

void Initialize()
{
	char icon[10]={};
	HideCursor();
    getLocalLanguage();
    if(Equal(Local_Language,"CHS")==false && Equal(Local_Language,"JPN")==false)NotSupport();
    srand((unsigned)time(NULL));        //Random number seed initial

    Equal(Local_Language,"CHS")?memcpy(icon,RECT_CHA_W,sizeof(RECT_CHA_W)):memcpy(icon,RECT_JPN_W,sizeof(RECT_JPN_W));
    drawFrame(LEFT, TOP, RIGHT, BOTTOM, icon, icon);  //outside frame

	memcpy(icon,STAR,sizeof(STAR));
    drawFrame(RIGHT+2, TOP, 78, BOTTOM, icon, icon);  //dashboard frame

    PrintInfo(INFO_INIT);
}

void PrintInfo(unsigned char identifier)
{
    if(isINIT(identifier))
    {
        SetPos(65,3);
        cout<<"Mode: Hard";
    }
    if(isINIT(identifier) || isUPDT(identifier))
    {
        SetPos(65,6);
        cout<<"Score: " << g_score;
        SetPos(65,9);
        cout<<"Eaten: " << g_eaten;
    }
    if(isINIT(identifier))
    {
        SetPos(64,15);
        cout<<"Pause: Space";
        SetPos(65,18);
        cout<<"Exit: ESC";
    }
    if(isINIT(identifier) || isSTAT(identifier))
    {
        SetPos(65,21);
        cout<<"Playing...";
    }
 }

void getLocalLanguage()
{
    FILE *fp;
    char language[10]={};
    char str[]="@echo off && for /f \"tokens=3\" %i in (\'reg query \"HKCU\\Control Panel\\International\" /v \"sLanguage\"\') do (echo %i>lan.txt)";
    system("echo=>lan.txt");
    system(str);
    fp=fopen("lan.txt","r");
    fscanf(fp,"%s",language);
    fclose(fp);
    system("del lan.txt");
    memcpy(Local_Language,language,sizeof(language));
}

void swap(int *a, int *b)
{
    int m;
    m = *a;
    *a = *b;
    *b = m;
}

bool Equal(char command_in[], char command_require[])
{
    if(strcmp(command_in,command_require)==0)return true;
    return false;
}

void NotSupport()
{
    system("cls");
    SetPos(20,10);
    char str[]="The system language is not supported!";
    cout<<str<<endl;
    getchar();
    exit(0);
}