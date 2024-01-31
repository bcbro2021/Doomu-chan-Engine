#ifndef BASE_H
#define BASE_H

int All_Textures[16384];

float degToRad(float a);
float FixAng(float a);
float distance(float ax, float ay, float bx, float by, float ang);
void readMapFromFile(const char* filename, int* mapArray, int mapSize);

int mapX;      //map width
int mapY;     //map height
int mapS;     //map cube size
int mapW[81];
int mapF[81];
int mapC[81];

void create_map(const char* map_dir);

#endif