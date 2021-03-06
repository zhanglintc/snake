﻿/*
All the defines and declares.   http://zhanglintc.co
(C) COPYRIGHT 2014-5-6 zhanglin.Nothing reserved.
*/

#ifndef _SNAKE_H_
#define _SNAKE_H_

//include
#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#include <locale.h>
using namespace std;

//define
#define uchar unsigned char

#define SAME_LINE       152 // Cause CTRL_UP+CTRL_DOWN=152 and CTRL_RIGHT+CTRL_LEFT=152
#define GAME_SPEED      50  // default game speed, will be change in Playing(), not very important
#define SLEEP_TIME      1   // obsoleted use: 12
#define PLAYING         1
#define PAUSING         0
#define EASY            1
#define MEDIUM          2
#define HARD            3

#define NODE_MAX        200
#define RANK_NUM_MAX    9
#define CTRL_UP         72
#define CTRL_DOWN       80
#define CTRL_RIGHT      77
#define CTRL_LEFT       75
#define CTRL_SPACE      32
#define CTRL_ESC        27
#define CTRL_ENTER      13
#define CTRL_F5         63

#define TOP             0
#define BOTTOM          24  //24
#define LEFT            0
#define RIGHT           58  //78

#define INFO_INIT       0x04 //0000 0100
#define INFO_UPDT       0x02 //0000 0010
#define INFO_STAT       0x01 //0000 0001

/****************************************
    3       2       1       0
    |       |       |       |
  mode    score   length  status
*****************************************/
#define GetModeStatus(x)        x>>3 & 1
#define GetScoreStatus(x)       x>>2 & 1
#define GetLenStatus(x)         x>>1 & 1
#define GetStatusStatus(x)      x>>0 & 1

/****************************************
    3       2       1       0
    |       |       |       |
  mode    score   length  status
*****************************************/
#define SetModeStatus(x)        x|=1<<3
#define SetScoreStatus(x)       x|=1<<2
#define SetLenStatus(x)         x|=1<<1
#define SetStatusStatus(x)      x|=1<<0

/****************************************
    2       1       0
    |       |       |
   init   update  status
*****************************************/
#define isINIT(x)               x>>2 & 1
#define isUPDT(x)               x>>1 & 1
#define isSTAT(x)               x>>0 & 1

//const
const uchar ICON_NULL[]={"  "};         //nothing here, means clean one place

const uchar CIRC_CHA_B[]={0xa1,0xf1,0}; //●
const uchar RECT_CHA_B[]={0xa1,0xf6,0}; //■
const uchar RECT_CHA_W[]={0xa1,0xf5,0}; //□
const uchar STAR_CHA_B[]={0xa1,0xef,0}; //★
const uchar STAR_CHA_W[]={0xa3,0xaa,0}; //※

const uchar CIRC_JPN_B[]={0x81,0x9c,0}; //●
const uchar RECT_JPN_B[]={0x81,0xa1,0}; //■
const uchar RECT_JPN_W[]={0x81,0xa0,0}; //□
const uchar STAR_JPN_B[]={0x81,0x9a,0}; //★
const uchar STAR_JPN_W[]={0x81,0x96,0}; //※

//typedef
typedef struct Frame
{
    COORD position[2];
    int flag;
}Frame;

typedef struct Node
{
    int x;
    int y;
}Node;

typedef struct Rank
{
    char name[20];
    int score;
}Rank;

//externs
extern Rank g_rank[];
extern char g_Local_Language[];
extern int  g_difficulty;
extern int  g_score;
extern int  g_mode;
extern int  g_status;
extern int  g_eaten;

extern const uchar *g_const_circ_b;
extern const uchar *g_const_rect_b;
extern const uchar *g_const_rect_w;
extern const uchar *g_const_star_b;
extern const uchar *g_const_star_w;


//class
class Snake;
class Food;

class Snake
{
public:
    Node node[NODE_MAX];
    Snake();
    void move(int);
    void eat(Food *);
    int  getsnakelength();
    void setsnakelength(int);
    bool getlife();
    void setlife(bool);
    int  getdirection();
    void setdirection(int);
    Node *getnode();
private:
    bool life;
    int  direction;
    int  length;
    void clear();
    void draw();
    void judge();
};

class Food
{
public:
    Food(Snake *);
    static int x;
    static int y;
private:
};

//declare
void SetPos(COORD a);
void SetPos(int i, int j);
void HideCursor();
void drawRow(int y, int x1, int x2, char ch[]);
void drawRow(COORD a, COORD b, char ch[]);
void drawCol(int x, int y1, int y2, char ch[]);
void drawCol(COORD a, COORD b, char ch[]);
void drawFrame(COORD a, COORD  b, char row[], char col[]);
void drawFrame(int x1, int y1, int x2, int y2, char row[], char col[]);
void drawFrame(Frame frame, char row[], char col[]);
int random(int a, int b);
COORD random(COORD leftup, COORD rightdown);
void Initialize();
void swap(int *a, int *b);
void drawOne(int x, int y, char ch[]);
void Playing();
void Pause();
void GameOver();
void getLocalLanguage();
bool Equal(char command_in[], char command_require[]);
void NotSupport();
void PrintInfo(char, char);
bool isSameLine(int direciton1st, int direction2nd);
bool isDirection(int input);
bool replay();
int ChooseMode();
void drawGame();
void read_record();
void write_record();
void show_rank();
void highScoreCheck();

#endif //_SNAKE_H_