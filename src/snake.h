#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <string>
using namespace std;

#define NODE_MAX		200
#define CTRL_UP			72
#define CTRL_DOWN		80
#define CTRL_RIGHT		77
#define CTRL_LEFT		75
#define CTRL_SPACE		32

class snake
{
public:
	snake();
	void move();
	void move(int);
	void draw();
private:
	int x[NODE_MAX];
	int y[NODE_MAX];
	int derection;
	int node;
	int live;
};

class food
{
public:
	int a;
private:
	int b;
};

typedef struct Frame
{
	COORD position[2];
	int flag;
}Frame;