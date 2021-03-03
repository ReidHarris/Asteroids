//
//  player.hpp
//  Asteroids
//
//  Created by Reid Harris on 12/9/20.
//  Copyright © 2020 Reid Harris. All rights reserved.
//

#ifndef player_hpp
#define player_hpp

#include <stdio.h>
#include <iostream>
#include <GLUT/glut.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <OpenGL/glu.h>
#include <random>


#define X_WINDOW_SIZE 800
#define Y_WINDOW_SIZE 700
#define GRAV_CONSTANT 0.1
#define PI acos(-1)

#define MAX_VELOCITY 300
#define MAX_ANGULAR_VEL 15.0
#define ANGULAR_VELOCITY_STEP 0.3

struct Object{
	double x_position;
	double y_position;
	double x_velocity;
	double y_velocity;
	double mass;
	double radius;

	void updatePosition(){
		x_position += x_velocity;
		y_position += y_velocity;
	}

	virtual bool checkBounds() = 0;
};
// Return distance between objects.
double distance(Object&, Object&);

// Update velocity of p2.
void updateVelocity(Object&, Object&);


struct Bullet : public Object{
public:
    Bullet(double x0, double y0, double vx0, double vy0){
        x_position = x0;
        y_position = y0;
        x_velocity = vx0;
        y_velocity = vy0;
        mass = 10;
        radius = 1;
    }

    // Return if bullet is out of bounds.
    bool checkBounds() override{
    	return abs(x_position) >= X_WINDOW_SIZE/2 || abs(y_position) >= Y_WINDOW_SIZE/2;
    }
};

struct Player : public Object{
	double x_orientation;
	double y_orientation;
	double angular_velocity;
	int score;

	bool up;
	bool down;
	bool left;
	bool right;

	Player(){
		x_orientation = 0;
		y_orientation = 1;
		angular_velocity = 0;
		x_position = 0;
		y_position = 0;
		x_velocity = 0;
		y_velocity = 0;
		score = 0;
		mass = 1;

		up = false;
		down = false;
		left = false;
		right = false;
	}

	// Return if player is out of bounds. If yes, update position.
	bool checkBounds() override {
		if(abs(x_position) >= X_WINDOW_SIZE/2) x_position = -x_position;
		if(abs(y_position) >= Y_WINDOW_SIZE/2) y_position = -y_position;
	}
};

struct Particle : public Object {
public:
	Particle(double x, double y, double r, double vx, double vy, double m, double* _color)
	{
		x_position = x;
		y_position = y;
		x_velocity = vx;
		y_velocity = vy;
		radius = r;
		mass = m;
		for(int i = 0; i < 3; i++) color[i] = _color[i];
	}

	// Return if particle is out of bounds.
	bool checkBounds() override{
		return x_position > X_WINDOW_SIZE/2 + 100
				|| y_position > Y_WINDOW_SIZE/2 + 100
				|| x_position < -X_WINDOW_SIZE/2 - 100
				|| y_position < -Y_WINDOW_SIZE/2 - 100;
	}

	// Divide the particle into two equal pieces if mass is greater than threshold.
	// Otherwise, place particle out of bounds for deletion.
	void divide();

	// Color of the particle.
    double color[3];
};




struct Game{
	//Game Objects.
	Player play;
	std::vector<Bullet> bullets;
	std::vector<Particle> particles;
	int remaining_lives;

	bool game_over;

	Game(){
		game_over = false;
		remaining_lives = 5;
	}
	~Game() = default;

	//Update velocities of bullets and players and positions of particles.
	void update();

	void gameOver();
};


extern Game* current_game;

void init();
void display();
void timer(int);
void keyboard(unsigned char, int, int);
void addRandomParticle(int);
void drawBitmapText(std::string, float, float);
void drawObjects(std::vector<Object>&);


#endif /* player_hpp */
