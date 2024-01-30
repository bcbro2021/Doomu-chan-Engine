#include <stdio.h>
#include "doomu_chan/window.h"
#include "doomu_chan/base.h"
#include <SDL2/SDL.h>
#include <math.h>

// map
int mapX=8;int mapY=8;int mapS=64;
int map[] =
{
    1,1,1,1,1,1,1,1,
    1,0,0,1,0,0,0,1,
    1,0,0,1,0,0,0,1,
    1,1,1,1,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,
};

void drawMap(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int y=0;y < mapY;y++) {
        for (int x=0;x < mapX;x++) {
            if (map[y*mapX+x] == 1) {
                SDL_Rect rect = {x*mapS,y*mapS,mapS-1,mapS-1};
                SDL_RenderFillRect(renderer,&rect);
            }
        }
    }
}


float distance(ax,ay,bx,by,ang){ return cos(degToRad(ang))*(bx-ax)-sin(degToRad(ang))*(by-ay);}
void drawRays2D(SDL_Renderer* renderer,int px,int py,float pdx,float dpy,float pa)
{
	
    int r,mx,my,mp,dof,side; float vx,vy,rx,ry,ra,xo,yo,disV,disH; 
    
    ra=FixAng(pa+30);                                                              //ray set back 30 degrees
    
    for(r=0;r<60;r++)
    {
        //---Vertical--- 
        dof=0; side=0; disV=100000;
        float Tan=tan(degToRad(ra));
        if(cos(degToRad(ra))> 0.001){ rx=(((int)px>>6)<<6)+64;      ry=(px-rx)*Tan+py; xo= 64; yo=-xo*Tan;}//looking left
        else if(cos(degToRad(ra))<-0.001){ rx=(((int)px>>6)<<6) -0.0001; ry=(px-rx)*Tan+py; xo=-64; yo=-xo*Tan;}//looking right
        else { rx=px; ry=py; dof=8;}                                                  //looking up or down. no hit  

        while(dof<8) 
        { 
            mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;                     
            if(mp>0 && mp<mapX*mapY && map[mp]==1){ dof=8; disV=cos(degToRad(ra))*(rx-px)-sin(degToRad(ra))*(ry-py);}//hit         
            else{ rx+=xo; ry+=yo; dof+=1;}                                               //check next horizontal
        }
        vx=rx; vy=ry;

        //---Horizontal---
        dof=0; disH=100000;
        Tan=1.0/Tan; 
        if(sin(degToRad(ra))> 0.001){ ry=(((int)py>>6)<<6) -0.0001; rx=(py-ry)*Tan+px; yo=-64; xo=-yo*Tan;}//looking up 
        else if(sin(degToRad(ra))<-0.001){ ry=(((int)py>>6)<<6)+64;      rx=(py-ry)*Tan+px; yo= 64; xo=-yo*Tan;}//looking down
        else{ rx=px; ry=py; dof=8;}                                                   //looking straight left or right
        
        while(dof<8) 
        { 
            mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;                          
            if(mp>0 && mp<mapX*mapY && map[mp]==1){ dof=8; disH=cos(degToRad(ra))*(rx-px)-sin(degToRad(ra))*(ry-py);}//hit         
            else{ rx+=xo; ry+=yo; dof+=1;}                                               //check next horizontal
        } 
        SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
        if(disV<disH){ rx=vx; ry=vy; disH=disV; SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);}

        //SDL_RenderDrawLine(renderer,px,py,rx,ry);
            
        int ca=FixAng(pa-ra); disH=disH*cos(degToRad(ca));                            //fix fisheye 
        int lineH = (mapS*320)/(disH); if(lineH>320){ lineH=320;}                     //line height and limit
        int lineOff = 160 - (lineH>>1);                                               //line offset

        SDL_Rect lineRect = { r * 8 + 300, lineOff+100, 8, lineH };
        SDL_RenderFillRect(renderer, &lineRect);

        //draw vertical wall  

        ra=FixAng(ra-1);                                                              //go to next ray
    }
}//-----------------------------------------------------------------------------


int main(int argc, char const *argv[])
{
    SDL_Window* window = create_win(1024,512,"Hello World!");

    SDL_Renderer* renderer = create_renderer(window);

    // def rect
    SDL_Rect player = {300,300,8,8};
    float pdx,pdy,pa;

    // loop
    const int targetFPS = 60;
    const int delayTime = 1000 / targetFPS;  // Time per frame in milliseconds
    Uint32 lastTime = SDL_GetTicks();
    float dt = 0.0f;

    Uint32 frameStart, frameTime;

    SDL_Event event;
    int quit = 0;
    while (!quit) {

        // events
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.scancode == SDL_SCANCODE_W) {
                    player.x+=pdx*5;
                    player.y+=pdy*5;
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_A) {
                    pa+=5;
                    pa=FixAng(pa);
                    pdx=cos(degToRad(pa));
                    pdy=-sin(degToRad(pa));
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_S) {
                    player.x-=pdx*5;
                    player.y-=pdy*5;
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_D) {
                    pa-=5;
                    pa=FixAng(pa);
                    pdx=cos(degToRad(pa));
                    pdy=-sin(degToRad(pa));
                }
            }
        }

        //Rendering
        clear_renderer(renderer,0,0,0,255);

        // draw map
        //drawMap(renderer);
        


        // draw player
        /*SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer,&player);
        SDL_RenderDrawLine(renderer,player.x,player.y,player.x+pdx*5,player.y+pdy*5);*/

        // draw rays
        drawRays2D(renderer,player.x,player.y,pdx,pdy,pa);        

        // update renderer
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < delayTime) {
            SDL_Delay(delayTime - frameTime);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
