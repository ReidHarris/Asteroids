# Asteroids
An asteroids game implemented in C++ and OpenGL. It also requires GLUT.

## Classes

### Object

This program implements an Object class which stores position, velocity, mass and radius (for collisions) information. The Bullet, Player, and Object classes all inherit from the Object class. Each of these classes implements its own checkBounds method.

#### Bullet

Bullets do not have any additional members. They are initialized with mass 10.

#### Player

This derived class includes members to store the orientation, angular velocity, score and keyboard inputs for the player. A Player is initialized in the upward orientation and with 0 velocity and 0 angular velocity and mass 1.

#### Particle

The Particle class includes additional members to store color in an array of 3 doubles.

### Game

The Game class stores a single Player and arrays of Bullets and Particles. This class also stores the number of lives which the player has and methods for updating and ending the game (in the event of a game over).

## Gameplay

The ship is initiated at the center of the screen. The following are the controls for the ship:
* spacebar - shoot a bullet in the direction the ship is pointed
* a - turn the ship clockwise
* d - turn the ship counterclockwise
* w - accelerate the ship in the direction it is pointing
* s - accelerate the ship in the opposite direction it is pointing
* ESC - end game and close window

There is an effect of gravity which can be tuned by changing GRAV_CONSTANT in player.hpp. The ship and bullets are attracted to asteroids but asteroids themselves are uneffected by gravity. This is a choice that makes the game more playable.

