/*
Some frequency use function here.   http://zhanglintc.co
(C) COPYRIGHT 2014-5-6 zhanglin. Nothing reserved.
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
    cout<<ch;
}

/*******************************************************
Function: set cursor's position
Argument: COORD
Return  : void
*******************************************************/
void SetPos(COORD position)// set cursor 
{
    HANDLE out=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(out, position);
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
    CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

/*******************************************************
Function: set y, [x1, x2) as ch
Argument: int, int, int, char[]
Return  : void
*******************************************************/
void drawRow(int y, int x1, int x2, char ch[]) //zhanglin
{
    SetPos(x1,y);
    for(int i = 0; i <= (x2-x1); i+=2)
        cout<<ch;
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
        cout<<ch;
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
Function: initialize, set color, hide cursor, get system language, copy icon data
Argument: none
Return  : void
*******************************************************/
void Initialize()
{
    system("cls");      //clean screen
    system("color 3e"); //set color

    HideCursor();       //hide cursor
    getLocalLanguage(); //get local language information

    //initialize icons
    if(Equal(g_Local_Language,"CHS")==false && Equal(g_Local_Language,"JPN")==false)
    {
        NotSupport();   //if not supported, stop in NotSupport() function
    }
    else if(Equal(g_Local_Language,"CHS")==true)
    {
        g_const_circ_b = CIRC_CHA_B;
        g_const_rect_b = RECT_CHA_B;
        g_const_rect_w = RECT_CHA_W;
        g_const_star_b = STAR_CHA_B;
        g_const_star_w = STAR_CHA_W;
    }
    else if(Equal(g_Local_Language,"JPN")==true)
    {
        g_const_circ_b = CIRC_JPN_B;
        g_const_rect_b = RECT_JPN_B;
        g_const_rect_w = RECT_JPN_W;
        g_const_star_b = STAR_JPN_B;
        g_const_star_w = STAR_JPN_W;
    }

    srand((unsigned)time(NULL));        //Random number seed initial
    read_record(); //debug
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

    system("cls");

    drawFrame(LEFT, TOP, RIGHT, BOTTOM, (char *)g_const_rect_w, (char *)g_const_rect_w);  //outside frame
    drawFrame(RIGHT+2, TOP, 78, BOTTOM, (char *)g_const_star_w, (char *)g_const_star_w);  //dashboard frame

    PrintInfo(INFO_INIT,PLAYING);
}

/*******************************************************
Function: print the informations in the right place(this comment need to be perfected)
Argument: char, char
Return  : void
*******************************************************/
void PrintInfo(char identifier, char stat)
{
    if(isINIT(identifier))
    {
        SetPos(65,3);
        switch(g_difficulty)
        {
            case HARD:
                cout<<"Mode: Hard";
                break;
            case MEDIUM:
                cout<<"Mode: Normal";
                break;
            case EASY:
                cout<<"Mode: Easy";
                break;
        }
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
        cout<<"Exit: Esc";
    }
    if(isINIT(identifier) || isSTAT(identifier))
    {
        if(stat==PLAYING)
        {
            SetPos(65,21);
            cout<<"          ";
        }
        else if(stat==PAUSING)
        {
            SetPos(65,21);
            cout<<"Pausing...";
        }
    }
}

/*******************************************************
Function: display mode choose screen and return the mode info
Argument: none
Return  : int
*******************************************************/
int ChooseMode()
{
    char cursor=0;
    char gotten=0;

    drawFrame(20,7,60,19,(char *)g_const_star_w,(char *)g_const_star_w);
    
    SetPos(32,5);
    cout<<"   Mode Select";
    SetPos(32,10);
    cout<<"Hard mode, fastest";
    SetPos(32,13);
    cout<<"Normal mode, medium";
    SetPos(32,16);
    cout<<"Easy mode, slowest";

    SetPos(29,10);
    cout<<">>";

    cursor=10;
    while(1)
    {
        gotten=0;
        if(_kbhit())
        {
            gotten=_getch();
            switch(gotten)
            {
                case CTRL_UP:
                {
                    if(cursor==16)
                    {
                        cursor=13;
                        SetPos(29,13);
                        cout<<">>";
                        SetPos(29,16);
                        cout<<(char *)ICON_NULL;
                    }
                    else if(cursor==13)
                    {
                        cursor=10;
                        SetPos(29,10);
                        cout<<">>";
                        SetPos(29,13);
                        cout<<(char *)ICON_NULL;
                    }
                    break;
                }
                case CTRL_DOWN:
                {
                    if(cursor==10)
                    {
                        cursor=13;
                        SetPos(29,10);
                        cout<<(char *)ICON_NULL;
                        SetPos(29,13);
                        cout<<">>";
                    }
                    else if(cursor==13)
                    {
                        cursor=16;
                        SetPos(29,13);
                        cout<<(char *)ICON_NULL;
                        SetPos(29,16);
                        cout<<">>";
                    }
                    break;
                }
                case CTRL_SPACE:
                {
                    switch(cursor)
                    {
                        case 10:
                            return HARD;
                        case 13:
                            return MEDIUM;
                        case 16:
                            return EASY;
                    }
                }
                case CTRL_ENTER:
                {
                    switch(cursor)
                    {
                        case 10:
                            return HARD;
                        case 13:
                            return MEDIUM;
                        case 16:
                            return EASY;
                    }
                }
                case CTRL_F5:
                {
                    switch(cursor)
                    {
                        case 10:
                            return HARD;
                        case 13:
                            return MEDIUM;
                        case 16:
                            return EASY;
                    }
                }
            }
        }
    }
}

/*******************************************************
Function: don't you wanna play one more time?
Argument: none
Return  : bool
*******************************************************/
bool replay()
{
    char cursor=0;
    char gotten=0;

    system("cls");

    SetPos(32,3);
    cout<<"Oops, Game Over!!!";
    SetPos(33,5);
    cout<<"Your score: " << g_score;

    drawFrame(20,7,60,18,(char *)g_const_star_w,(char *)g_const_star_w);

    SetPos(32,10);
    cout<<"Just one more time?";
    SetPos(32,15);
    cout<<"No, leave me alone!";

    SetPos(29,10);
    cout<<">>";

    cursor=10;
    while(1)
    {
        gotten=0;
        if(_kbhit())
        {
            gotten=_getch();
            switch(gotten)
            {
                case CTRL_UP:
                {
                    if(cursor==15)
                    {
                        cursor=10;
                        SetPos(29,10);
                        cout<<">>";
                        SetPos(29,15);
                        cout<<(char *)ICON_NULL;
                    }
                    break;
                }
                case CTRL_DOWN:
                {
                    if(cursor=10)
                    {
                        cursor=15;
                        SetPos(29,10);
                        cout<<(char *)ICON_NULL;
                        SetPos(29,15);
                        cout<<">>";
                    }
                    break;
                }
                case CTRL_SPACE:
                {
                    if(cursor==10)return true;
                    else return false;
                }
                case CTRL_ENTER:
                {
                    if(cursor==10)return true;
                    else return false;
                }
                case CTRL_F5:
                {
                    if(cursor==10)return true;
                    else return false;
                }
            }
        }
    }
}

/*******************************************************
Function: read the registry info and get the system language info, set the info to a global variable
Argument: none
Return  : void
*******************************************************/
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
    memcpy(g_Local_Language, language, sizeof(language));
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
Function: show system language not supported screen and exit
Argument: none
Return  : void
*******************************************************/
void NotSupport()
{
    system("cls");
    SetPos(20,10);
    char str[]="The system language is not supported!";
    cout<<str<<endl;
    getchar();
    exit(0);
}

/*******************************************************
Function: judge the two directions is in same line or not
Argument: int, int
Return  : bool
*******************************************************/
bool isSameLine(int direciton1st, int direction2nd)
{
    if(direciton1st==direction2nd || direciton1st+direction2nd==SAME_LINE)
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

/*******************************************************
Function: read record information from files(if no existing files, new one)
Argument: none
Return  : void
*******************************************************/
void read_record()
{
    FILE *fr;       //file pointer
    int  i=0;       //index
    int  isEOF=1;   //any number bigger than 0 is OK

    fr=fopen("snake_record.log","r");   //try to open the log
    if(fr==NULL)                        //if not exist, new one
    {
        fr=fopen("snake_record.log","w");
        for(i=0; i<RANK_NUM_MAX; i++)
        {
            fprintf(fr,"%s %d\n", "anonymous", 0);
        }
    }
    i=0;
    fclose(fr);
    
    fr=fopen("snake_record.log","r");   //open the file
    while(isEOF>0 && i<RANK_NUM_MAX)               //read all lines
    {
        isEOF=fscanf(fr,"%s %d", &g_rank[i].name, &g_rank[i].score);
        i++;
    }
    i=0;
    fclose(fr);
}

/*******************************************************
Function: write record information to files
Argument: none
Return  : void
*******************************************************/
void write_record()
{
    FILE *fw;
	int i=0;

    fw=fopen("snake_record.log","w");
    for(i=0; i<RANK_NUM_MAX; i++)
    {
        fprintf(fw,"%s %d\n", g_rank[i].name, g_rank[i].score);
    }
    fclose(fw);
}

/*******************************************************
Function: show rank information on the screen
Argument: none
Return  : void
*******************************************************/
void show_rank()
{
    int i=0;
    system("cls");

    for(i=0; i<RANK_NUM_MAX; i++)
    {
        SetPos(26,5+i);
        cout<<"No."<<i+1<<":\t"<<g_rank[i].name<<"\t"<<g_rank[i].score;
    }
    getchar();
}

/*******************************************************
Function: check current score is new high score or not. if is high score, record it
Argument: none
Return  : void
*******************************************************/
void highScoreCheck()
{
    int  i=0;
    int  j=0;
    char name[20];

    for(i=0; i<RANK_NUM_MAX; i++)       //from first to last
    {
        if(g_score>=g_rank[i].score)    //if is new high score
        {
            //get player's name
            system("cls");
            SetPos(20,10);
            cout<<"Congratulations! New high score!!!";
            SetPos(20,11);
            cout<<"Enter your name: ";
            gets(name);

            //all item one step backward
            for(j=RANK_NUM_MAX-1; j>i; j--)
            {
                g_rank[j]=g_rank[j-1];
            }

            memcpy(g_rank[i].name, name, sizeof(name)); //set new name
            g_rank[i].score=g_score;                    //set new score
            write_record();                             //write it to log
            show_rank();                                //show it
            break;                                      //break
        }
    }
}