//
//  player.cpp
//  Asteroids
//
//  Created by Reid Harris on 12/9/20.
//  Copyright © 2020 Reid Harris. All rights reserved.
//

#include "player.hpp"

Game* current_game = nullptr;

void init(){
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glOrtho(GLdouble(-X_WINDOW_SIZE/2), GLdouble(X_WINDOW_SIZE/2), GLdouble(Y_WINDOW_SIZE/2), GLdouble(-Y_WINDOW_SIZE/2), -1, 1);
}


void drawBitmapText(std::string s, float x, float y){
    glRasterPos2f(x, y);

    for(char c : s)  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, c);
}

void gameOver(){
	std::string go = "GAME OVER";
    glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2f(-50, 0);

	for(char c : go) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
}

/*
 * Display Function.
 */

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Display stats.
    drawBitmapText("Number of Planets: " 	+ std::to_string(current_game->particles.size()), -X_WINDOW_SIZE/2+50, -Y_WINDOW_SIZE/2+65);
    drawBitmapText("X Position: " 			+ std::to_string(current_game->play.x_position), -X_WINDOW_SIZE/2+50, -Y_WINDOW_SIZE/2+80);
    drawBitmapText("Y Position: " 			+ std::to_string(current_game->play.y_position), -X_WINDOW_SIZE/2+50, -Y_WINDOW_SIZE/2+95);
    drawBitmapText("X Velocity: " 			+ std::to_string(current_game->play.x_velocity), -X_WINDOW_SIZE/2+50, -Y_WINDOW_SIZE/2+110);
    drawBitmapText("Y Velocity: " 			+ std::to_string(current_game->play.y_velocity), -X_WINDOW_SIZE/2+50, -Y_WINDOW_SIZE/2+125);
    drawBitmapText("Y Orientation: " 		+ std::to_string(current_game->play.x_orientation), -X_WINDOW_SIZE/2+50, -Y_WINDOW_SIZE/2+140);
    drawBitmapText("Y Orientation: " 		+ std::to_string(current_game->play.y_orientation), -X_WINDOW_SIZE/2+50, -Y_WINDOW_SIZE/2+155);
    drawBitmapText("Lives: " 		+ std::to_string(current_game->remaining_lives), -X_WINDOW_SIZE/2+50, -Y_WINDOW_SIZE/2+170);
    drawBitmapText("Score: " 				+ std::to_string(current_game->play.score), -X_WINDOW_SIZE/2+50, Y_WINDOW_SIZE/2-50);

    if(current_game->game_over) gameOver();

    //Show particles.
    for (Particle &p : current_game->particles) {
        glColor3f(p.color[0], p.color[1], p.color[2]);
        glBegin(GL_POLYGON);
        // Draw the particle as a dodecagon.
        for (float t = 0.0; t < 2 * PI; t += PI/6) {
            glVertex2f(p.radius * cos(t) + p.x_position, p.radius * sin(t) + p.y_position);
        }
        glColor3f(1.0f, 1.0f, 1.0f);
        glEnd();
    }

    //Show bullets.
    for (Bullet b : current_game->bullets) {
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_POLYGON);
        // drawing the particle as a circle
        for (float t = 0.0; t < 2 * PI; t += PI/3) {
            glVertex2f(cos(t) + b.x_position, sin(t) + b.y_position);
        }
        glColor3f(1.0f, 1.0f, 1.0f);
        glEnd();
    }

    //Show player.
    if(!current_game->game_over){
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex2f(current_game->play.x_position - 5*current_game->play.x_orientation - 7*current_game->play.y_orientation,
				   current_game->play.y_position - 5*current_game->play.y_orientation + 7*current_game->play.x_orientation);
		glVertex2f(current_game->play.x_position - 5*current_game->play.x_orientation + 7*current_game->play.y_orientation,
				   current_game->play.y_position - 5*current_game->play.y_orientation - 7*current_game->play.x_orientation);
		glVertex2f(current_game->play.x_position - 5*current_game->play.x_orientation + 20*current_game->play.x_orientation,
				   current_game->play.y_position - 5*current_game->play.y_orientation + 20*current_game->play.y_orientation);
		glEnd();
    }


    glFlush();
    glutSwapBuffers();
}

/*
 * Update Function.
 */

void timer(int) {
    display();

    // Calculate speed of player.
    double vel = sqrt(current_game->play.x_velocity*current_game->play.x_velocity
    				+ current_game->play.y_velocity*current_game->play.y_velocity);

    // Update player variables based on what keys are pressed, if possible.
    if (current_game->play.up && vel < MAX_VELOCITY){
    	current_game->play.x_velocity += current_game->play.x_orientation;
    	current_game->play.y_velocity += current_game->play.y_orientation;
    	current_game->play.up = false;
    }

    if (current_game->play.down && vel < MAX_VELOCITY){
    	current_game->play.x_velocity -= current_game->play.x_orientation;
    	current_game->play.y_velocity -= current_game->play.y_orientation;
    	current_game->play.down = false;
    }

    if (current_game->play.left && current_game->play.angular_velocity < MAX_ANGULAR_VEL){
    	current_game->play.angular_velocity += ANGULAR_VELOCITY_STEP;
    	current_game->play.left = false;
    }

    if (current_game->play.right && current_game->play.angular_velocity > -MAX_ANGULAR_VEL){
    	current_game->play.angular_velocity -= ANGULAR_VELOCITY_STEP;
    	current_game->play.right = false;
    }

    // Use gravity to update velocities;
    current_game->update();

    // Check if game over. Stop updating if true.
    if(!current_game->game_over) glutTimerFunc(1, timer, 0);
    else display();
}



/*
 Keyboard Functions.
 */

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27: //ESC.
            exit(0);
            break;

        case 'w': //Up.
        	current_game->play.up = true;
            break;

        case 's': //Down.
        	current_game->play.down = true;
            break;

        case 'a': //Left.
        	current_game->play.left = true;
            break;

        case 'd': //Right.
        	current_game->play.right = true;
            break;

        case 32: //Spacebar.
            Bullet b(   current_game->play.x_position, //Bullet x-position.
            		    current_game->play.y_position, //Bullet y-position.
            		 10*current_game->play.x_orientation,  //Bullet x-direction.
					 10*current_game->play.y_orientation); //Bullet y-direction.
            current_game->bullets.push_back(b);
    }
}

/*
 * Particle Functions.
 */

void addRandomParticle(int size) {
	//Add a random particle just out of frame.

	/* Repeatedly pick random coordinates until (x,y) does not lie within visual window.
	 *
	 *			-----------------
	 * 			|INSERT PARTICLE|
	 * 			|	  ----- 	|
	 * 			|	  |\ /| 	|
	 * 			|	  | X | 	|
	 * 			|	  |/ \| 	|
	 * 			|	  ----- 	|
	 * 			|	   HERE		|
	 * 			-----------------
	 */

    double x = rand() % (100 + X_WINDOW_SIZE) - X_WINDOW_SIZE/2;
    double y = rand() % (100 + Y_WINDOW_SIZE) - Y_WINDOW_SIZE/2;

    while(abs(x) < X_WINDOW_SIZE/2 && abs(y) < Y_WINDOW_SIZE/2){
        x = rand() % (100 + X_WINDOW_SIZE) - X_WINDOW_SIZE/2;
        y = rand() % (100 + Y_WINDOW_SIZE) - Y_WINDOW_SIZE/2;
    }

    // Pick random mass between [0, size].
    double m = rand() % size;

    // Random float generator.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0, 2);

    // Pick random velocities and make sure they are pointed inward toward the center.
    double vx = ((x > 0)? -1 : 1)*dist(gen);
    double vy = ((y > 0)? -1 : 1)*dist(gen);

    // Pick random color.
    double color[3];
    for(int i = 0; i < 3; i++) color[i] = rand() % 200/200.0;

    //Insert particle.
    Particle p(x, y, 5*sqrt(m), vx, vy, m, color);
    current_game->particles.push_back(p);

    //Stop if game over.
    if(!current_game->game_over) glutTimerFunc(500, addRandomParticle, size);
}

double distance(Object& p1, Object& p2){
	return sqrt((p2.x_position - p1.x_position)*(p2.x_position - p1.x_position)
			  + (p2.y_position - p1.y_position)*(p2.y_position - p1.y_position));
}

/*
 * Update velocity of p2 and return distance.
 */

void updateVelocity(Object& p1, Object& p2){
	double d = distance(p1, p2);
	p2.x_velocity += GRAV_CONSTANT*p2.mass*p1.mass/(d*d) * (p1.x_position - p2.x_position)/d;
	p2.y_velocity += GRAV_CONSTANT*p2.mass*p1.mass/(d*d) * (p1.y_position - p2.y_position)/d;

	// Check if p1 collides with p2.
}

/*
 * Update function for positions and velocities.
 */

//
void Game::update(){
	// Update positions and velocities of all objects.

	for (int i = 0; i < int(particles.size()); i++) {
		Particle &p = particles[i];

		//Check if particle is out of bounds.
		if(p.checkBounds()){
			particles.erase(particles.begin() + i--);
			continue;
		}

		bool not_destroy = true;
		for(int j = 0; j < int(bullets.size()); j++){
			// Update velocity of bullet.
			updateVelocity(p, bullets[j]);

			// Check if bullet destroys particle.
			if(distance(p, bullets[j]) < p.radius){
				not_destroy = false;
				bullets.erase(bullets.begin() + j);
				play.score++;
				break;
			}
		}

		// Update velocity of player.
		updateVelocity(p, play);

		// Collision with particle and player.
		if(distance(p, play) < p.radius + 3){
			if(remaining_lives == 0) game_over = true;
			else{
				// Reset game.
				remaining_lives--;
				play.x_position = 0;
				play.y_position = 0;
				play.x_velocity = 0;
				play.y_velocity = 0;
				particles.clear();
				bullets.clear();
			}
		}

		if(not_destroy) p.updatePosition();
		else p.divide();
	}
	// Update player coordinates.
	play.updatePosition();

	// Update player orientation.
	double angle = acos(play.x_orientation) * ((play.y_orientation >= 0)? 1 : -1);
	play.x_orientation = cos(angle - play.angular_velocity*PI/90);
	play.y_orientation = sin(angle - play.angular_velocity*PI/90);

	//Check if player is out of bounds and update coordinates if true.
	play.checkBounds();

	//Update bullet coordinates.
	for(int j = 0; j < int(bullets.size()); ++j){
		if(bullets[j].checkBounds()) bullets.erase(bullets.begin() + j);
		else bullets[j].updatePosition();
	}
}

void Particle::divide(){
	//Divide a particle into to equal pieces, moving along +/-45 degree angles from original particle.

	double nf = sqrt(2)/2; // Normalization factor.

	//If particle is smaller than a threshold mass, place out of bounds for deletion.
	if(radius <= 25) x_position = X_WINDOW_SIZE;

	// Add a new smaller particle.
	Particle p1(x_position, y_position, radius/2, nf*(x_velocity+y_velocity), nf*(y_velocity-x_velocity), mass/3, color);
	current_game->particles.push_back(p1);

	//Update the mass and radius of old particle to match p1.
	radius /= 2;
	mass /= 3;

	// Update the mass and velocity of old particle.
	double new_y_velocity = nf*(x_velocity + y_velocity);
	x_velocity = nf*(x_velocity - y_velocity);
	y_velocity = new_y_velocity;


}
