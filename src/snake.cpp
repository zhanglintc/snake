#include "snake.h"

int main()
{

    Initialize();
    drawFrame(LEFT, TOP, RIGHT, BOTTOM, "��", "��");

    playing();

    GameOver();
    return 0;
}

void drawOne(int x, int y, char ch[])
{
    SetPos(x, y);
    cout<<ch;
}

Snake::Snake()
{
    node[0].x=40;
    node[0].y=12;
    node[1].x=38;
    node[1].y=12;
    node[2].x=36;
    node[2].y=12;
    node_len=3;
    derection=CTRL_RIGHT;
}

void Snake::move()
{
    clear();
    for(int i=node_len-1; i>0; i--)
    {
        node[i]=node[i-1];
    }
    switch(derection)
    {
        case CTRL_UP:
            node[0].y-=1;
            break;
        case CTRL_DOWN:
            node[0].y+=1;
            break;
        case CTRL_LEFT:
            node[0].x-=2;
            break;
        case CTRL_RIGHT:
            node[0].x+=2;
            break;
    }
    draw();
}

void Snake::judge()
{
    
}

void Snake::clear()
{
    for(int i=0;i<node_len; i++)
    {
        drawOne(node[i].x, node[i].y, "  ");
    }
}

void Snake::draw()
{
    for(int i=0;i<node_len; i++)
    {
        if(i==0)
        {
			drawOne(node[i].x, node[i].y, "��");
        }
        else
        {
            drawOne(node[i].x, node[i].y, "��");
        }
    }
}

void playing()
{
    int i=0;

    Snake *snake=new Snake();
    snake->draw();

    while(true)
    {
        Sleep(4);
        if(snake->node[0].x>RIGHT-2 || snake->node[0].x<LEFT+2 || snake->node[0].y>BOTTOM-1 || snake->node[0].y<TOP+1)
        {
            break;
        }
        if(_kbhit())
        {
            char derection=_getch();
            if (CTRL_UP == derection || CTRL_DOWN == derection || CTRL_RIGHT == derection || CTRL_LEFT == derection)
            {
                snake->derection = derection;
            }
        }
        i++;
        if(i==10)
        {
            snake->move();
            i=0;
        }
    }
}

void GameOver()
{
    system("cls");
    SetPos(36,12);
    char str[]="Game Over!";
	cout<<str<<endl;
	getchar();
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

//�ѵ�y�У�[x1, x2) ֮����������Ϊ ch
void drawRow(int y, int x1, int x2, char ch[]) //zhanglin
{
    SetPos(x1,y);
    for(int i = 0; i <= (x2-x1); i+=2)
        cout<<ch;
}

//�� left, right ��������ͬ��ǰ���£������� [left, right] ֮�����Ϊ ch
void drawRow(COORD left, COORD right, char ch[]) //zhanglin
{
    if(left.Y == right.Y)
        drawRow(left.Y, left.X, right.X, ch);
    else
    {
        SetPos(0, 25);
        cout<<"error code 01���޷�����У���Ϊ���������������(x)�����";
        system("pause");
    }
}

//�ѵ�x�У�[y1, y2] ֮����������Ϊ ch
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

//�� left, right ��������ͬ��ǰ���£������� [left, right] ֮�����Ϊ ch
void drawCol(COORD left, COORD right, char ch[]) //zhanglin
{
    if(left.X == right.X)
        drawCol(left.X, left.Y, right.Y, ch);
    else
    {
        SetPos(0, 25);
        cout<<"error code 02���޷�����У���Ϊ��������ĺ�����(y)�����";
        system("pause");
    }
}

//���Ͻ����ꡢ���½����ꡢ��row����С���col�����
void drawFrame(COORD leftup, COORD  rightdown, char row[], char col[])
{
    drawRow(   leftup.Y, leftup.X, rightdown.X, row);       //��ʼ��
    drawRow(rightdown.Y, leftup.X, rightdown.X, row);       //�����
    drawCol(   leftup.X, leftup.Y, rightdown.Y, col);       //��ʼ��
    drawCol(rightdown.X, leftup.Y, rightdown.Y, col);       //�����
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
    int c=(rand() % (min-max))+ min;
    return c;
}

//��������������ľ��ο����������һ������
COORD random(COORD leftup, COORD rightdown)
{
    int x=random(leftup.X, rightdown.X);
    int y=random(leftup.Y, rightdown.Y);
    COORD c={x, y};
    return c;
}

void Initialize()
{
    _tsetlocale(LC_ALL, L"CHS");        //set locale codepage (Chinese)
    srand((unsigned)time(NULL));        //Random number seed initial, 2014.03.16 zhanglin
    HideCursor();
}

void swap(int *a, int *b)
{
    int m;
    m = *a;
    *a = *b;
    *b = m;
}