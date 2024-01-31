#ifndef BASE_H
#define BASE_H

#define MAP_SIZE 81

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

typedef struct {
    float x,y,dx,dy,a;
} Player;

typedef struct {
    int type;
    int state;
    int map;
    int x,y,z;
} Sprite;


int All_Textures[16384];

float degToRad(float a);
float FixAng(float a);
float distance(float ax, float ay, float bx, float by, float ang);
void readMapFromFile(const char* filename, int* mapArray);

int mapX;      //map width
int mapY;     //map height
int mapS;     //map cube size
int mapW[MAP_SIZE];
int mapF[MAP_SIZE];
int mapC[MAP_SIZE];

void create_map(const char* map_dir);

#endif