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

//define
#define NODE_MAX		200
#define CTRL_UP			72
#define CTRL_DOWN		80
#define CTRL_RIGHT		77
#define CTRL_LEFT		75
#define CTRL_SPACE		32

#define TOP				0
#define BOTTOM			24
#define LEFT			0
#define RIGHT			78

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
class Snake
{
public:
	int derection;
	Node node[NODE_MAX];
	Snake();
	void move();
	void move(int);
	void judge();
	void eat();
	void draw();
private:
	
	int node_len;
	int live;
	void clear();
	//void draw();
};

class food
{
public:
	food();
private:
	int x;
	int y;
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

#endif //_SNAKE_H_