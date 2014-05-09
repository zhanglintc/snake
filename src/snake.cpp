﻿/*
    Main process here.
(C) COPYRIGHT 2014-5-6 zhanglin. Nothing reserved.
*/

#include "snake.h"

int Food::x=0;
int Food::y=0;
char Local_Language[10];

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

    Snake *snake = new Snake(); // new one snake
    Food *food = new Food(snake);  //new a food

    while(snake->getlife()) //while alive
    {
        Sleep(GAME_SPEED); //game speed

        if(snake->node[0].x>RIGHT-2 || snake->node[0].x<LEFT+2 || snake->node[0].y>BOTTOM-1 || snake->node[0].y<TOP+1)
        { //out of bound, die
			snake->setlife(false);
        }
		for(int i=3;i<snake->getsnakelength();i++)
        { //hit itself, die
            if(snake->node[0].x==snake->node[i].x && snake->node[0].y==snake->node[i].y)
            {
                snake->setlife(false);
            }
        }
        if(snake->node[0].x==food->x && snake->node[0].y==food->y)
        { //eat food, ^_^
            snake->eat(food);
            food = new Food(snake); //after eat, generate a new food
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
    SetPos(34,10);
    char str[]="Game Over!";
    cout<<str<<endl;
    getchar();
}

Snake::Snake()
{
    length = 3; //three nodes

    node[0].x = 6; //(RIGHT-LEFT)/2-0;
    node[0].y = (BOTTOM-TOP)/2;
    node[1].x = 4; //(RIGHT-LEFT)/2-2;
    node[1].y = (BOTTOM-TOP)/2;
    node[2].x = 2; //(RIGHT-LEFT)/2-4;
    node[2].y = (BOTTOM-TOP)/2;
	
	if(node[0].x%2!=0) //set snake in even position
	{
		for(int i=0;i<length;i++)
		{
			node[i].x=node[i].x-1;
		}
	}

    derection = CTRL_RIGHT; //head to right
    life = true;            //alive

    draw();          //show this snake
}

void Snake::move()
{
    char head[10]={};
    char body[10]={};
    for(int i=length-1; i>0; i--)
    {
        if(i==length-1)
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
    Equal(Local_Language,"CHS")?memcpy(head,CIRC_CHA_B,sizeof(CIRC_CHA_B)):memcpy(head,CIRC_JPN_B,sizeof(CIRC_JPN_B));
    Equal(Local_Language,"CHS")?memcpy(body,RECT_CHA_B,sizeof(RECT_CHA_B)):memcpy(body,RECT_JPN_B,sizeof(RECT_JPN_B));
    drawOne(node[0].x, node[0].y, head);
    drawOne(node[1].x, node[1].y, body);
}

void Snake::judge()
{
    
}

void Snake::eat(Food *food)
{
    length += 1;
    node[length-1]=node[length-2];
    free(food); //after eat, this food will dispear
}

void Snake::clear()
{
    for(int i=0;i<length; i++)
    {
        drawOne(node[i].x, node[i].y, "  ");
    }
}

Food::Food(Snake *snake) //should be fixed at 2014.05.08, remove static members
{
    char icon[10]={};
    /* get food(x,y) */
    x=random(LEFT+2,RIGHT-2); 
    while(x%2!=0)x=random(LEFT+2,RIGHT-2); //ajust x to even number
    y=random(TOP+1,BOTTOM-1);

    for(int i=0;i<snake->getsnakelength();i++) //for every node of this snake
    {
        if(x==snake->node[i].x && y==snake->node[i].y) //if this food is in one node of this snake
        {
			Food::Food(snake); //new another food
			return; //return to avoid showing it
        }
    }
    Equal(Local_Language,"CHS")?memcpy(icon,STAR_CHA_B,sizeof(STAR_CHA_B)):memcpy(icon,STAR_JPN_B,sizeof(STAR_JPN_B));
    drawOne(x,y,icon); //show this food
}

void Snake::draw()
{
    char head[10]={};
    char body[10]={};
    for(int i=0;i<length; i++)
    {
        Equal(Local_Language,"CHS")?memcpy(head,CIRC_CHA_B,sizeof(CIRC_CHA_B)):memcpy(head,CIRC_JPN_B,sizeof(CIRC_JPN_B));
        Equal(Local_Language,"CHS")?memcpy(body,RECT_CHA_B,sizeof(RECT_CHA_B)):memcpy(body,RECT_JPN_B,sizeof(RECT_JPN_B));
        if(i==0)
        {
			drawOne(node[i].x, node[i].y, head);
        }
        else
        {
            drawOne(node[i].x, node[i].y, body);
        }
    }
}

void Snake::setsnakelength(int length)
{
	this->length = length;
}

int Snake::getsnakelength()
{
    return this->length;
}

void Snake::setlife(bool life)
{
    this->life = life;
}

bool Snake::getlife()
{
    return this->life;
}

void Snake::setderection(int derection)
{
    this->derection = derection;
}

int Snake::getderection()
{
    return this->derection;
}

Node *Snake::getnode()
{
    return this->node;
}