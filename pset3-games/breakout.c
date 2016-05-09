/**
* breakout.c
* Charles Williams
* lose311@aol.com
*
* Breakout game made using the Stanford Portable Library.
* User must bounce the ball off the paddle and break 
* all the bricks before running out of lives.
*/

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>
#include <spl/gevents.h>
#include <spl/ginteractors.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// paddle size
#define PADDLEW 50
#define PADDLEH 10

// brick size
#define BRICKH 10
#define BRICKW 33
#define BRICKGAP 6

// ball size
#define BALLSIZE 20

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator with current time
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);
    add(window, paddle);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);
    GLabel playlabel = newGLabel("");
    add(window, label);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    // char array for click to play, lives, and game over label
    char s[28];
    
    // display 'click to play', remaining lives label, or game over
    WAITFORCLICK:
    if (lives > 0)
        sprintf(s, "Click to play | Lives = %i", lives);
    else
        sprintf(s, "Game Over! %i lives left :(", lives);
    setLabel(playlabel, s);
    setFont(playlabel, "SansSerif-18");
    double x = ((getWidth(window) - getWidth(playlabel)) / 2);
    double y = ((getHeight(window) - getHeight(playlabel)) / 2 + 50);
    setLocation(playlabel, x, y);
    add(window, playlabel);  
    add(window, ball);  

    // user click starts game
    while (true)
    {
        // wait for mouse click
        GEvent event = getNextEvent(MOUSE_EVENT);
        if (event != NULL)
        {
            if (getEventType(event) == MOUSE_CLICKED)
            {
                // remove 'click to play' and remaining lives label once playing
                removeGWindow(window, playlabel);   
                break;
            }
        }
    }

    // initiate ball movement with random x-axis velocity
    int ballsize = BALLSIZE;
    double velocityx = drand48() + .2;    
    double velocityy = 2;
    int pauselength = 4;

    // keep playing until game over
    while (lives > 0 && points < 50)
    {
        // ball movement
        move(ball, velocityx, velocityy);

        // bounce off right edge of window
        if (getX(ball) + getWidth(ball) >= getWidth(window))
        {
            velocityx = -velocityx;
        }

        // bounce off left edge of window
        else if (getX(ball) <= 0)
        {
            velocityx = -velocityx;
        }
        
        // bounce off top of window
        else if (getY(ball) <= 0)
        {
            velocityy = -velocityy;
        }
                
        // lose a life if ball crosses bottom of the window
        else if (getY(ball) + getHeight(ball) >= getHeight(window))
        {
            lives--;
            setLocation (ball, getWidth(window)/2 - ballsize/2, getHeight(window)/2 - ballsize/2);
            goto WAITFORCLICK;
        }

        // linger before moving again
        pause(pauselength);
        
        // cursor follows mouse input along x-axis for paddle movement
        GEvent mouseevent = getNextEvent(MOUSE_EVENT);
        if (mouseevent != NULL)
        {
            if (getEventType(mouseevent) == MOUSE_MOVED)
            {
                double x = getX(mouseevent) - getWidth(paddle) / 2;
                setLocation(paddle, x, getHeight(window) - 20);
            }
        }
        
        // detect collision with paddle or bricks
        GObject object = detectCollision(window, ball);
        if (object != NULL)
        {
            if (strcmp(getType(object), "GRect") == 0)
            {
                velocityy = -velocityy;
                if (object == paddle)
                ;
                else
                {
                    removeGWindow(window, object);    
                    points++;
                    updateScoreboard(window, label, points);
                }
            }
        }       
    }
    
    // If user destorys all bricks, display winning message
    if (points == bricks)
    {
        sprintf(s, "Congratulations! You Win!");
        setLabel(playlabel, s);
        setFont(playlabel, "SansSerif-18");
        double x = ((getWidth(window) - getWidth(playlabel)) / 2);
        double y = ((getHeight(window) - getHeight(playlabel)) / 2 + 50);
        setLocation(playlabel, x, y);
        add(window, playlabel);      
    }
    
    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    int rows = ROWS;
    int cols = COLS;
    int brickh = BRICKH;
    int brickw = BRICKW;
    int brickgap = BRICKGAP;
    
    // draws bricks
    for (int i = 0; i < cols; i++)
    {
        GRect brick = newGRect(brickgap + (i * (brickw + brickgap)), 20, brickw, brickh);
        setFilled(brick, true);
        setColor(brick, "BLACK");
        add(window, brick);
    }
    for (int i = 0; i < cols; i++)
    {
        GRect brick = newGRect(brickgap + (i * (brickw + brickgap)), 20 + brickh + brickgap, brickw, brickh);
        setFilled(brick, true);
        setColor(brick, "DARK_GRAY");
        add(window, brick);
    }
    for (int i = 0; i < cols; i++)
    {
        GRect brick = newGRect(brickgap + (i * (brickw + brickgap)), 20 + 2 * (brickh + brickgap), brickw, brickh);
        setFilled(brick, true);
        setColor(brick, "GRAY");
        add(window, brick);
    }
    for (int i = 0; i < cols; i++)
    {
        GRect brick = newGRect(brickgap + (i * (brickw + brickgap)), 20 + 3 * (brickh + brickgap), brickw, brickh);
        setFilled(brick, true);
        setColor(brick, "BLUE");
        add(window, brick);
    }
    for (int i = 0; i < cols; i++)
    {
        GRect brick = newGRect(brickgap + (i * (brickw + brickgap)), 20 + 4 * (brickh + brickgap), brickw, brickh);
        setFilled(brick, true);
        setColor(brick, "CYAN");
        add(window, brick);
    }    
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    int ballsize = BALLSIZE;
    GOval ball = newGOval(getWidth(window)/2 - ballsize/2, getHeight(window)/2 - ballsize/2, ballsize, ballsize);
    setFilled(ball, true);
    setColor(ball, "GREEN");
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    // create and return blue paddle based on defined dimensions
    int paddlew = PADDLEW;
    int paddleh = PADDLEH;
    GRect paddle = newGRect(getWidth(window)/2 - paddlew/2, getHeight(window) - 20, paddlew, paddleh);
    setFilled(paddle, true);
    setColor(paddle, "BLUE");
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel score = newGLabel("");
    setFont(score, "SansSerif-36");
    return score;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[4];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
