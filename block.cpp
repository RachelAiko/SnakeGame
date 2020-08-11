/******************************************************************************
* File:    block.cpp
* Class:   COP 2001 202005
* Author:  Rachel Matthews
* Purpose: Implementation file for a Block class that represents a rectangle
* game object with position, size, and velocities.
******************************************************************************/
#include "block.h"

// ----------------------------------------------------------------------------
// constructors
// ----------------------------------------------------------------------------
/******************************************************************************
 default consructor
 *****************************************************************************/
Block::Block()
{
	x = 0.0f;		 
	y = 0.0f;		  
	width = 0;      
	height = 0;     
	color = Black;	  
	velocityX = 0.0f;  
	velocityY = 0.0f;  
	before = nullptr;
	after = nullptr;
}	

/******************************************************************************
 property consructor
******************************************************************************/
Block::Block(float x, float y, float width, float height, Color color, 
	         float velocityX, float velocityY, Block* before, Block* after)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->color = color;
	this->velocityX = velocityX;
	this->velocityY = velocityY;
	this->before = before;
	this->after = after;
}

/******************************************************************************
 copy consructor
******************************************************************************/
Block::Block(Block* block)
{
	x = block->x;
	y = block->y;
	width = block->width;
	height = block->height;
	color = block->color;
	velocityX = block->velocityX;
	velocityY = block->velocityY;
	before = block->before;
	after = block->after;
}

// ----------------------------------------------------------------------------
// accessors (getters and setters)
// ----------------------------------------------------------------------------
float   Block::getX() { return x; }
void    Block::setX(float value) { x = value; }
float   Block::getY() { return y; }
void    Block::setY(float value) { y = value; }
int     Block::getWidth() { return width; }
void    Block::setWidth(int value) { width = value; }
int     Block::getHeight() { return height; }
void    Block::setHeight(int value) { height = value; }
Color   Block::getColor() { return color; }
void    Block::setColor(Color value) { color = value; }
float   Block::getVelocityX() { return velocityX; }
void    Block::setVelocityX(float value) { velocityX = value; }
float   Block::getVelocityY() { return velocityY; }
void    Block::setVelocityY(float value) { velocityY = value; }
Block*  Block::getBefore() { return before; }
void    Block::setBefore(Block* ptrBlock) { before = ptrBlock; }
Block*  Block::getAfter() { return after; }
void    Block::setAfter(Block* ptrBlock) { after = ptrBlock; }


/******************************************************************************
draws this block object to the graphics window
parameters:
	window - handle to the graphics window
	snake  - game object
returns:
	void
*******************************************************************************/
void Block::draw(OpenGL& window)
{
	float position[2]{ x, y };
	float size[2]{ width, height };
	window.DrawSprite(position, size, reinterpret_cast<const unsigned char*> (&color));

	return;
}


/******************************************************************************
determines if a block overlaps this block
parameters:
	other - block to check overlaps this one
returns:
	bool  - returns true if they intersect
*******************************************************************************/
bool Block::intersects(Block* other)
{
	// if any of these conditions are true then the blocks 
	// do not overlap
	return !(int(other->x) > int(this->x + this->width) ||
		   int(other->x + other->width) < int(this->x)  ||
		   int(other->y) > int(this->y + this->height)  ||
		   int(other->y + other->height) < int(this->y) );
} 


/******************************************************************************
determines if this block has velocity in either the x or y coordinate
returns:
	bool   - returns true if the block is moving
*******************************************************************************/
bool Block::isMoving()
{
	// truncate floats to integer for comparison
	return int(velocityX) != 0 ||
		int(velocityY) != 0;

} 


/******************************************************************************
move a block based on set distance and velocities
parameters:
	distance  -  distance in pixels to move the snake
returns:
	void
*******************************************************************************/
void Block::move(float distance)
{
	x += velocityX * distance;
	y += velocityY * distance;
}


/******************************************************************************
link a new block to the after pointer of this block
parameters:
	other  -  append copy of 
returns:
	void
*******************************************************************************/
void Block::append(Block* other)
{
	// create new block as copy of other block
	Block* newTail = new Block(other);

	// attach new block behind this one
	after = newTail;

	// make new block's forward pointer to this one
	newTail->before = this;

	return;
}