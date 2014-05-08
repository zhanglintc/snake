/*
    All the defines and declares.
(C) COPYRIGHT 2014-5-6 zhanglin.Nothing reserved.
*/

#ifndef _SNAKE_H_
#define _SNAKE_H_

//inlcude
#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#include <locale.h>
using namespace std;

extern char Local_Language[10];

//define
#define GAME_SPEED		12

#define NODE_MAX		200
#define CTRL_UP			72
#define CTRL_DOWN		80
#define CTRL_RIGHT		77
#define CTRL_LEFT		75
#define CTRL_SPACE		32

#define TOP				0
#define BOTTOM			24	//24
#define LEFT			0
#define RIGHT			78	//78

//const
const char CIRC_CHA_B[]={0xa1,0xf1,0}; //●
const char RECT_CHA_B[]={0xa1,0xf6,0}; //■
const char RECT_CHA_W[]={0xa1,0xf5,0}; //□
const char STAR_CHA_B[]={0xa1,0xef,0}; //★

const char CIRC_JPN_B[]={0x81,0x9c,0}; //●
const char RECT_JPN_B[]={0x81,0xa1,0}; //■
const char RECT_JPN_W[]={0x81,0xa0,0}; //□
const char STAR_JPN_B[]={0x81,0x9a,0}; //★

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

//class
class Snake;
class Food;

class Snake
{
public:
	Node node[NODE_MAX];
	Snake();
	void move();
	void eat(Food *);
	int  getsnakelength();
	void setsnakelength(int);
	bool getlife();
	void setlife(bool);
	int  getderection();
	void setderection(int);
	Node *getnode();
	private:
	bool life;
	int  derection;
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
void playing();
void GameOver();
void getLocalLanguage();
bool Equal(char command_in[], char command_require[]);
void NotSupport();

#endif //_SNAKE_H_