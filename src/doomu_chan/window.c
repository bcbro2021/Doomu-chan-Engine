#include "window.h"
#include "base.h"
#include <GL/glut.h>

win_width = 960;
win_height = 640;

void init_window(int width, int height, const char* title) {
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(200, 200);
    glutCreateWindow(title);
    glClearColor(0.3, 0.3, 0.3, 0);
    gluOrtho2D(0, win_width, win_height, 0);
}

void resize_window(int w, int h) // screen window rescaled, snap back
{
    glutReshapeWindow(win_width, win_height);
}