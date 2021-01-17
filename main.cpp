//
//  main.cpp
//  AsteroidsAI
//
//  Created by Reid Harris on 12/8/20.
//  Copyright © 2020 Reid Harris. All rights reserved.
//

#include "game/player.hpp"

int main(int argc, char **argv) {
	Game G;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(X_WINDOW_SIZE, Y_WINDOW_SIZE);
    glutCreateWindow("Gravity simulation");
    G.init();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);
	timer(0);
	addRandomParticle(100);

    glutMainLoop();
    return 1;
}




