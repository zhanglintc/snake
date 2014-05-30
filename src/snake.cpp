/*
Main process here.  http://zhanglintc.co
(C) COPYRIGHT 2014-5-6 zhanglin. Nothing reserved.

*/

#include "snake.h"

//these variables are assigned in funtction Initialize()
uchar const *g_const_circ_b;    //global ●
uchar const *g_const_rect_b;    //global ■
uchar const *g_const_rect_w;    //global □
uchar const *g_const_star_b;    //global ★
uchar const *g_const_star_w;    //global ※

//global variable declares
char g_Local_Language[10];      //global language info
int  g_difficulty=0;            //global difficulty
int  g_score=0;                 //global score
int  g_mode=MEDIUM;             //global mode
int  g_status=PLAYING;          //global status
int  g_eaten=0;                 //global eaten

//food info (plan to make it local)
int  Food::x=0;
int  Food::y=0;

/*******************************************************
Function: main entrance
Argument: none
Return  : int
*******************************************************/
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

/*******************************************************
Function: food, snake, control, move
Argument: none
Return  : void
*******************************************************/
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
            GameSpeed=100;
            break;
        case EASY:
            GameSpeed=200;
            break;
    }

    Snake *snake = new Snake();     // new one snake
    Food  *food = new Food(snake);  //new a food

    while(snake->getlife()) //while alive, process the sanke move, snake eat things. inportant block
    {
        Sleep(SLEEP_TIME);  //game speed
        counter++;

        //debug print
        //SetPos(74,23);
        //cout<<counter;
        if(_kbhit())
        {
            gotten=_getch();        //get direction(but can be not direction input)

            if(gotten==-32)continue;
            if(gotten==CTRL_SPACE || gotten==CTRL_ENTER)Pause();
			if(gotten==CTRL_ESC)exit(0);

            //when cache1st contains data(which means direction) && gotten is not in the same line with cache1st
            if(isDirection(cache1st)==true && isSameLine(gotten,cache1st)==false)
            {
                cache2nd=gotten;    //store gotten in cache2nd
            }

            //when gotten is direction && gotten is not in the same line with snake's direction 
            if(isDirection(gotten)==true && isSameLine(gotten,snake->getdirection())==false)
            {
                cache1st=gotten;    //store gotten in cache1st
            }
        }
        if(counter>=GameSpeed)      //each GameSpeed times, judge move
        {
            counter=1;  //reset counter

            //cache2nd -> cache1st -> direction -> snake->move(direction)
            direction=cache1st;     //assembly line work
            cache1st=cache2nd;      //assembly line work

            snake->move(direction); //snake wriggling

            //eat food
            if(snake->node[0].x==food->x && snake->node[0].y==food->y)
            {
                snake->eat(food);       //^_^
				food = new Food(snake); //after eat, generate a new food

                g_score+=g_difficulty;  //difficulty means point
                g_eaten+=1;
                PrintInfo(INFO_UPDT, PLAYING);
            }

            //hit walls, die (to be or not to be...?)
            if(snake->node[0].x>RIGHT-2 || snake->node[0].x<LEFT+2 || snake->node[0].y>BOTTOM-1 || snake->node[0].y<TOP+1)
            {
                snake->setlife(false);      //T_T
            }

            //hit itself, die (to be or not to be...?)
            for(int i=3;i<snake->getsnakelength();i++)
            {
                if(snake->node[0].x==snake->node[i].x && snake->node[0].y==snake->node[i].y)
                {
                    snake->setlife(false);  //T_T
                }
            }
        }
    }

    //clean food and snake
    free(food);
    free(snake);
}

/*******************************************************
Function: pause the game
Argument: none
Return  : void
*******************************************************/
void Pause()
{
    PrintInfo(INFO_STAT,PAUSING);
    char c=_getch();
    while(c!=CTRL_SPACE && c!=CTRL_ENTER)c=_getch();
    PrintInfo(INFO_STAT,PLAYING);
}

/*******************************************************
Function: game over screen(unused)
Argument: none
Return  : void
*******************************************************/
void GameOver()
{
    system("cls");
    SetPos(34,10);
    char str[]="Game Over!";
    cout<<str<<endl;
    getchar();
}

/*******************************************************
Function: constructor of snake, new a three node length snake
Argument: none
Return  : none
*******************************************************/
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
	
	if(node[0].x%2!=0)      //set snake in even position
	{
		for(int i=0;i<length;i++)
		{
			node[i].x=node[i].x-1;
		}
	}

    draw();                 //show this snake
}

/*******************************************************
Function: move and show a snake
Argument: int
Return  : void
*******************************************************/
void Snake::move(int direction)
{
    //if input is direction keys
    if(isDirection(direction)==true)
    {
        // not same direction      &&    not converse direction, cause CTRL_UP+CTRL_DOWN=152 and CTRL_RIGHT+CTRL_LEFT=152, too
        //if(this->direction != direction && (this->direction + direction) != 152)
        if(isSameLine(this->direction,direction)==false)
        {
            this->direction=direction; //set direction
        }
    }
    //else this->direction stay the same

    //wriggling
    for(int i=length-1; i>0; i--)   //from last body to first body(not contain head)
    {
        if(i==length-1)             //if this is the last node
        {
            if(node[i].x==node[i-1].x && node[i].y==node[i-1].y)
            {
                continue;           //if last and last-1 is the same, do not clean
            }
            drawOne(node[i].x, node[i].y, (char *)ICON_NULL); //clean last node
        }
        node[i]=node[i-1];          //wriggle forward
    }

    switch(this->direction)         //head move
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
    
    drawOne(node[0].x, node[0].y, (char *)g_const_circ_b); //head
    drawOne(node[1].x, node[1].y, (char *)g_const_rect_b); //body
}

/*******************************************************
Function: 
Argument: 
Return  : 
*******************************************************/
void Snake::judge()
{
    
}

/*******************************************************
Function: snake eat a food
Argument: Food
Return  : void
*******************************************************/
void Snake::eat(Food *food)
{
    length += 1;                    //become longer
    node[length-1]=node[length-2];  //set the new node in the same place of last one
    free(food);                     //after eat, this food will disappear
}

/*******************************************************
Function: undisplay a snake
Argument: none
Return  : void
*******************************************************/
void Snake::clear()
{
    for(int i=0;i<length; i++)
    {
        drawOne(node[i].x, node[i].y, (char *)ICON_NULL);
    }
}

/*******************************************************
Function: 
    new a food, this function need a argument Snake 
    to determine which place could be used
Argument: 
    Snake
Return: 
    none
*******************************************************/
Food::Food(Snake *snake)
{
    /* get food(x, y) */
    x=random(LEFT+2,RIGHT-2); 
    while(x%2!=0)x=random(LEFT+2,RIGHT-2);              //adjust x to even number
    y=random(TOP+1,BOTTOM-1);

    for(int i=0;i<snake->getsnakelength();i++)          //for every node of this snake
    {
        if(x==snake->node[i].x && y==snake->node[i].y)  //if this food is in one node of this snake
        {
			Food::Food(snake);                          //new another food
			return;                                     //return to avoid showing it
        }
    }
    drawOne(x,y,(char *)g_const_star_b); //show this food
}

/*******************************************************
Function: show a snake
Argument: none
Return  : void
*******************************************************/
void Snake::draw()
{
    for(int i=0;i<length; i++)
    {
        if(i==0)
        {
			drawOne(node[i].x, node[i].y, (char *)g_const_circ_b); //head
        }
        else
        {
            drawOne(node[i].x, node[i].y, (char *)g_const_rect_b); //body
        }
    }
}

/*******************************************************
Function: set snake's length
Argument: int
Return  : void
*******************************************************/
void Snake::setsnakelength(int length)
{
	this->length = length;
}

/*******************************************************
Function: get snake's length
Argument: none
Return  : int
*******************************************************/
int Snake::getsnakelength()
{
    return this->length;
}

/*******************************************************
Function: set snake's life
Argument: bool
Return  : void
*******************************************************/
void Snake::setlife(bool life)
{
    this->life = life;
}

/*******************************************************
Function: get snake's life
Argument: none
Return  : bool
*******************************************************/
bool Snake::getlife()
{
    return this->life;
}

/*******************************************************
Function: set snake's direction
Argument: int
Return  : void
*******************************************************/
void Snake::setdirection(int direction)
{
    this->direction = direction;
}

/*******************************************************
Function: get snake's direction
Argument: none
Return  : int
*******************************************************/
int Snake::getdirection()
{
    return this->direction;
}

/*******************************************************
Function: get snake's node info(unused)
Argument: none
Return  : Node
*******************************************************/
Node *Snake::getnode()
{
    return this->node;
}