#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "doomu_chan/base.h"
#include "doomu_chan/render.h"

#define MAX_PATH_LENGTH 256

#define PI 3.14159265359

int win_width = 950;
int win_height = 620;

float px,py,pdx,pdy,pa;
float frame1,frame2,fps;

typedef struct
{
 int w,a,d,s;                     //button state on off
}ButtonKeys; ButtonKeys Keys;


//------------------------PLAYER------------------------------------------------
void drawPlayer2D()
{
    glColor3f(1,1,0);   glPointSize(8);    glLineWidth(4);
    glBegin(GL_POINTS); glVertex2i(px,py); glEnd();
    glBegin(GL_LINES);  glVertex2i(px,py); glVertex2i(px+pdx*20,py+pdy*20); glEnd();
}//-----------------------------------------------------------------------------


void init()
{
    glClearColor(0.3,0.3,0.3,0);
    gluOrtho2D(0,win_width,win_height,0);
    px=150; py=400; pa=90;
    pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));                                 //init player
}


void display()
{  
    //frames per second
    frame2=glutGet(GLUT_ELAPSED_TIME); fps=(frame2-frame1); frame1=glutGet(GLUT_ELAPSED_TIME); 

    //buttons
    if(Keys.a==1){ pa+=0.2*fps; pa=FixAng(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));} 	
    if(Keys.d==1){ pa-=0.2*fps; pa=FixAng(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));} 

    int xo=0; if(pdx<0){ xo=-20;} else{ xo=20;}                                    //x offset to check map
    int yo=0; if(pdy<0){ yo=-20;} else{ yo=20;}                                    //y offset to check map
    int ipx=px/64.0, ipx_add_xo=(px+xo)/64.0, ipx_sub_xo=(px-xo)/64.0;             //x position and offset
    int ipy=py/64.0, ipy_add_yo=(py+yo)/64.0, ipy_sub_yo=(py-yo)/64.0;             //y position and offset
    if(Keys.w==1)                                                                  //move forward
    {  
        if(mapW[ipy*mapX        + ipx_add_xo]==0){ px+=pdx*0.2*fps;}
        if(mapW[ipy_add_yo*mapX + ipx       ]==0){ py+=pdy*0.2*fps;}
    }
    if(Keys.s==1)                                                                  //move backward
    { 
        if(mapW[ipy*mapX        + ipx_sub_xo]==0){ px-=pdx*0.2*fps;}
        if(mapW[ipy_sub_yo*mapX + ipx       ]==0){ py-=pdy*0.2*fps;}
    } 

    glutPostRedisplay();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    //drawMap2D();
    //drawPlayer2D();
    drawRays2D(px,py,pdx,pdy,pa);
    glutSwapBuffers();
}

void ButtonDown(unsigned char key,int x,int y)                                  //keyboard button pressed down
{
    if(key=='a'){ Keys.a=1;} 	
    if(key=='d'){ Keys.d=1;} 
    if(key=='w'){ Keys.w=1;}
    if(key=='s'){ Keys.s=1;}
    if(key=='e')             //open doors
    { 
        int xo=0; if(pdx<0){ xo=-25;} else{ xo=25;}
        int yo=0; if(pdy<0){ yo=-25;} else{ yo=25;} 
        int ipx=px/64.0, ipx_add_xo=(px+xo)/64.0;
        int ipy=py/64.0, ipy_add_yo=(py+yo)/64.0;
        if(mapW[ipy_add_yo*mapX+ipx_add_xo]==4){ mapW[ipy_add_yo*mapX+ipx_add_xo]=0;}
    }

    glutPostRedisplay();
}

void ButtonUp(unsigned char key,int x,int y)                                    //keyboard button pressed up
{
    if(key=='a'){ Keys.a=0;} 	
    if(key=='d'){ Keys.d=0;} 
    if(key=='w'){ Keys.w=0;}
    if(key=='s'){ Keys.s=0;}
    glutPostRedisplay();
}

void resize(int w,int h)                                                        //screen window rescaled, snap back
{
    glutReshapeWindow(win_width,win_height);
}

int main(int argc, char* argv[])
{ 
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(win_width,win_height);
    glutInitWindowPosition(200,200);
    glutCreateWindow("Raycaster Testing");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(ButtonDown);
    glutKeyboardUpFunc(ButtonUp);
    glutMainLoop();
    return 0;
}