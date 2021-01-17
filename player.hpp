//
//  player.hpp
//  AsteroidsAI
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


#define X_WINDOW_SIZE 1000
#define Y_WINDOW_SIZE 750
#define GRAV_CONSTANT 0.03
#define PI acos(-1)

#define MAX_VELOCITY 300
#define MAX_ANGULAR_VEL 15.0
#define ANGULAR_VELOCITY_STEP 0.3
//#include "../ai/neuron.hpp"



//extern NeuralNet nn;


struct bullet{
public:
    bullet(double x0, double y0, double vx0, double vy0){
        x = x0;
        y = y0;
        vx = vx0;
        vy = vy0;
        m = 25;
    }
    double x; //x-coordinate
    double y; //y-coordinate
    double vx; //x-velocity
    double vy; //y-velocity
    double m; // Mass
};

struct Player{
	double x_orientation;
	double y_orientation;
	double angular_velocity;
	double x_position;
	double y_position;
	double x_velocity;
	double y_velocity;
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

		up = false;
		down = false;
		left = false;
		right = false;
	}
};

struct particle {
public:
	particle(double x0, double y0, double r0, double vx0, double vy0, double m0, double* color0)
	{

			x = x0;
			y = y0;
			r = r0;
			vx = vx0;
			vy = vy0;
			m = m0;
	        for(int i = 0; i < 3; i++) color[i] = color0[i];
	}


	bool operator==(particle p){
		return p.x == x && p.y == y;
	}


    double x; //x-coordinate
    double y; //y-coordinate
    double r; //Radius
    double vx; //x-velocity
    double vy; //y-velocity
    double m; // Mass
    double color[3];
};




struct Game{
	//Game Objects.
	Player play;
	std::vector<bullet> bullets;
	std::vector<particle> particles;

	//Mouse Positions.
	int Mx; //Current Mouse Position.
	int My;
	int dx; //Change in Mouse Position.
	int dy;

	//Mouse Button States.
	//Clicked
	bool leftClicked;
	bool rightClicked;
	bool middleClicked;
	//Released
	bool leftRelease;
	bool rightRelease;
	bool middleRelease;

	int curr_mouse_state;

	//Variables of objects.
	int curr_mass;

	bool game_over;

	Game(){
		Mx = 0;
		My = 0;
		dx = 0;
		dy = 0;
		leftClicked = false;
		rightClicked = false;
		middleClicked = false;
		leftRelease = false;
		rightRelease = false;
		middleRelease = false;
		curr_mouse_state = 0;

		curr_mass = 1000;
		game_over = false;

	}
	~Game() = default;

	void init();
	void drawBitmapText(std::string, float, float);
	void gameOver();
	void _display();
	void _timer(int);

	void _mouse(int, int, int, int);
	void _mouseMotion(int, int);
	void _keyboard(unsigned char, int, int);

	void addParticle(double, double, double, double);
	void removeParticles();
	void addCircleofParticles(double, double);
	void _addRandomParticle(int);
};

extern Game* current_game;

extern void display();
extern void timer(int);
void mouse(int, int, int, int);
void mouseMotion(int, int);
void keyboard(unsigned char, int, int);

extern void addRandomParticle(int);


#endif /* player_hpp */
