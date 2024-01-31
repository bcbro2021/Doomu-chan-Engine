#ifndef WINDOW_H
#define WINDOW_H

int win_width;
int win_height;

void init_window(int width, int height, const char* title);
void resize_window(int width, int height);

#endif