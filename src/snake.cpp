/*
    Main process here.
(C) COPYRIGHT 2014-5-6 zhanglin. Nothing reserved.
*/

#include "snake.h"

Snake *snake;
Food *food;
int Food::x=0;
int Food::y=0;


int main()
{
    Initialize();
    playing();
    GameOver();
    return 0;
}

void playing()
{
    int counter=0;
    char derection=CTRL_RIGHT;

    snake = new Snake(); // new one snake
    food = new Food();  //new a food

    while(snake->getlive()) //while alive
    {
        Sleep(GAME_SPEED); //game speed

        if(snake->node[0].x>RIGHT-2 || snake->node[0].x<LEFT+2 || snake->node[0].y>BOTTOM-1 || snake->node[0].y<TOP+1)
        { //out of bound, die
			snake->setlive(false);
        }
		for(int i=3;i<snake->getsnakelength();i++)
        { //hit itself, die
            if(snake->node[0].x==snake->node[i].x && snake->node[0].y==snake->node[i].y)
            {
                snake->setlive(false);
            }
        }
        if(snake->node[0].x==food->x && snake->node[0].y==food->y)
        { //eat food, ^_^
            snake->eat();
            free(food);
            food = new Food();
        }
        if(_kbhit())
        {
            derection=_getch(); //get derection
        }
        counter++;
        if(counter==10) //each 10 times, judge move
        {
            if (CTRL_UP == derection || CTRL_DOWN == derection || CTRL_RIGHT == derection || CTRL_LEFT == derection)
            {//up down left right, valid input
                if((snake->getderection()==CTRL_UP && derection==CTRL_DOWN)||
                    (snake->getderection()==CTRL_DOWN && derection==CTRL_UP)||
                    (snake->getderection()==CTRL_LEFT && derection==CTRL_RIGHT)||
                    (snake->getderection()==CTRL_RIGHT && derection==CTRL_LEFT))
                    {
                        ; //converse derection, do nothing
                    }
                else
                    {
                        snake->setderection(derection); //set derection
                    }
            }
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

Snake::Snake()
{
    snake_length = 3; //three nodes

    node[0].x = 6; //(RIGHT-LEFT)/2-0;
    node[0].y = (BOTTOM-TOP)/2;
    node[1].x = 4; //(RIGHT-LEFT)/2-2;
    node[1].y = (BOTTOM-TOP)/2;
    node[2].x = 2; //(RIGHT-LEFT)/2-4;
    node[2].y = (BOTTOM-TOP)/2;
	
	if(node[0].x%2!=0) //set snake in even position
	{
		for(int i=0;i<snake_length;i++)
		{
			node[i].x=node[i].x-1;
		}
	}

    derection = CTRL_RIGHT; //head to right
    live = true;            //alive

    draw();          //show this snake
}

void Snake::move()
{
    for(int i=snake_length-1; i>0; i--)
    {
        if(i==snake_length-1)
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
    drawOne(node[0].x, node[0].y, "¡ñ");
    drawOne(node[1].x, node[1].y, "¡ö");
}

void Snake::judge()
{
    
}

void Snake::eat()
{
    snake_length += 1;
    node[snake_length-1]=node[snake_length-2];
}

void Snake::clear()
{
    for(int i=0;i<snake_length; i++)
    {
        drawOne(node[i].x, node[i].y, "  ");
    }
}

Food::Food()
{
    /* get food(x,y) */
    x=random(LEFT+2,RIGHT-2); 
    while(x%2!=0)x=random(LEFT+2,RIGHT-2); //ajust x to even number
    y=random(TOP+1,BOTTOM-1);

    for(int i=0;i<snake->getsnakelength();i++) //for every node of this snake
    {
        if(x==snake->node[i].x && y==snake->node[i].y) //if this food is in one node of this snake
        {
            Food(); //new another food
			return; //return to avoid showing it
        }
    }
    drawOne(x,y,"¡ï"); //show this food
}

void Snake::draw()
{
    for(int i=0;i<snake_length; i++)
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

void Snake::setsnakelength(int snake_length)
{
	this->snake_length = snake_length;
}

int Snake::getsnakelength()
{
    return snake_length;
}

void Snake::setlive(bool live)
{
    this->live = live;
}

bool Snake::getlive()
{
    return this->live;
}

void Snake::setderection(int derection)
{
    this->derection = derection;
}

int Snake::getderection()
{
    return this->derection;
}