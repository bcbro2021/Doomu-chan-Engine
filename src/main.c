#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include "doomu_chan/base.h"
#include "doomu_chan/render.h"
#include "doomu_chan/window.h"
#include <stdbool.h>

#define MAX_PATH_LENGTH 256

#define PI 3.14159265359

float px, py, pdx, pdy, pa;
float frame1, frame2, fps;
char *scene;

typedef struct {
    int w, a, d, s; // button state on off
} ButtonKeys;

typedef struct {
    int x, y; // mouse coordinates
} Mouse;

typedef struct {
    float x;
    float y;
    float width;
    float height;
} Rectangle;

ButtonKeys Keys;
Mouse mouse;

Rectangle playBtn;
bool playBtnColliding;

void mouseMotion(int x, int y) {
    // Store the new mouse coordinates
    mouse.x = x;
    mouse.y = y;
}

bool pointRectCollision(Mouse point, Rectangle rect) {
    return (point.x >= rect.x && point.x <= rect.x + rect.width &&
            point.y >= rect.y && point.y <= rect.y + rect.height);
}


void move_and_collide() {

    int xo = 0; if (pdx < 0) { xo = -20; } else { xo = 20; } // x offset to check map
    int yo = 0; if (pdy < 0) { yo = -20; } else { yo = 20; } // y offset to check map
    int ipx = px / 64.0, ipx_add_xo = (px + xo) / 64.0, ipx_sub_xo = (px - xo) / 64.0; // x position and offset
    int ipy = py / 64.0, ipy_add_yo = (py + yo) / 64.0, ipy_sub_yo = (py - yo) / 64.0; // y position and offset

    // buttons
    if (Keys.a == 1) {
        // Left strafing
        float new_px = px + pdy * 0.2 * fps;
        float new_py = py - pdx * 0.2 * fps;
        int new_ipx = new_px / 64.0;
        int new_ipy = new_py / 64.0;

        if (mapW[new_ipy * mapX + new_ipx] == 0) {
            px = new_px;
            py = new_py;
        }
    }
    if (Keys.d == 1) {
        // Right strafing
        float new_px = px - pdy * 0.2 * fps;
        float new_py = py + pdx * 0.2 * fps;
        int new_ipx = new_px / 64.0;
        int new_ipy = new_py / 64.0;

        if (mapW[new_ipy * mapX + new_ipx] == 0) {
            px = new_px;
            py = new_py;
        }
    }

    if (Keys.w == 1) // move forward
    {
        if (mapW[ipy * mapX + ipx_add_xo] == 0) { px += pdx * 0.2 * fps; }
        if (mapW[ipy_add_yo * mapX + ipx] == 0) { py += pdy * 0.2 * fps; }
    }
    if (Keys.s == 1) // move backward
    {
        if (mapW[ipy * mapX + ipx_sub_xo] == 0) { px -= pdx * 0.2 * fps; }
        if (mapW[ipy_sub_yo * mapX + ipx] == 0) { py -= pdy * 0.2 * fps; }
    }

    // Center the mouse at the window's center only if it has moved
    if (mouse.x != win_width / 2 || mouse.y != win_height / 2) {
        glutWarpPointer(win_width / 2, win_height / 2);
        // Update the player's angle based on the mouse movement
        int dx = mouse.x - win_width / 2;
        pa += -dx * 0.08; // Adjust the sensitivity as needed
        pa = FixAng(pa);
        pdx = cos(degToRad(pa));
        pdy = -sin(degToRad(pa));
    }
}

void display() {
    // frames per second
    frame2 = glutGet(GLUT_ELAPSED_TIME);
    fps = (frame2 - frame1);
    frame1 = glutGet(GLUT_ELAPSED_TIME);

    // movement and menu update
    if (scene == "game") {
        move_and_collide();
    } 
    
    else if (scene == "menu") {
        pa += 0.2;
        if (pointRectCollision(mouse,playBtn)) {
            playBtnColliding = true;
            printf("Hello WOrld");
        } else {
            playBtnColliding = false;
        }
    }

    glutPostRedisplay();

    // drawing
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawRays2D(px, py, pdx, pdy, pa);

    if (scene == "game") {
        glColor3f(1.0,1.0,1.0);
        char coords[1000];
        sprintf(coords,"x: %f y: %f a: %f",px,py,pa);
        renderText(20,20,GLUT_BITMAP_HELVETICA_18,coords);
    } 
    if (scene == "menu") {
        glColor3f(1.0,1.0,1.0);
        renderStrokeText(GLUT_STROKE_ROMAN,"DOOMU CHAN",250,200,0.5);

        glColor3f(1.0, 1.0, 1.0); // Set rectangle color to white

        // Draw a rectangle with width 100, height 100, and position (50, 50)
        glRectf(playBtn.x, playBtn.y, playBtn.x + playBtn.width, playBtn.y + playBtn.height);

        glColor3f(0.0,0.0,0.0);
        renderText(playBtn.x + playBtn.width/2 - 18,playBtn.y + playBtn.height/2 +4,GLUT_BITMAP_HELVETICA_18,"Play");
    }
    
    glutSwapBuffers();
}

void ButtonDown(unsigned char key, int x, int y) // keyboard button pressed down
{
    if (key == 'a') { Keys.a = 1; }
    if (key == 'd') { Keys.d = 1; }
    if (key == 'w') { Keys.w = 1; }
    if (key == 's') { Keys.s = 1; }
    if (key == 'e') // open doors
    {
        int xo = 0; if (pdx < 0) { xo = -25; } else { xo = 25; }
        int yo = 0; if (pdy < 0) { yo = -25; } else { yo = 25; }
        int ipx = px / 64.0, ipx_add_xo = (px + xo) / 64.0;
        int ipy = py / 64.0, ipy_add_yo = (py + yo) / 64.0;
        if (mapW[ipy_add_yo * mapX + ipx_add_xo] == 4) { mapW[ipy_add_yo * mapX + ipx_add_xo] = 0; }
    }
    if (key == 27) // ASCII code for escape key
    {
        exit(0); // Exit the game
    }
    glutPostRedisplay();
}

void ButtonUp(unsigned char key, int x, int y) // keyboard button pressed up
{
    if (key == 'a') { Keys.a = 0; }
    if (key == 'd') { Keys.d = 0; }
    if (key == 'w') { Keys.w = 0; }
    if (key == 's') { Keys.s = 0; }
    glutPostRedisplay();
}

void init_menu() {
    scene="menu";
    px = 300; py = 430; pa = 90;
    pdx = cos(degToRad(pa)); pdy = -sin(degToRad(pa)); // init player

    // init map
    create_map("mainmenu");

    // init button
    playBtn.x = 425;
    playBtn.y = 250;
    playBtn.width = 100;
    playBtn.height = 40;

    glutPassiveMotionFunc(mouseMotion);
}

void init_game() {
    scene="game";
    px = 300; py = 430; pa = 90;
    pdx = cos(degToRad(pa)); pdy = -sin(degToRad(pa)); // init player

    // init map
    create_map("test");

    // Enable continuous mouse motion
    glutSetCursor(GLUT_CURSOR_NONE);
}

void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (playBtnColliding) {
            init_game();
        }
    }
}

int main(int argc, char* argv[]) {
    
    glutInit(&argc, argv);
    init_window(win_width,win_height,"Doomu Chan");
    init_menu();


    glutMouseFunc(mouseClick);
    glutDisplayFunc(display);
    glutReshapeFunc(resize_window);
    glutKeyboardFunc(ButtonDown);
    glutKeyboardUpFunc(ButtonUp);

    glutMainLoop();
    return 0;
}