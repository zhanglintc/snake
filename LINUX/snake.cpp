/*
Main process here.  http://zhanglintc.co
(C) COPYRIGHT 2018.02.04 zhanglintc.
*/

#include "snake.h"

// these variables are assigned in function Initialize()
uchar const *g_const_circ_b;    // global ●
uchar const *g_const_rect_b;    // global ■
uchar const *g_const_rect_w;    // global □
uchar const *g_const_star_b;    // global ★
uchar const *g_const_star_w;    // global ※

// global variable declares
Rank    g_rank[RANK_NUM_MAX];        // global rank info
char    g_Local_Language[10];        // global language info
int     g_difficulty = 0;            // global difficulty
int     g_score = 0;                 // global score
int     g_mode = MEDIUM;             // global mode
int     g_status = PLAYING;          // global status
int     g_eaten = 0;                 // global eaten

// food info (plan to make it local)
int  Food::x = 0;
int  Food::y = 0;

/*******************************************************
Function: constructor of snake, new a three node length snake
Argument: none
Return  : none
*******************************************************/
Snake::Snake()
{
    this->life      = true;               // alive
    this->direction = CTRL_RIGHT;         // head to right
    this->length    = 3;                  // three nodes
    this->head      = &(this->body[0]);   // head
    this->init_brain();

    this->body[0].x = (BOARD_RIGHT - BOARD_LEFT) / 2 - 0;
    this->body[0].y = (BOARD_BOTTOM - BOARD_TOP) / 2;
    this->body[1].x = (BOARD_RIGHT - BOARD_LEFT) / 2 - 1;
    this->body[1].y = (BOARD_BOTTOM - BOARD_TOP) / 2;
    this->body[2].x = (BOARD_RIGHT - BOARD_LEFT) / 2 - 2;
    this->body[2].y = (BOARD_BOTTOM - BOARD_TOP) / 2;

    this->save_to_brain();

    this->initialDraw();                 // show this snake
}

void Snake::init_brain()
{
    for (int i = 0; i < BOARD_RIGHT * BOARD_BOTTOM; i++)
    {
        this->brain[i].board_type = BOARD_TYPE_NONE;
        this->brain[i].distance = -1;
        this->brain[i].pos = this->body[i];
    }
}

void Snake::save_to_brain()
{
    for (int i = 0; i < this->length; i++)
    {
        int sn = twoD2oneD(this->body[i]);
        this->brain[sn].board_type = BOARD_TYPE_SNAKE;
        this->brain[sn].pos = this->body[i];
    }
}

template <typename T>
bool Snake::way_to_head(T *T_node)
{
    Node food;
    food.x = T_node->x;
    food.y = T_node->y;

    int board_size = BOARD_RIGHT * BOARD_BOTTOM;
    int sn = twoD2oneD(food);

    Node left(-1, 0);
    Node up(0, -1);
    Node right(1, 0);
    Node down(0, 1);

    Node node;
    Node move[4] = {left, up, right, down};

    map<string, int> visited;
    queue<Node> bfs;
    bfs.push(food);

    while (bfs.size())
    {
        node = bfs.front(); bfs.pop();
        if (visited.count(node.to_s()))
        {
            continue;
        }
        visited[node.to_s()] = 1;

        for (int i = 0; i < 4; i++)
        {
            Node direction = move[i];
            Node after_move = node + direction;
            if (hitWall(after_move) || hitBody(after_move, this))
            {
                continue;
            }
            if (after_move == this->body[0])
            {
                return true;
            }
            if (!visited.count(after_move.to_s()))
            {
                bfs.push(after_move);
            }
        }
    }
    return false;
}

/*******************************************************
Function: move and show a snake
Argument: int
Return  : void
*******************************************************/
void Snake::move(int direction)
{
    // if input is direction keys && not in the same line
    if (isDirection(direction) == true && isSameLine(this->direction, direction) == false)
    {
        this->direction = direction; // set direction
    }
    // else this->direction stay the same

    // wriggling
    for(int i = length - 1; i > 0; i--)   // from last body to first body(not contain head)
    {
        if (i == length - 1)             // if this is the last body
        {
            if (this->body[i].x == this->body[i-1].x && this->body[i].y == this->body[i-1].y)
            {
                continue;           // if last and last-1 is the same, do not clean
            }
            drawOne(this->body[i].x, this->body[i].y, (char *)ICON_NULL); // clean last body
        }
        this->body[i] = this->body[i-1];          // wriggle forward
    }

    switch(this->direction)         // head move
    {
        case CTRL_UP:
            this->body[0].y -= 1;
            break;
        case CTRL_DOWN:
            this->body[0].y += 1;
            break;
        case CTRL_LEFT:
            this->body[0].x -= 1;
            break;
        case CTRL_RIGHT:
            this->body[0].x += 1;
            break;
    }
    // end of wriggling

    drawOne(this->body[0].x, this->body[0].y, (char *)g_const_circ_b); // head
    drawOne(this->body[1].x, this->body[1].y, (char *)g_const_rect_b); // body
}

/*******************************************************
Function: auto play
Argument: none
Return  : void
*******************************************************/
int Snake::Control_AI(Food *food, Snake *snake)
{
    typedef struct Pair1
    {
        int direction;
        int distance;
    }Pair1;

    int direction   = CTRL_RIGHT;
    int triedTimes  = 0;

    Node    head = this->body[0];
    Pair1   left, right, up, down;

    left.direction  = CTRL_LEFT;
    left.distance   = abs( (head.x - 1) - food->x ) + abs( head.y - food->y );

    right.direction = CTRL_RIGHT;
    right.distance  = abs( (head.x + 1) - food->x ) + abs( head.y - food->y );

    up.direction    = CTRL_UP;
    up.distance = abs( head.x - food->x ) + abs( (head.y - 1) - food->y );

    down.direction  = CTRL_DOWN;
    down.distance   = abs( head.x - food->x ) + abs( (head.y + 1) - food->y );

    Pair1 list[] = { left, right, up, down };

    for ( int i = 0; i <= 3; i++ )
    {
        for ( int j = i + 1; j <= 3; j++ )
        {
            if ( list[i].distance > list[j].distance )
            {
                Pair1 tmp;
                tmp = list[i];
                list[i] = list[j];
                list[j] = tmp;
            }
        }
    }

    direction = list[0].direction;
    if (direction + this->direction == OPPOSITE_DIRECT)
    {
        direction = list[1].direction;
    }

    // for (int i = 0; i < 4; i++)
    // {
    //     direction = list[i].direction;
    //     if (direction + this->direction == OPPOSITE_DIRECT)
    //     {
    //         continue;
    //     }
    // }

    // while (fakeMove lead to snake die || direction AND this->direction are opposite)
    while (!fakeMove(direction) || direction + this->direction == OPPOSITE_DIRECT)
    {
        if (triedTimes >= 4)
        {
            break;
        }

        triedTimes += 1;

        int choice1, choice2;
        switch(direction)
        {
            case CTRL_UP:
                // direction = CTRL_RIGHT;
                choice1 = snake->survivalProbability(CTRL_LEFT);
                choice2 = snake->survivalProbability(CTRL_RIGHT);
                direction = choice1 >= choice2 ? CTRL_LEFT : CTRL_RIGHT;
                break;
            case CTRL_DOWN:
                // direction = CTRL_LEFT;
                choice1 = snake->survivalProbability(CTRL_LEFT);
                choice2 = snake->survivalProbability(CTRL_RIGHT);
                direction = choice1 >= choice2 ? CTRL_LEFT : CTRL_RIGHT;
                break;
            case CTRL_LEFT:
                // direction = CTRL_UP;
                choice1 = snake->survivalProbability(CTRL_UP);
                choice2 = snake->survivalProbability(CTRL_DOWN);
                direction = choice1 >= choice2 ? CTRL_UP : CTRL_DOWN;
                break;
            case CTRL_RIGHT:
                // direction = CTRL_DOWN;
                choice1 = snake->survivalProbability(CTRL_UP);
                choice2 = snake->survivalProbability(CTRL_DOWN);
                direction = choice1 >= choice2 ? CTRL_UP : CTRL_DOWN;
                break;
        }
    }

    return direction;
}

/*******************************************************
Function: calculate survival probability of this direction
Argument: int
Return  : int
*******************************************************/
int Snake::survivalProbability(int direction)
{
    Node fakeHead = *this->head;
    int  prob = 0;      // distance to body or wall, 1000 bonus if wall
    int  type = 0;      // 0 for hit body, 1 for hit wall
    bool life = true;

    while (life)
    {
        // hit walls, die (to be or not to be...?)
        if (hitWall(fakeHead))
        {
            type = 1;
            life = false;
            break;
        }

        // hit itself, die (to be or not to be...?)
        if (hitBody(fakeHead, this))
        {
            type = 0;
            life = false;
            break;
        }

        // move fakeHead
        switch(direction)
        {
            case CTRL_UP:
                fakeHead.y -= 1;
                break;
            case CTRL_DOWN:
                fakeHead.y += 1;
                break;
            case CTRL_LEFT:
                fakeHead.x -= 1;
                break;
            case CTRL_RIGHT:
                fakeHead.x += 1;
                break;
        }

        prob += 1;
    }

    if (type == 1)
    {
        prob += 1000;
    }

    return prob;
}

/*******************************************************
Function: try to move one step to find
          snake is live or die
Argument: int
Return  : bool
*******************************************************/
bool Snake::fakeMove(int direction)
{
    bool life = true;
    Node fakeHead = *this->head;

    switch(direction)
    {
        case CTRL_UP:
            fakeHead.y -= 1;
            break;
        case CTRL_DOWN:
            fakeHead.y += 1;
            break;
        case CTRL_LEFT:
            fakeHead.x -= 1;
            break;
        case CTRL_RIGHT:
            fakeHead.x += 1;
            break;
    }

    // hit wall or hit body
    if (hitWall(fakeHead) || hitBody(fakeHead, this))
    {
        life = false;
    }
    if ( !this->way_to_head(&this->body[this->length-1]) )
    {
        life = false;
    }

    return life;
}

/*******************************************************
Function:
Argument:
Return  :
*******************************************************/
void Snake::judgeLife()
{
    // // hit wall or hit body, die
    if (hitWall(*this->head) || hitBody(*this->head, this))
    {
        getch();
        this->setlife(false);  // T_T
    }
}

/*******************************************************
Function: snake eat a food
Argument: Food
Return  : void
*******************************************************/
void Snake::eat(Food *food)
{
    length += 1;                                  // become longer
    this->body[length-1] = this->body[length-2];  // set the new body in the same place of last one
    delete food;                                  // after eat, this food will disappear
}

/*******************************************************
Function: hide a snake
Argument: none
Return  : void
*******************************************************/
void Snake::clear()
{
    for(int i = 0; i < length; i++)
    {
        drawOne(this->body[i].x, this->body[i].y, (char *)ICON_NULL);
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
    x = random(BOARD_LEFT + 1, BOARD_RIGHT - 1);
    // while (x % 2 != 0) x = random(BOARD_LEFT + 2, BOARD_RIGHT - 2);              // adjust x to even number
    y = random(BOARD_TOP + 1, BOARD_BOTTOM - 1);

    for(int i = 0; i < snake->getsnakelength(); i++)          // for every node of this snake
    {
        if (x == snake->body[i].x && y == snake->body[i].y)  // if this food is in one node of this snake
        {
            new Food(snake);                            // new another food
            return;                                     // return to avoid showing it
        }
    }
    drawOne(x, y, (char *)g_const_star_b); // show this food
}

/*******************************************************
Function: show a snake
Argument: none
Return  : void
*******************************************************/
void Snake::initialDraw()
{
    for (int i = 0; i < length; i++)
    {
        if (i == 0)
        {
            drawOne(this->body[i].x, this->body[i].y, (char *)g_const_circ_b); // head
        }
        else
        {
            drawOne(this->body[i].x, this->body[i].y, (char *)g_const_rect_b); // body
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
Node *Snake::getbody()
{
    return this->body;
}

/*******************************************************
Function: food, snake, control, move
Argument: none
Return  : void
*******************************************************/
void Playing()
{
    int  counter = 0;             // counter, if this counts appointed times, snake will change direction(if necessary)
    int  GameSpeed = GAME_SPEED;  // game speed
    char direction = 0;           // direction the snake will wriggle
    char gotten = 0;              // the input from keyboard
    char cache1st = 0;            // store the next direction
    char cache2nd = 0;            // store the one after next direction(when necessary)

    switch(g_difficulty)
    {
        case HARD:
            GameSpeed = 5;
            break;
        case MEDIUM:
            GameSpeed = 10;
            break;
        case EASY:
            GameSpeed = 20;
            break;
    }

    Snake *snake = new Snake();     // new one snake
    Food  *food = new Food(snake);  // new a food

    while (snake->getlife()) // while alive, process the snake move, snake eat things. important block
    {
        // game speed  -S
        counter++;
        usleep(40000);
        // game speed -E

        if (_kbhit())
        {
            gotten = _getch();        // get direction(but can be not direction input)

            if (gotten == -32)continue;
            if (gotten == CTRL_SPACE || gotten == CTRL_ENTER) Pause();
            if (gotten == CTRL_ESC)exit(0);

            // when cache1st contains data(which means direction) && gotten is not in the same line with cache1st
            if (isDirection(cache1st) == true && isSameLine(gotten,cache1st) == false)
            {
                cache2nd = gotten;    // store gotten in cache2nd
            }

            // when gotten is direction && gotten is not in the same line with snake's direction
            if (isDirection(gotten) == true && isSameLine(gotten,snake->getdirection()) == false)
            {
                cache1st = gotten;    // store gotten in cache1st
            }
        }

        if (counter >= GameSpeed)      // each GameSpeed times, judge move
        {
            counter = 1;  // reset counter

            // cache2nd -> cache1st -> direction -> snake->move(direction)
            direction = cache1st;     // assembly line work
            cache1st = cache2nd;      // assembly line work

            #ifdef _AI_MODE_
            direction = snake->Control_AI(food, snake); GameSpeed = 1; // auto play
            #endif

            snake->move(direction); // snake wriggling

            snake->judgeLife();     // to be or not to be...

            // eat food
            if (snake->body[0].x == food->x && snake->body[0].y == food->y)
            {
                snake->eat(food);       // ^_^
                food = new Food(snake); // after eat, generate a new food

                g_score += g_difficulty;  // difficulty means point
                g_eaten += 1;
            }
        }
    }

    // clean food and snake
    clear();
    drawGame();
    delete food;
    delete snake;
}

/*******************************************************
Function: main entrance
Argument: none
Return  : int
*******************************************************/
int main()
{
    Initialize();
    g_difficulty = 3;
    drawGame();
    while (true)
    {
        Playing();
    }
    return 0;
}


