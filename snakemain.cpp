/******************************************************************************
* File:    snakemain.cpp
* Class:   COP 2001 202005
* Author:  Rachel Matthews
* Purpose: main application file for the Snake Game (Final Lab)
******************************************************************************/
#include <iostream>      // console i/o
#include <stdlib.h>		 // random numbers
#include <time.h>		 // seed random numbers

#include "opengl.h"      // graphics library
#include "snakedefs.h"   // game definitions
#include "block.h"       // game block objects
#include "snake.h"		 // the snake object

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void InitializeGame(Block borders[], Snake& snake, Block& food);
Direction processInput(OpenGL& window);
bool update(Snake& snakes, Block borders[], Block& food, float deltaTime);
void render(OpenGL& window, Block borders[], Snake snake, Block& food);
bool CollisionChecks(Snake snake, Block borders[]);
Block NextFood(Snake snake);

int main()
{
	// initiate random number sequences
	srand(time(NULL));

	// create graphic window for drawing
	OpenGL window = OpenGL(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str());

	// game objects
	//  border wall index: top bottom left right
	Block borders[4];

	// the snake
	Snake snake = Snake();  // initialize with default constructor

	// the food block
	Block food;

	InitializeGame(borders, snake, food);

	// frames-per-second time variables
	// -------------------
	float deltaTime{ 0.0f };
	float lastFrame{ 0.0f };

	// main game loop
	bool gameOver{ false };
	while (!gameOver && !window.isClosing())
	{
		// calculate delta(elapsed) time
		// -----------------------------
		float currentFrame = window.getTime();
		deltaTime += (currentFrame - lastFrame) * 1000;
		lastFrame = currentFrame;

		// get user input if not still waiting to process input
		if (snake.getNext() == None)
			snake.setNext(processInput(window));

		// set gameover if user chose to exit
		if (snake.getNext() == Exit)
			gameOver = true;

		// update game objects at each sub-frame interval
		while (!gameOver && deltaTime >= FPS_RATE)
		{
			gameOver = update(snake, borders, food, deltaTime);
			deltaTime -= FPS_RATE;
		}

		// draw objects
		render(window, borders, snake, food);

		window.pollEvents();
	}

	char pause;
	std::cin >> pause;

	return 0; // echo %errorlevel%
} // end main


/******************************************************************************
initialize game objects prior to game starting
parameters:
	borders - array of border blocks for drawing screen borders
	snake   - snake class object
	food    - the food block
returns:
	void
*******************************************************************************/
void InitializeGame(Block borders[], Snake& snake, Block& food)
{
	// initialize border walls, border index: top bottom left right
	// top border
	borders[Top] = Block(MARGIN,
		                 MARGIN,
		                 WINDOW_WIDTH - 2 * MARGIN,
		                 BORDER_SIZE,
		                 BORDER_COLOR);

	// bottom border
	borders[Bottom] = Block( MARGIN,
						     WINDOW_HEIGHT - MARGIN - BORDER_SIZE,
						     WINDOW_WIDTH - 2 * MARGIN,
						     BORDER_SIZE,
						     BORDER_COLOR);


	// left border
	borders[LeftBorder] = Block(MARGIN,
		                        MARGIN,
	                         	BORDER_SIZE,
		                        WINDOW_HEIGHT - 2 * MARGIN,
		                        BORDER_COLOR);


	// right border
	borders[RightBorder] = Block( WINDOW_WIDTH - MARGIN - BORDER_SIZE,
							      MARGIN,
							      BORDER_SIZE,
							      WINDOW_HEIGHT - 2 * MARGIN,
							      BORDER_COLOR);

	// get a new block of food
	food = NextFood(snake);                 

	return;
} // end InitializeGame


/******************************************************************************
process the user input from keyboard
parameters:
	 window    - graphics and I/O object
returns:
	 Direction - user selected direction or None
*******************************************************************************/
Direction processInput(OpenGL& window)
{
	Direction DirectionValue{ None };

	GL_KEY key = window.GetKey();

	switch (key)
	{
	case GL_KEY::KEY_ESC:
	case GL_KEY::KEY_X:
		DirectionValue = Direction::Exit;
		break;
	case GL_KEY::KEY_W:
	case GL_KEY::KEY_UP_ARROW:
		DirectionValue = Direction::Up;
		break;
	case GL_KEY::KEY_A:
	case GL_KEY::KEY_LEFT_ARROW:
		DirectionValue = Direction::Left;
		break;
	case GL_KEY::KEY_S:
	case GL_KEY::KEY_DOWN_ARROW:
		DirectionValue = Direction::Down;
		break;
	case GL_KEY::KEY_D:
	case GL_KEY::KEY_RIGHT_ARROW:
		DirectionValue = Direction::Right;
	}

	return DirectionValue;
} // end ProcessInput


/******************************************************************************
updates changes to state of objects in game
parameters:
	snake      - snake class object
	borders    - array of border wall blocks
	food       - the food block
	deltaTime  - accumulated frame time
returns:
	bool       - true if snake hit a wall or itself (gameOver!!)
*******************************************************************************/
bool update(Snake& snake, Block borders[], Block& food, float deltaTime)
{
	//skip updates until snake starts moving
	if (!snake.isMoving())
	{
		if (snake.getNext() == None)
			return false;
		else
			// make first turn
			snake.turn();
	}

	// calculate distance traveled this frame
	float distance = SNAKE_VELOCITY / deltaTime;
	snake.move(distance);

	// while the snake is still moving it's full block size
	if (snake.hasMoved())
	{
		snake.turn();

		snake.reset();

		// check if snake ate the food
		if (snake.hitBlock(&food))
		{
			snake.grow();

			// get a new food block
			food = NextFood(snake);
		}
		// check if the snake hit a wall or itself
		else if (CollisionChecks(snake, borders))
		{
			std::cout << "Game Over!!" << '\n';
			std::cout << "Press any key and Enter to exit..." << '\n';
			return true;  // game over
		}

	} // end full block transversal

	return false; // no collisions this update

}// end update


/******************************************************************************
draws game objects to the graphics window
parameters:
	window    - handle to the graphics window
	borders   - array of border blocks for drawing screen borders
	snake     - blocks that will comprise snake
	food      - the food block
returns:
	void
*******************************************************************************/
void render(OpenGL& window, Block borders[], Snake snake, Block& food)
{
	// clear window memory buffer
	Color background = Gray;
	window.clearWindow(reinterpret_cast<const unsigned char*>(&background));

	// draw the snake
	snake.draw(window);

	// draw food block
	food.draw(window);

	// draw borders
	 borders[Top].draw(window);
	 borders[Bottom].draw(window);
	 borders[LeftBorder].draw(window);
	 borders[RightBorder].draw(window);

	// repaint screen
	window.paintWindow();

	return;
} // end render


/******************************************************************************
checks for collisions between snake's head and the walls and snake body
parameters:
	snake     - snake class object
	borders   - walls
	direction - which direction head of snake is traveling
returns:
	bool      - returns true if collision
*******************************************************************************/
bool CollisionChecks(Snake snake, Block borders[])
{
	bool collide = false;

	// see if snake hit top wall
	if (snake.getCurrent() == Up && snake.hitBlock(&borders[Top]))
		collide = true;

	// see if snake hit bottom wall
	else if (snake.getCurrent() == Down && snake.hitBlock(&borders[Bottom]))
		collide = true;

	// see if snake hit left wall
	else if (snake.getCurrent() == Left && snake.hitBlock(&borders[LeftBorder]))
		collide = true;

	// see if snake hit right wall
	else if (snake.getCurrent() == Right && snake.hitBlock(&borders[RightBorder]))
		collide = true;

	// see if snake hit itself
	else
	{
		collide = snake.hitSelf();
	}

	return collide;
} // end CollisionChecks


/******************************************************************************
generate random location for new food
parameters:
	snake  - snake class object / make sure food doesn't spawn on top of snake
returns:
	Block  - location of next food
*******************************************************************************/
Block NextFood(Snake snake)
{
	Block food = Block( 0, 0, BLOCK_SIZE, BLOCK_SIZE, FOOD_COLOR);

	bool placed = false;
	while (!placed)
	{
		// get random (x,y) within game window for food
		int xCoord= rand() % (GAME_WIDTH - BLOCK_SIZE);   // random column
		int yCoord = rand() % (GAME_HEIGHT - BLOCK_SIZE);  // random row

		// move coordinates into visible screen
		xCoord += (MARGIN + BORDER_SIZE);
		yCoord += (MARGIN + BORDER_SIZE);

		food.setX(xCoord);
		food.setY(yCoord);

		placed = !snake.hitBlock(&food, true);

	} // not placed

	return food;
} // end NextFood
