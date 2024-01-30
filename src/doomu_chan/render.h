#ifndef RENDER_H
#define RENDER_H

int mapX;      //map width
int mapY;     //map height
int mapS;     //map cube size
int mapW[64];
int mapF[64];
int mapC[64];

void drawRays2D(float px,float py,float pdx,float pdy,float pa);
void drawMap2D();

#endif