#include<iostream>
#include<SDL/SDL_image.h>
#include<SDL/SDL_mixer.h>
#include<SDL/SDL.h>
#include<SDL/SDL_ttf.h>
#include<SDL/SDL_gfxPrimitives.h>
#include <string>
#include<vector>
using namespace std;
struct rckt
{
	int xp=225+10;
	int yp=680;
	int ymax=550;
	float vx=2;
	float vy=-4;
	int health=3;
	int shotnum=10;
	SDL_Surface * surface;
};
struct oppRocket
{
	int xp;
	int yp;
	int ymax;
	float vx;
	float vy;
	int health;
	int state;
	int height;
	int width;
	SDL_Surface * surface;
};
struct gft
{
	int xp;
	int yp;
	int r;
	float vy;
	int state;
	int time;
	SDL_Surface * surface;
};
struct meteor
{
	int xp;
	int yp;
	int r;
	float vy;
	SDL_Surface * surface;
};
struct coin
{
	int xp;
	int yp;
	int r;
	float vx;
	float vy;
	SDL_Surface * surface;
};
struct ammo
{
	int xp;
	int yp;
	int r;
	float vy;
	SDL_Surface * surface;
};
SDL_Surface * load_image (std::string file);
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );
void gift_motion (gft * gift ,int n,SDL_Surface * screen);
void Gift(gft *gift,int n, SDL_Surface *screen);
void Rocket(rckt &rocket,vector <ammo> &shot,SDL_Event event,SDL_Surface * screen);
int main()
{
	int bgX = 0, bgy = 0;
	SDL_Event event;	
	SDL_Surface * screen=NULL;
	SDL_Surface * picture;
	screen=SDL_SetVideoMode( 450, 700, 32, 0 );
	gft gift[5];
	rckt rocket;
	rocket.surface=load_image("rocket-r.png");
	picture=load_image("stars1.jpg");
	vector <ammo> shot;
	for(int i=0;i<5;i++)
	{
		gift[i].yp=0;
		gift[i].xp=10+(10*i);
		gift[i].vy=i+1;
		gift[i].state=2;
		gift[i].surface=load_image("gift0.png");
	}
	while(1)
	{
		bgy+=2;
		if(bgy>=picture->h)
		{
			bgy=0;
		}
	  	apply_surface( bgX, bgy, picture, screen ); 
      	apply_surface( bgX , bgy-picture->h, picture, screen );
    	Gift(&gift[0],5, screen);
    	Rocket(rocket,shot,event,screen);
		SDL_Flip(screen);
		SDL_Delay(20);
	}
	return 0;
}

SDL_Surface * load_image (std::string file)
{

	SDL_Surface * image=NULL;
	image=IMG_Load( file.c_str() );
	return image;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( source, NULL, destination, &offset );
}
void Gift(gft * gift,int n,SDL_Surface * screen )
{
	for(int i=0 ; i<n ; i++)
	{
		if(gift[i].state == 2)
		{
			gift[i].yp +=gift[i].vy;
			if(gift[i].yp >= 700)
			{
				gift[i].state = 0;
				return;
			}
			apply_surface(gift[i].xp , gift[i].yp , gift[i].surface,screen);	
		}
		else 
		{
			if(gift[i].state==1)
			{
				gift[i].time--;
				if(gift[i].time==0)
					gift[i].state=0;
			}
			else
			{
				if(rand()%10>6)
				{
					gift[i].xp=rand()%450;
					gift[i].yp=0;
					gift[i].state=2;
					gift[i].vy=rand()%3+1;

				} 
			}

		}
	}
	
}
void Rocket(rckt &rocket,vector <ammo> &shot,SDL_Event event,SDL_Surface * screen) 
{ 
    if(rocket.vy!=0)//in tike baraye avale bazie k safine amoodi harekat mikone 
    { 
    	rocket.yp+=rocket.vy;
    	if(rocket.yp<=rocket.ymax) 
       		rocket.vy=0; 
    } 
    else//harekate amooodi 
    { 
  		Uint8 *keystates = SDL_GetKeyState( NULL );
    	if( SDL_PollEvent( &event ) ) //do halat ro barresi mikone k :
   		{ 
        	if( event.type == SDL_KEYDOWN ) //1.jahate safine avaz beshe
        	{ 
            	if( keystates[ SDLK_SPACE ]) 
    			{ 
                	rocket.vx*= (-1); 
          			if(rocket.vx>0)
          				rocket.surface=load_image("rocket-r.png");
      				else
      					rocket.surface=load_image("rocket-l.png");

        		} 
        		else if(keystates[ SDLK_UP ] && rocket.shotnum>0)//2.tir shelik kone
        		{ 
         			rocket.shotnum--;
        			ammo sht; 
        			sht.xp=rocket.xp; 
        			sht.yp=rocket.yp; 
        			sht.vy=2;
        			sht.surface=load_image("shot_up.png"); 
        			shot.push_back(sht); 
       			} 
        	} 
    	}  
    	rocket.xp+=rocket.vx;
    }  
    /*if(rocket_strike()==true) 
    { 
       rocket.health--;
    }*/ 
    apply_surface( rocket.xp, rocket.yp, rocket.surface, screen ); 
    return; 
} 
//////////////////////////////
/*
rocket strike:
1.shahab
3.gift
shot:
1.motion
2.strike
opp:
1.exsist
2.motion
3.shot
*/
