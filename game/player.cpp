//
//  player.cpp
//  AsteroidsAI
//
//  Created by Reid Harris on 12/9/20.
//  Copyright © 2020 Reid Harris. All rights reserved.
//

#include "player.hpp"

Game* current_game = nullptr;

void Game::init(void){
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glOrtho(GLdouble(-X_WINDOW_SIZE/2), GLdouble(X_WINDOW_SIZE/2), GLdouble(Y_WINDOW_SIZE/2), GLdouble(-Y_WINDOW_SIZE/2), -1, 1);

    current_game = this;
}


void Game::drawBitmapText(std::string s, float x, float y){
    glRasterPos2f(x, y);
    for(char c : s){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, c);
    }
}

void Game::gameOver(){
	std::string go = "GAME OVER";
    glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2f(-50, 0);
	for(char c : go){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}
}

/*
 * Display Function.
 */

void Game::_display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Display the mass of next particle.
    Game::drawBitmapText("Mass of Next Planet: " + std::to_string(curr_mass), -X_WINDOW_SIZE/2+50, -Y_WINDOW_SIZE/2+50);

    // Display stats.
    Game::drawBitmapText("Number of Planets: " + std::to_string(particles.size()), -X_WINDOW_SIZE/2+50, -Y_WINDOW_SIZE/2+65);
    Game::drawBitmapText("X Position: " + std::to_string(play.x_position), -X_WINDOW_SIZE/2+50, -Y_WINDOW_SIZE/2+80);
    Game::drawBitmapText("Y Position: " + std::to_string(play.y_position), -X_WINDOW_SIZE/2+50, -Y_WINDOW_SIZE/2+95);
    Game::drawBitmapText("X Velocity: " + std::to_string(play.x_velocity), -X_WINDOW_SIZE/2+50, -Y_WINDOW_SIZE/2+110);
    Game::drawBitmapText("Y Velocity: " + std::to_string(play.y_velocity), -X_WINDOW_SIZE/2+50, -Y_WINDOW_SIZE/2+125);
    Game::drawBitmapText("Y Orientation: " + std::to_string(play.x_orientation), -X_WINDOW_SIZE/2+50, -Y_WINDOW_SIZE/2+140);
    Game::drawBitmapText("Y Orientation: " + std::to_string(play.y_orientation), -X_WINDOW_SIZE/2+50, -Y_WINDOW_SIZE/2+155);
    Game::drawBitmapText("Score: " + std::to_string(play.score), -X_WINDOW_SIZE/2+50, Y_WINDOW_SIZE/2-50);
    if(game_over) gameOver();

    //Show particles.
    for (int i = 0; i < int(particles.size()); ++i) {
        particle &p = particles[i];
        glColor3f(p.color[0], p.color[1], p.color[2]);
        glBegin(GL_POLYGON);
        // drawing the particle as a circle
        for (float t = 0.0; t < 2 * PI; t += PI/6) {
            glVertex2f(p.r * cos(t) + p.x, p.r * sin(t) + p.y);
        }
        glColor3f(1.0f, 1.0f, 1.0f);
        glEnd();
    }

    //Show bullets.
    for (bullet b : bullets) {
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_POLYGON);
        // drawing the particle as a circle
        for (float t = 0.0; t < 2 * PI; t += PI/3) {
            glVertex2f(cos(t) + b.x, sin(t) + b.y);
        }
        glColor3f(1.0f, 1.0f, 1.0f);
        glEnd();
    }

    //Show player.
    if(!game_over){
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex2f(play.x_position - 5*play.x_orientation - 7*play.y_orientation, play.y_position - 5*play.y_orientation + 7*play.x_orientation);
		glVertex2f(play.x_position - 5*play.x_orientation + 7*play.y_orientation, play.y_position - 5*play.y_orientation - 7*play.x_orientation);
		glVertex2f(play.x_position - 5*play.x_orientation + 20*play.x_orientation, play.y_position - 5*play.y_orientation + 20*play.y_orientation);
		glEnd();
    }


    glFlush();
    glutSwapBuffers();
}

/*
 * Update Function.
 */

void Game::_timer(int) {
    display();
    //neuralKeyboard();

    /* Add a particle manually.
    if (leftRelease) {
        addParticle(curr_mass, cbrt(curr_mass), dx, dy);
        leftRelease = false;
        dx = 0;
        dy = 0;
    }
    if (rightRelease) {
        addParticle(10*curr_mass, cbrt(10*curr_mass), dx, dy);
        rightRelease = false;
        dx = 0;
        dy = 0;
    }
    */

    double vel = sqrt(play.x_velocity*play.x_velocity + play.y_velocity*play.y_velocity);

    if (play.up && vel < MAX_VELOCITY){
    	play.x_velocity += 2*play.x_orientation;
    	play.y_velocity += 2*play.y_orientation;
    	play.up = false;
    }

    if (play.down && vel < MAX_VELOCITY){
    	play.x_velocity -= 2*play.x_orientation;
    	play.y_velocity -= 2*play.y_orientation;
    	play.down = false;
    }

    if (play.left && play.angular_velocity < MAX_ANGULAR_VEL){
    	play.angular_velocity += ANGULAR_VELOCITY_STEP;
    	play.left = false;
    }

    if (play.right && play.angular_velocity > -MAX_ANGULAR_VEL){
    	play.angular_velocity -= ANGULAR_VELOCITY_STEP;
        play.right = false;
    }

    for (int i = 0; i < int(particles.size()); i++) {
        particle &p = particles[i];

        //Check out of bounds.
        if(p.x > X_WINDOW_SIZE/2 + 100 || p.y > Y_WINDOW_SIZE/2 + 100 || p.x < -X_WINDOW_SIZE/2 - 100 || p.y < -Y_WINDOW_SIZE/2 - 100){
            particles.erase(particles.begin() + i);
            continue;
        }


        bool notFall = true;

        for(int j = 0; j < int(bullets.size()); j++){
            bullet &b = bullets[j];
            double d = sqrt((b.x - p.x)*(b.x - p.x) + (b.y - p.y)*(b.y - p.y));

            b.vx += 100*GRAV_CONSTANT*b.m*p.m/(d*d) * (p.x - b.x)/d;
            b.vy += 100*GRAV_CONSTANT*b.m*p.m/(d*d) * (p.y - b.y)/d;

            if(d < p.r){
                notFall = false;
                play.score++;
                break;
            }
        }

        //Collision with a particle.
        if(sqrt((play.x_position - p.x)*(play.x_position - p.x) + (play.y_position - p.y)*(play.y_position - p.y)) < p.r + 10){
        	play.x_position = 0;
        	play.y_position = 0;
        	game_over = true;
        }

        double d =sqrt((play.x_position - p.x)*(play.x_position - p.x) + (play.y_position - p.y)*(play.y_position - p.y));

        play.x_velocity += 100*GRAV_CONSTANT*p.m / (d*d)  * (p.x - play.x_position)/d;
        play.y_velocity += 100*GRAV_CONSTANT*p.m / (d*d)  * (p.y - play.y_position)/d;

        if (notFall) {
            p.x += p.vx;
            p.y += p.vy;
        }
        else {
            particles.erase(particles.begin() + i);
        }
    }

    play.x_position += play.x_velocity/5;
    play.y_position += play.y_velocity/5;

    double angle = acos(play.x_orientation) * ((play.y_orientation >= 0)? 1 : -1);
    play.x_orientation = cos(angle - play.angular_velocity*PI/90);
    play.y_orientation = sin(angle - play.angular_velocity*PI/90);



    for(int j = 0; j < int(bullets.size()); ++j){
        bullet &b = bullets[j];
        if(abs(b.x) >= X_WINDOW_SIZE/2 || abs(b.y) >= Y_WINDOW_SIZE/2)
            bullets.erase(bullets.begin() + j);
        b.x += b.vx;
        b.y += b.vy;
    }


    if(abs(play.x_position) >= X_WINDOW_SIZE/2) play.x_position = -play.x_position;
    if(abs(play.y_position) >= Y_WINDOW_SIZE/2) play.y_position = -play.y_position;

    if(!game_over) glutTimerFunc(1, timer, 0);
    else display();
}


/*
 * Keyboard and Mouse Functions.
 */

void Game::_mouse(int button, int state, int x, int y) {
    if(state == GLUT_DOWN){
        curr_mouse_state = GLUT_DOWN;
        Mx = x - X_WINDOW_SIZE/2;
        My = y - Y_WINDOW_SIZE/2;

        switch(button){
            case GLUT_LEFT_BUTTON:
                leftClicked = state == GLUT_DOWN;
                break;
            case GLUT_RIGHT_BUTTON:
                rightClicked = state == GLUT_DOWN;
                break;
            case GLUT_MIDDLE_BUTTON:
                middleClicked = state == GLUT_DOWN;
                break;
        }
    }
    else{
        curr_mouse_state = GLUT_UP;
        Mx = x - X_WINDOW_SIZE/2;
        My = y - Y_WINDOW_SIZE/2;
        switch(button){
            case GLUT_LEFT_BUTTON:
                leftRelease = state == GLUT_UP;
                break;
            case GLUT_RIGHT_BUTTON:
                rightRelease = state == GLUT_UP;
                break;
            case GLUT_MIDDLE_BUTTON:
                middleRelease = state == GLUT_UP;
                break;
        }
    }
}

void Game::_mouseMotion(int x, int y) {
    if(curr_mouse_state == GLUT_DOWN){
        dx += x - X_WINDOW_SIZE/2 - Mx;
        dy += y - Y_WINDOW_SIZE/2 - My;
    }
    else{
        dx = 0;
        dy = 0;
    }
    Mx = x - X_WINDOW_SIZE/2;
    My = y - Y_WINDOW_SIZE/2;
}

/*
 Keyboard Functions.
 */

void Game::_keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27:
            exit(0);
            break;

        case 'w':
            play.up = true;
            break;

        case 's':
        	play.down = true;
            break;

        case 'a':
        	play.left = true;
            break;

        case 'd':
        	play.right = true;
            break;

        case 'c':
            removeParticles();
            break;

        case 32:
            bullet b(play.x_position, play.y_position, 10*play.x_orientation, 10*play.y_orientation);
            bullets.push_back(b);
    }
}

/*
 * Particle Functions.
 */

void Game::addParticle(double m, double r, double vx ,double vy) {
    double color[3];
    color[0] = rand() % 200 / 200.0;
    color[1] = rand() % 200 / 200.0;
    color[2] = rand() % 200 / 200.0;
    particle p(Mx, My, r, vx/20, vy/20, m, color);
    particles.push_back(p);
}

void Game::removeParticles() {
    for (int i = 0; i < int(particles.size()); i++)
        particles.pop_back();
}

void Game::addCircleofParticles(double m, double r){
    for(double t = 0; t <= 2*PI; t += PI/100){
        double color[3] = {1.0, 0, 0};
        particle p(r*cos(t), r*sin(t), cbrt(curr_mass), 0.001*r*sin(t), -0.001*r*cos(t), curr_mass, color);
        particles.push_back(p);
    }
}

void Game::_addRandomParticle(int size) {
    double x = rand() % X_WINDOW_SIZE - X_WINDOW_SIZE/2;
    double y = rand() % Y_WINDOW_SIZE - Y_WINDOW_SIZE/2;
    while(abs(x) < X_WINDOW_SIZE/2 && abs(y) < Y_WINDOW_SIZE/2){
        x = rand() % (100 + X_WINDOW_SIZE) - X_WINDOW_SIZE/2;
        y = rand() % (100 + Y_WINDOW_SIZE) - Y_WINDOW_SIZE/2;
    }

    double m = rand() % size;
    double vx = ((x > 0)? -1 : 1)*rand() % 2;
    double vy = ((y > 0)? -1 : 1)*rand() % 2;
    double color[3];
    for(int i = 0; i < 3; i++) color[i] = rand() % 200/200.0;
    particle p(x, y, 10*sqrt(m), vx, vy, m, color);
    particles.push_back(p);

    if(!game_over) glutTimerFunc(500, addRandomParticle, size);
}

void display(){
	current_game->_display();
}
void timer(int x){
	current_game->_timer(0);
}
void addRandomParticle(int size){
	current_game->_addRandomParticle(size);
}
void mouse(int button, int state, int x, int y){
	current_game->_mouse(button, state, x, y);
}
void mouseMotion(int x, int y){
	current_game->_mouseMotion(x, y);
}
void keyboard(unsigned char key, int x, int y){
	current_game->_keyboard(key, x, y);
}
