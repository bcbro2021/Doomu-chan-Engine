#include "render.h"
#include "base.h"
#include "../../assets/sprites.ppm"
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

void renderText(float x, float y, void* font, const char* string) {
    glRasterPos2f(x, y);

    while (*string) {
        glutBitmapCharacter(font, *string);
        string++;
    }
}

void renderStrokeText(void* font, const char* string, float x, float y, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, -scale, 1.0);

    while (*string) {
        glutStrokeCharacter(font, *string);
        string++;
    }

    glPopMatrix();
}

//---------------------------Draw Rays and Walls--------------------------------
void drawRays2D(float px,float py,float pdx,float pdy,float pa)
{
int r,mx,my,mp,dof,side; float vx,vy,rx,ry,ra,xo,yo,disV,disH; 
    
    ra=FixAng(pa+30);                                                              //ray set back 30 degrees
 
    for(r=0;r<120;r++)
    {
        int vmt=0,hmt=0;                                                              //vertical and horizontal map texture number 
        //---Vertical--- 
        dof=0; side=0; disV=100000;
        float Tan=tan(degToRad(ra));
        if(cos(degToRad(ra))> 0.001){ rx=(((int)px>>6)<<6)+64;      ry=(px-rx)*Tan+py; xo= 64; yo=-xo*Tan;}//looking left
        else if(cos(degToRad(ra))<-0.001){ rx=(((int)px>>6)<<6) -0.0001; ry=(px-rx)*Tan+py; xo=-64; yo=-xo*Tan;}//looking right
        else { rx=px; ry=py; dof=8;}                                                  //looking up or down. no hit  

        while(dof<8) 
        { 
            mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;                     
            if(mp>0 && mp<mapX*mapY && mapW[mp]>0){ vmt=mapW[mp]-1; dof=8; disV=cos(degToRad(ra))*(rx-px)-sin(degToRad(ra))*(ry-py);}//hit         
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
            if(mp>0 && mp<mapX*mapY && mapW[mp]>0){ hmt=mapW[mp]-1; dof=8; disH=cos(degToRad(ra))*(rx-px)-sin(degToRad(ra))*(ry-py);}//hit         
            else{ rx+=xo; ry+=yo; dof+=1;}                                               //check next horizontal
        } 
        
        float shade=1;
        glColor3f(0,0.8,0);
        if(disV<disH){ hmt=vmt; shade=0.5; rx=vx; ry=vy; disH=disV; glColor3f(0,0.6,0);}//horizontal hit first
        //glLineWidth(2); glBegin(GL_LINES); glVertex2i(px,py); glVertex2i(rx,ry); glEnd();//draw 2D ray
            
        int ca=FixAng(pa-ra); disH=disH*cos(degToRad(ca));                            //fix fisheye 
        int lineH = (mapS*640)/(disH); 
        float ty_step=32.0/(float)lineH; 
        float ty_off=0; 
        if(lineH>640){ ty_off=(lineH-640)/2.0; lineH=640;}                            //line height and limit
        int lineOff = 320 - (lineH>>1);                                           //line offset

        depth[r]=disH; // line depth
        //---draw walls---
        int xOffset = 0;
        int y;
        float ty=ty_off*ty_step+hmt*32;
        float tx;
        if(shade==1){ tx=(int)(rx/2.0)%32; if(ra>180){ tx=31-tx;}}  
        else        { tx=(int)(ry/2.0)%32; if(ra>90 && ra<270){ tx=31-tx;}}
        for(y=0;y<lineH;y++)
        {
            float c=All_Textures[(int)(ty)*32 + (int)(tx)]*shade;
            if(hmt==0){ glColor3f(c    , c    , c/2.0); } //checkerboard red
            if(hmt==1){ glColor3f(c    , c/2.0, c/2.0);} //Brick yellow
            if(hmt==2){ glColor3f(c/2.0, c/2.0, c    );} //window blue
            if(hmt==3){ glColor3f(c/2.0, c    , c/2.0);} //door green
            glPointSize(8);glBegin(GL_POINTS);glVertex2i(r*8+xOffset,y+lineOff);glEnd();//draw vertical wall  
            ty+=ty_step;
        }
        
        //---draw floors---
        for(y=lineOff+lineH;y<640;y++)
        {
            float dy=y-(640/2.0), deg=degToRad(ra), raFix=cos(degToRad(FixAng(pa-ra)));
            tx=px/2 + cos(deg)*158*2*32/dy/raFix;
            ty=py/2 - sin(deg)*158*2*32/dy/raFix;
            int mp=mapF[(int)(ty/32.0)*mapX+(int)(tx/32.0)]*32*32;
            float c=All_Textures[((int)(ty)&31)*32 + ((int)(tx)&31)+mp]*0.7;
            glColor3f(c/1.3,c/1.3,c);glPointSize(8);glBegin(GL_POINTS);glVertex2i(r*8+xOffset,y);glEnd();

            //---draw ceiling---
            mp=mapC[(int)(ty/32.0)*mapX+(int)(tx/32.0)]*32*32;
            c=All_Textures[((int)(ty)&31)*32 + ((int)(tx)&31)+mp]*0.7;
            glColor3f(c/2.0,c/1.2,c/2.0);glPointSize(8);glBegin(GL_POINTS);glVertex2i(r*8+xOffset,640-y);glEnd();
        }
        
        ra=FixAng(ra-0.5);                                                               //go to next ray, 60 total
    }
}

void drawSprite(Sprite sp, Player p) {
int x,y;
  float sx=sp.x-p.x; //temp float variables
  float sy=sp.y-p.y;
  float sz=sp.z;

  float CS=cos(degToRad(p.a)), SN=sin(degToRad(p.a)); //rotate around origin
  float a=sy*CS+sx*SN; 
  float b=sx*CS-sy*SN; 
  sx=a; sy=b;

  sx=(sx*108.0/sy)+(120/2); //convert to screen x,y
  sy=(sz*108.0/sy)+( 80/2);

  int scale=32*80/b;   //scale sprite based on distance
  if(scale<0){ scale=0;} if(scale>120){ scale=120;}

  //texture
  float t_x=0, t_y=31, t_x_step=31.5/(float)scale, t_y_step=32.0/(float)scale;

  for(x=sx-scale/2;x<sx+scale/2;x++)
  {
   t_y=31;
   for(y=0;y<scale;y++)
   {
    if(sp.state==1 && x>0 && x<120 && b<depth[x])
    {
     int pixel=((int)t_y*32+(int)t_x)*3+(sp.map*32*32*3);
     int red   =sprites[pixel+0];
     int green =sprites[pixel+1];
     int blue  =sprites[pixel+2];
     if(red!=184, green!=0, blue!=255) //dont draw if purple
     {
      glPointSize(8); glColor3ub(red,green,blue); glBegin(GL_POINTS); glVertex2i(x*8,sy*8-y*8); glEnd(); //draw point 
     }
     t_y-=t_y_step; if(t_y<0){ t_y=0;}
    }
   }
   t_x+=t_x_step;
  }
}