/******************************************************************************
* File:    snake.h
* Class:   COP 2001 202005
* Author:  Rachel Matthews
* Purpose: Implementation file for Snake class for a snake game where the
* snake moves around the screen and grows when it eats food
******************************************************************************/
#include "snake.h"

// ----------------------------------------------------------------------------
// constructors
// ----------------------------------------------------------------------------
/******************************************************************************
 default consructor
 *****************************************************************************/
Snake::Snake()
{
	// initialize the snake with 1 body block for the head
	head = Block(WINDOW_WIDTH / 2 - BLOCK_SIZE / 2,
		         WINDOW_HEIGHT / 2 - BLOCK_SIZE / 2,
		         BLOCK_SIZE,
		         BLOCK_SIZE,
		         SNAKE_COLOR);

	tail = &head;
							
	size = 1;					// snake has one block at start
	current = None;				// not moving, so no current or
	next = None;				// next direction
	distance = 0;				// snake has not started moving yet
	
}

// ------------------------------------------------------------------------
// accessors (getters and setters)
// ------------------------------------------------------------------------
Block* Snake::getHead() { return &head; }
Block* Snake::getTail() { return tail; }
int	   Snake::getSize() { return size; }
float  Snake::getDistance() { return distance; }

Direction Snake::getCurrent() { return current; }
Direction Snake::getNext() { return next; }
void      Snake::setNext(Direction value) { next = value; }


/******************************************************************************
determines if the snake has velocity in either the x or y coordinate
returns:
	bool   - returns true if the block is moving
*******************************************************************************/
bool Snake::isMoving()
{
	// truncate floats to integer for comparison
	return head.isMoving();
}


/******************************************************************************
turn the snake based on user input direction
returns:
	void
*******************************************************************************/
void Snake::turn()
{
	// adjust the head of the snake's velocity based on
	// next direction the user wants the snake's head to go
	switch (next)
	{
	case Up:
		if (current != Down)
		{
			head.setVelocityX(0);
			head.setVelocityY(-1);
			current = next;
		}
		break;
	case Left:
		if (current != Right)
		{
			head.setVelocityX(-1);
			head.setVelocityY(0);
			current = next;
		}
		//x -= width;
		break;
	case Down:
		if (current != Up)
		{
			head.setVelocityX(0);
			head.setVelocityY(1);
			current = next;
		}
		break;
	case Right:
		if (current != Left)
		{
			head.setVelocityX(1);
			head.setVelocityY(0);
			current = next;
		}
	}
	// wait for another user direction change
	next = None;

	return;
}// end turn


/******************************************************************************
move the snake a set distance
parameters:
	distance  -  distance in pixels to move the snake
returns:
	void
*******************************************************************************/
void Snake::move(float distance)
{
	// limit the distance to full blocks only
	if (this->distance + distance > BLOCK_SIZE)
		distance = BLOCK_SIZE - this->distance;

	// set snake's total distance moved
	this->distance += distance;

	Block* part = &head;
	while(part)
	{
		// move the block
		part->move(distance);

		// move pointer to block after this one
		part = part->getAfter();
	}

	return;
}// end move


/******************************************************************************
 snake has traveld a full block size
parameters:
returns:
	bool  -  true if moved full block size
*******************************************************************************/
bool Snake::hasMoved()
{
	return distance >= BLOCK_SIZE;

}


/******************************************************************************
reset snake's distance and set snake direction and alignement
parameters:
returns:
	void
*******************************************************************************/
void Snake::reset()
{
	distance = 0;

	Block* part = tail;
	// loop from tail to head to adjust velocities
	while (part->getBefore())
	{
		// copy the x-velocity from the block infront of this one
		part->setVelocityX( part->getBefore()->getVelocityX() );

		// align the vertical tops for this block
		if (part->getVelocityX())
			part->setY( part->getBefore()->getY() );

		// copy they-velocity from the block infront of this one
		part->setVelocityY( part->getBefore()->getVelocityY() );

		// align the horizontal left sides for this block
		if ( part->getVelocityY())
			 part->setX( part->getBefore() ->getX() );

		part = part->getBefore();
	}// adjust velocities

} // end reset


/******************************************************************************
checks to see if head of the snake hit the body
parameters:
returns:
	bool  -  true if collision
*******************************************************************************/
bool Snake::hitSelf()
{
	bool collide = false;

	Block* part = head.getAfter();
	int skip = 3;
	while (part && !collide)
	{
		// skip the first 3 blocks after the head
		if (skip)
			skip--;
		else
			// check the head with current part
			collide = head.intersects(part);

		// move to next part
		part = part->getAfter();
	}

	return collide;
}// end hitSelf


/******************************************************************************
checks to see if block overlaps the head of the snake or the whole snake
parameters:
	block  -  pointer to block to check
	all    -  false = just head, true = check whole snake (default to false)
returns:
	bool   -  true if collision
*******************************************************************************/
bool Snake::hitBlock(Block* block, bool all)
{
	bool collide = head.intersects(block);

	// do we need to check rest of body
	if (all && !collide)
	{
		Block* part = head.getAfter();
		
		while (part && !collide)
		{	
			// check the block with current part
			collide = block->intersects(part);

			// move to next part
			part = part->getAfter();
		}
	}

	return collide;
}// end hitBlock


/******************************************************************************
adds a new tail onto end of snake
parameters:
returns:
	void
*******************************************************************************/
void Snake::grow()
{
	// save off the current tail location
	tail->append(tail);

	tail = tail->getAfter();

	// adjust new tail to after current in opposite direction
	tail->setX(tail->getX() + tail->getVelocityX() * -1 * BLOCK_SIZE);
	tail->setY(tail->getY() + tail->getVelocityY() * -1 * BLOCK_SIZE);

	// increment snake size
	size ++;

	return;
}// end grow


/******************************************************************************
render the snake's body on the graphics window
parameters:
	window  -  handle to graphics window
returns:
	void
*******************************************************************************/
void Snake::draw(OpenGL& window)
{
	Block* part = &head;
	while (part)
	{
		// move the block
		part->draw(window);

		// move pointer to block after this one
		part = part->getAfter();
	}

	return;
}// end draw
