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
struct oppRckt
{
	int xp=255+12;
	int yp=0;
	int ymin=50;
	float vx=1;
	float vy=4;
	int health=3;
	int state=0;
	int height=24;
	int width=100;
	SDL_Surface * surface;
};
struct gft
{
	int xp;
	int yp=0;
	int r;
	float vy;
	int state=0;
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
	int valu=1;
	float vx;
	float vy;
	SDL_Surface * surface;
};
struct ammo
{
	int xp;
	int yp;
	int r=10;
	float vy;
	SDL_Surface * surface;
};
SDL_Surface * load_image (std::string file);
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );
void gift_motion (gft * gift ,int n,SDL_Surface * screen);
void Gift(gft *gift,int n, SDL_Surface *screen);
void Rocket(rckt &rocket,vector <ammo> &shot,SDL_Event event,SDL_Surface * screen);
void OppRocket(oppRckt &oppRocket,vector <ammo> &shot, SDL_Surface *screen);
void Shot(vector <ammo>& shot,SDL_Surface *screen);

int main()
{
	int bgX = 0, bgy = 0;
	SDL_Event event;	
	SDL_Surface * screen=NULL;
	SDL_Surface * picture;
	screen=SDL_SetVideoMode( 450, 700, 32, 0 );
	gft gift[5];
	rckt rocket;
	oppRckt oppRocket;
	oppRocket.surface=load_image("spacecraft.png");
	rocket.surface=load_image("rocket-r.png");
	picture=load_image("stars1.jpg");
	vector <ammo> shot;
	for(int i=0;i<5;i++)
	{
		gift[i].xp=10+(10*i);
		gift[i].vy=i+1;
	}
	gift[0].surface=load_image("gift0.png");
	gift[1].surface=load_image("gift1.png");
	gift[2].surface=load_image("gift2.png");
	gift[3].surface=load_image("gift3.png");
	gift[4].surface=load_image("gift4.png");
	gift[5].surface=load_image("gift5.png");
	gift[6].surface=load_image("gift6.png");
	while(1)
	{
		bgy+=2;
		if(bgy>=picture->h)
		{
			bgy=0;
		}
	  	apply_surface( bgX, bgy, picture, screen ); 
      	apply_surface( bgX , bgy-picture->h, picture, screen );
    	//Gift(&gift[0],5, screen);
    	//Rocket(rocket,shot,event,screen);
    	OppRocket(oppRocket,shot,screen);
    	Shot(shot,screen);
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
				if(rand()%20+rand()%20==38)
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
        			sht.vy=-2;
        			sht.surface=load_image("shot-up.png"); 
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
void OppRocket(oppRckt &oppRocket,vector <ammo> &shot, SDL_Surface *screen)
{
	if(oppRocket.state==0)
	{
		//if((score%200<3 || score%200>197) && score>197 )
		{
			oppRocket.state=1;
			oppRocket.vy=2;
			oppRocket.health=3;
		}
	}
	else
	{
		if(oppRocket.vy!=0)//hengame vared shodan b safhe
		{
			oppRocket.yp+=oppRocket.vy	;
			if(oppRocket.yp>oppRocket.ymin)
				oppRocket.vy=0;
		}
		else//bad az mostaghar shodan
		{
			if(oppRocket.xp<=0 || oppRocket.xp>=(450-oppRocket.width))	
			{
				oppRocket.vx*=-1;
			}
			oppRocket.xp+=oppRocket.vx;
		}
		if(rand()%200==4)
		{
			ammo sht;
			sht.xp=oppRocket.xp-oppRocket.vx+(oppRocket.width/2);
			sht.yp=oppRocket.yp;
			sht.vy=4;
			sht.surface=load_image("shot-down.png");
			shot.push_back(sht);
		}
	}
	apply_surface(oppRocket.xp,oppRocket.yp,oppRocket.surface,screen);
}
void Shot(vector <ammo>& shot,SDL_Surface *screen) 	
{
   	if(shot.size()!=0)
	for(int i=0;i<shot.size()-1;i++)
	{
		shot[i].yp+=shot[i].vy;
		apply_surface(shot[i].xp,shot[i].yp,shot[i].surface,screen);
		if((shot[i].vy>0 && shot[i].yp==700-40) || (shot[i].vy<0 && shot[i].yp==0))
			shot.erase(shot.begin()+i);
	}
}
//////////////////////////////
/*
rocket strike:
1.shahab
3.gift
shot:
	1.strike
	2.start point
coin:
	1.exsict
	2.motion
score:
menu:
	1.start
	2.pause
	3.sound
	4.end
graphic:
	1.coin
	2.score
	3.heart
	4.shot
afzayesh deghat va sakhti bazi
*/
