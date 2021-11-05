#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#ifdef WIN32
#include <windows.h>
#include <conio.h>
#endif

#define WIDTH 96
#define HEIGHT 48
#define FPS 30.f
#define GRAVITY 2.0f
#define DECELERATION 0.65f
#define MARGIN 0.05f


typedef enum PIXEL{
		   MPTY,
		   FULL
} PIXEL;

static PIXEL display[WIDTH*HEIGHT];
static char pixels[4] = " .'O";



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
  #ifdef _WIN32
  COORD coord = {.X = x, .Y = y};
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
  #else
  printf("\033[%d;%dH", y, x);
  #endif
}

void hide(int o){
  #ifndef WIN32
  if(o)
    printf("\e[?25h");
  else
    printf("\e[?251");
  #endif
}

void back(){
  move(0, 0);
}

void csleep(int milliseconds){
  #ifdef _WIN32
  Sleep(milliseconds);
  #else
  float fmilliseconds = milliseconds/1000;
  sleep(fmilliseconds);
  #endif
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
        }
      }
    }
  }
}

void drawDisplay(){
  char row[WIDTH];
  for(int y = 0; y < HEIGHT/2; ++y){
    for(int x = 0; x < WIDTH; ++x){
      PIXEL t = display[x + (2*y+0)*WIDTH];
      PIXEL b = display[x + (2*y+1)*WIDTH];
      row[x] = (pixels[b + t*2]);
    }
    fwrite(row, sizeof(char), WIDTH, stdout);
    putchar('\n');
  }
}

int main(){

  clear();
  clearDisplay();
  back();
  hide(1);
  ball b = initBall(10, 10, 8);
  v2 vel = initV2(3, 0.1);
  
  while(vel.y != 0 || vel.x != 0){

    if(vel.x < MARGIN && vel.x > -MARGIN)
      vel.x = 0;
    if(vel.y < MARGIN && vel.y > -MARGIN)
      vel.y = 0;


    vel.y += (GRAVITY/FPS)*(b.center.y+b.radius-HEIGHT < 0);
    
    b.center.x += vel.x;
    b.center.y += vel.y;

    if(b.center.x+b.radius > WIDTH || b.center.x+b.radius+MARGIN > WIDTH){
      b.center.x = WIDTH - b.radius;
      vel.x *= -DECELERATION;;
    }
    if(b.center.x-b.radius < 0 || b.center.x-b.radius-MARGIN < 0){
      b.center.x = b.radius;
      vel.x *= -DECELERATION;
    }
    if(b.center.y+b.radius > HEIGHT || b.center.y+b.radius+MARGIN > HEIGHT){
      b.center.y = HEIGHT - b.radius;
      vel.y *= -DECELERATION;
      vel.x *= 1.5*DECELERATION;
    }
    
    back();
    drawBall(b);
    drawDisplay();
    csleep(1000/FPS);
  }
  
  return 0;
}
