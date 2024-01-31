#include "base.h"

#ifndef RENDER_H
#define RENDER_H

void renderText(float x, float y, void* font, const char* string);
void renderStrokeText(void* font, const char* string, float x, float y, float scale);
void drawRays2D(float px,float py,float pdx,float pdy,float pa);

#endif