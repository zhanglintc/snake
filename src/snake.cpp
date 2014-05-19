/*
    Main process here.
(C) COPYRIGHT 2014-5-6 zhanglin. Nothing reserved.
*/

#include "snake.h"

char g_Local_Language[10];
int  g_difficulty=0;
int  g_score=0;
int  g_mode=MEDIUM;
int  g_status=PLAYING;
int  g_eaten=0;
int  Food::x=0;
int  Food::y=0;

int main()
{
    Initialize();
    g_difficulty=ChooseMode();
    do //loop
    {
        drawGame();
        Playing();
    }
    while(replay()==true);

    /*GameOver();*/

    return 0;
}

void Playing()
{
    int  counter=0;             //counter, if this counts appointed times, snake will change direction(if necessary)
    int  GameSpeed=GAME_SPEED;  //game speed
    char direction=0;           //direction the snake will wriggle
    char gotten=0;              //the input from keyboard
    char cache1st=0;            //store the next direction
    char cache2nd=0;            //store the one after next direction(when necessary)

    switch(g_difficulty)
    {
        case HARD:
            GameSpeed=50;
            break;
        case MEDIUM:
            GameSpeed=125;
            break;
        case EASY:
            GameSpeed=200;
            break;
    }

    Snake *snake = new Snake(); // new one snake
    Food  *food = new Food(snake);  //new a food

    while(snake->getlife()) //while alive
    {
        Sleep(SLEEP_TIME); //game speed
        counter++;

        if(_kbhit())
        {
            gotten=_getch(); //get direction(and maybe not direction input)

            if(gotten==-32)continue;
            if(gotten==CTRL_SPACE)Pause();
			if(gotten==CTRL_ESC)exit(0);

            //when cache1st contains data(which means direction) && gotten is not in the same line with cache1st
            if(isDirection(cache1st)==true && isSameLine(gotten,cache1st)==false)
            {
                cache2nd=gotten; //store gotten in cache2nd
            }

            //when gotten is direction && gotten is not in the same line with snake's direction 
            if(isDirection(gotten)==true && isSameLine(gotten,snake->getdirection())==false)
            {
                cache1st=gotten; //store gotten in cache1st
            }
        }
        if(counter>=GameSpeed) //each GameSpeed times, judge move
        {
            counter=1; //reset counter

            //cache2nd -> cache1st -> direction -> snake->move(direction)
            direction=cache1st; //assembly line work
            cache1st=cache2nd;  //assembly line work

            snake->move(direction); //snake wriggling

            //eat food
            if(snake->node[0].x==food->x && snake->node[0].y==food->y)
            {
                snake->eat(food); //^_^
				food = new Food(snake); //after eat, generate a new food

                g_score+=g_difficulty;
                g_eaten+=1;
                PrintInfo(INFO_UPDT, PLAYING);
            }

            //hit walls, die
            if(snake->node[0].x>RIGHT-2 || snake->node[0].x<LEFT+2 || snake->node[0].y>BOTTOM-1 || snake->node[0].y<TOP+1)
            {
                snake->setlife(false); //T_T
            }

            //hit itself, die
            for(int i=3;i<snake->getsnakelength();i++)
            {
                if(snake->node[0].x==snake->node[i].x && snake->node[0].y==snake->node[i].y)
                {
                    snake->setlife(false); //T_T
                }
            }
        }
    }

    //clean food and snake
    free(food);
    free(snake);
}

void Pause()
{
    PrintInfo(INFO_STAT,PAUSING);
    char c=_getch();
    while(c!=CTRL_SPACE)c=_getch();
    PrintInfo(INFO_STAT,PLAYING);
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
    life      = true;       //alive
    direction = CTRL_RIGHT; //head to right
    length    = 3;          //three nodes

    node[0].x = (RIGHT-LEFT)/2-0;
    node[0].y = (BOTTOM-TOP)/2;
    node[1].x = (RIGHT-LEFT)/2-2;
    node[1].y = (BOTTOM-TOP)/2;
    node[2].x = (RIGHT-LEFT)/2-4;
    node[2].y = (BOTTOM-TOP)/2;
	
	if(node[0].x%2!=0) //set snake in even position
	{
		for(int i=0;i<length;i++)
		{
			node[i].x=node[i].x-1;
		}
	}

    draw(); //show this snake
}

void Snake::move(int direction)
{
    char head[10]={};
    char body[10]={};

    //if input is direction keys
    if(isDirection(direction)==true)
    {
        // not same direction      &&    not converse direction, cause CTRL_UP+CTRL_DOWN=152 and CTRL_RIGHT+CTRL_LEFT=152, too
        //if(this->direction!=direction && (this->direction+direction)!=152)
        if(isSameLine(this->direction,direction)==false)
        {
            this->direction=direction; //set direction
        }
    }
    //else this->direction stay the same

    //wriggling
    for(int i=length-1; i>0; i--) //from last body to first body(not contain head)
    {
        if(i==length-1) //if this is the last node
        {
            if(node[i].x==node[i-1].x && node[i].y==node[i-1].y)
            {
                continue; //if last and last-1 is the same, do not clean
            }
            drawOne(node[i].x, node[i].y, "  "); //clean last node
        }
        node[i]=node[i-1]; //wriggle forward
    }

    switch(this->direction) //head move
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
    //end of wriggling
    
    Equal(g_Local_Language,"CHS")?memcpy(head,CIRC_CHA_B,sizeof(CIRC_CHA_B)):memcpy(head,CIRC_JPN_B,sizeof(CIRC_JPN_B));
    Equal(g_Local_Language,"CHS")?memcpy(body,RECT_CHA_B,sizeof(RECT_CHA_B)):memcpy(body,RECT_JPN_B,sizeof(RECT_JPN_B));
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
    free(food); //after eat, this food will disappear
}

void Snake::clear()
{
    for(int i=0;i<length; i++)
    {
        drawOne(node[i].x, node[i].y, "  ");
    }
}

Food::Food(Snake *snake)
{
    char icon[10]={};
    /* get food(x, y) */
    x=random(LEFT+2,RIGHT-2); 
    while(x%2!=0)x=random(LEFT+2,RIGHT-2); //adjust x to even number
    y=random(TOP+1,BOTTOM-1);

    for(int i=0;i<snake->getsnakelength();i++) //for every node of this snake
    {
        if(x==snake->node[i].x && y==snake->node[i].y) //if this food is in one node of this snake
        {
			Food::Food(snake); //new another food
			return; //return to avoid showing it
        }
    }
    Equal(g_Local_Language,"CHS")?memcpy(icon,STAR_CHA_B,sizeof(STAR_CHA_B)):memcpy(icon,STAR_JPN_B,sizeof(STAR_JPN_B));
    drawOne(x,y,icon); //show this food
}

void Snake::draw()
{
    char head[10]={};
    char body[10]={};
    for(int i=0;i<length; i++)
    {
        Equal(g_Local_Language,"CHS")?memcpy(head,CIRC_CHA_B,sizeof(CIRC_CHA_B)):memcpy(head,CIRC_JPN_B,sizeof(CIRC_JPN_B));
        Equal(g_Local_Language,"CHS")?memcpy(body,RECT_CHA_B,sizeof(RECT_CHA_B)):memcpy(body,RECT_JPN_B,sizeof(RECT_JPN_B));
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

void Snake::setdirection(int direction)
{
    this->direction = direction;
}

int Snake::getdirection()
{
    return this->direction;
}

Node *Snake::getnode()
{
    return this->node;
}