#include "snake.h"

Snake *snake;
Food *food;

int main()
{
    Initialize();
    drawFrame(LEFT, TOP, RIGHT, BOTTOM, "¡õ", "¡õ");

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
    node[0].x = (RIGHT-LEFT)/2-0;
    node[0].y = (BOTTOM-TOP)/2;
    node[1].x = (RIGHT-LEFT)/2-2;
    node[1].y = (BOTTOM-TOP)/2;
    node[2].x = (RIGHT-LEFT)/2-4;
    node[2].y = (BOTTOM-TOP)/2;
    // node[3].x = (RIGHT-LEFT)/2-6;
    // node[3].y = (BOTTOM-TOP)/2;
    // node[4].x = (RIGHT-LEFT)/2-8;
    // node[4].y = (BOTTOM-TOP)/2;
    // node[5].x = (RIGHT-LEFT)/2-10;
    // node[5].y = (BOTTOM-TOP)/2;
    node_len = 3;
	
	if(node[0].x%2!=0)
	{
		for(int i=0;i<node_len;i++)
		{
			node[i].x=node[i].x-1;
		}
	}

    derection = CTRL_RIGHT;
    live = true;
}

void Snake::move()
{
    //clear();
    for(int i=node_len-1; i>0; i--)
    {
        if(i==node_len-1)
        {
            if(node[i].x==node[i-1].x && node[i].y==node[i-1].y)
            {
                continue;
            }
            drawOne(node[i].x, node[i].y, "  ");
        }
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
    //draw();
    drawOne(node[0].x, node[0].y, "¡ñ");
    drawOne(node[1].x, node[1].y, "¡ö");
}

void Snake::judge()
{
    
}

void Snake::eat()
{
    node_len += 1;
    node[node_len-1]=node[node_len-2];
}

void Snake::clear()
{
    for(int i=0;i<node_len; i++)
    {
        drawOne(node[i].x, node[i].y, "  ");
    }
}

Food::Food()
{
    x=random(LEFT+2,RIGHT-2);
    while(x%2!=0)
    {
        x=random(LEFT+2,RIGHT-2);
    }
    y=random(TOP+1,BOTTOM-1);
    for(int i=0;i<snake->getNodeLen();i++)
    {
        if(x==snake->node[i].x && y==snake->node[i].y)
        {
            Food();
			return;
        }
    }
    drawOne(x,y,"¡ï");
}

void Snake::draw()
{
    for(int i=0;i<node_len; i++)
    {
        if(i==0)
        {
			drawOne(node[i].x, node[i].y, "¡ñ");
        }
        else
        {
            drawOne(node[i].x, node[i].y, "¡ö");
        }
    }
}

int Snake::getNodeLen()
{
    return node_len;
}

void playing()
{
    int counter=0;

    snake = new Snake();
    snake->draw();

    food = new Food();

	// snake->eat();
	// snake->eat();
	// snake->eat();
	// snake->eat();
	// snake->eat();
	// snake->eat();


    while(snake->live == true)
    {
        Sleep(GAME_SPEED);
        if(snake->node[0].x>RIGHT-2 || snake->node[0].x<LEFT+2 || snake->node[0].y>BOTTOM-1 || snake->node[0].y<TOP+1)
        {
            snake->live = false;
        }
        for(int i=3;i<snake->getNodeLen();i++)
        {
            if(snake->node[0].x==snake->node[i].x && snake->node[0].y==snake->node[i].y)
            {
                snake->live = false;
            }
        }
        if(snake->node[0].x==food->x && snake->node[0].y==food->y)
        {
			snake->eat();
            free(food);
            food = new Food();
        }
        if(_kbhit())
        {
            char derection=_getch();
            if (CTRL_UP == derection || CTRL_DOWN == derection || CTRL_RIGHT == derection || CTRL_LEFT == derection)
            {
                if(
                    (snake->derection==CTRL_UP && derection==CTRL_DOWN)||
                    (snake->derection==CTRL_DOWN && derection==CTRL_UP)||
                    (snake->derection==CTRL_LEFT && derection==CTRL_RIGHT)||
                    (snake->derection==CTRL_RIGHT && derection==CTRL_LEFT)
                    );
                else
                    {
                        snake->derection = derection;
                    }
            }
        }
        counter++;
        if(counter==10)
        {
            snake->move();
            counter=0;
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
    int c=(rand() % (min-max))+ min;
    return c;
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
    //_tsetlocale(LC_ALL, L"CHS");        //set locale codepage (Chinese)
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