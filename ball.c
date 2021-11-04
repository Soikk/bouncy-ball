#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define WIDTH 96
#define HEIGHT 48
#define FPS 30
#define GRAVITY 2.0f
#define DECELERATION 0.65f


typedef enum PIXEL{
		   MPTY,
		   FULL
} PIXEL;

static PIXEL display[WIDTH*HEIGHT];
static char pixels[4] = " -^C";



typedef struct v2{
  float x;
  float y;
} v2;

typedef struct ball{
  v2 center;
  float radius;
} ball;


void clear(){
  #ifdef WIN32
  system("cls");
  #else
  system("clear");
  #endif
}

void move(int x, int y){
  printf("\033[%d;%dH", y, x);
}

void hide(int o){
  if(o)
    printf("\e[?25h");
  else
    printf("\e[?251");
}

void back(){
  move(0, 0);
}

void clearDisplay(){
  for(int y = 0; y < HEIGHT; ++y){
    for(int x = 0; x < WIDTH; ++x){
      display[x + y*WIDTH] = MPTY;
    }
  }
}

v2 initV2(float x, float y){
  v2 newV2;
  newV2.x = x;
  newV2.y = y;
  return newV2;
}

ball initBall(float x, float y, float radius){
  ball newBall;
  newBall.center = initV2(x, y);
  newBall.radius = radius;
  return newBall;
}

void drawBall(ball b){
  clearDisplay();
  v2 min = initV2(floor(b.center.x-b.radius), floor(b.center.y-b.radius));
  v2 max = initV2(ceil(b.center.x+b.radius), ceil(b.center.y+b.radius));
  for(int y = min.y; y < max.y; ++y){
    for(int x = min.x; x < max.x; ++x){
      float px = b.center.x - x - 0.5f, py = b.center.y - y - 0.5f;
      if((px*px + py*py) <= b.radius*b.radius){
	if(x >= 0 && x <= WIDTH && y >= 0 && y <= HEIGHT){
	  display[x + y*WIDTH] = FULL;
	};
      }
    }
  }
}

void drawDisplay(){
  for(int y = 0; y < HEIGHT/2; ++y){
    for(int x = 0; x < WIDTH; ++x){
      PIXEL t = display[x + (2*y+0)*WIDTH];
      PIXEL b = display[x + (2*y+1)*WIDTH];
      putchar(pixels[b + t*2]);
    }
    putchar('\n');
  }
}

int main(){
  clear();
  clearDisplay();
  back();
  hide(0);
  ball b = initBall(10, 10, 8);
  v2 vel = initV2(0, 0.1);
  
  while(vel.y != 0){
    
    int v = 1;
    if(b.center.y+b.radius-HEIGHT == 0)
      v = 0;
    
    vel.y += (GRAVITY/FPS)*v;
    b.center.x += vel.x;
    b.center.y += vel.y;
    if(v == 0){
      printf("CERO: %f\n", vel.y);
    }
    printf("v: %f\n", vel.y);
    getchar();
    if(b.center.y+b.radius > HEIGHT){
      b.center.y = HEIGHT - b.radius;
      //b.center.y -= floor(b.radius*(vel.y*(GRAVITY/FPS)));
      float a = vel.y;
      vel.y *= -DECELERATION;
      printf("\n%f *= -%f = %f", a, DECELERATION, vel.y);
      getchar();
    }

    
    clear();
    back();
    drawBall(b);
    drawDisplay();

   
    usleep(1000*1000/FPS);
  }
  
  return 0;
}
