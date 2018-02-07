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
	float vx=6;
	float vy=-6;
	int health=3;
	int shotnum=10;
	SDL_Surface * surface;
};
struct oppRckt
{
	int xp=255+12;
	int yp=0;
	int ymin=50;
	float vx=4;
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
	int kind;
	float vy;
	SDL_Surface * surface;
};
struct cn
{
	int xp;
	int yp=0;
	int r=5;
	int valu=1;
	int vx=0;
	int vy=3;
	int zarib=1;
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
//void gift_motion (gft * gift ,int n,SDL_Surface * screen,);
void Gift(gft *gift,int n, SDL_Surface *screen,int roundNum);
void Rocket(rckt &rocket,vector <ammo> &shot,SDL_Event &event,SDL_Surface * screen);
void OppRocket(oppRckt &oppRocket,vector <ammo> &shot, SDL_Surface *screen);
void Shot(vector <ammo>& shot,SDL_Surface *screen);
bool menu(SDL_Event &event);
void Meteor(vector <meteor> & meteorite,SDL_Surface *screen,int roundNum);
void Coin(vector <cn> &coin,SDL_Surface * screen,gft gift5,int xrocket,int yrocket,int roundNum);

int main()
{
	int bgX = 0, bgy = 0;
	int roundNum=0;
	SDL_Event event;	
	SDL_Surface * screen=NULL;
	SDL_Surface * picture;
	gft gift[5];
	rckt rocket;
	oppRckt oppRocket;
	vector <ammo> shot;
	vector <meteor> meteorite;
	vector <cn> coin;
	screen=SDL_SetVideoMode( 650, 700, 32, 0 );
	thickLineRGBA(screen,451,0,451,700,3,200,0,200,100);
	oppRocket.surface=load_image("spacecraft.png");
	rocket.surface=load_image("rocket-r.png");
	picture=load_image("stars1.jpg");
	bool meniu=true;
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
	do
	{
		roundNum++;
		bgy+=2;
		if(bgy>=picture->h)
		{
			bgy=0;
		}
	  	apply_surface( bgX, bgy, picture, screen ); 
      	apply_surface( bgX , bgy-picture->h, picture, screen );
    	Gift(&gift[0],5, screen,roundNum);
    	Rocket(rocket,shot,event,screen);
    	OppRocket(oppRocket,shot,screen);
    	Meteor(meteorite,screen,roundNum);
    	Shot(shot,screen);
    	Coin(coin,screen,gift[5],rocket.xp,rocket.yp,roundNum);
    	//meniu=menu(event);
		SDL_Flip(screen);
		SDL_Delay(20);
	}while(meniu==true);
	SDL_Delay(3000);
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
void Gift(gft * gift,int n,SDL_Surface * screen ,int roundNum )
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
				if(rand()%400==4)
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
void Rocket(rckt &rocket,vector <ammo> &shot,SDL_Event &event,SDL_Surface * screen) 
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
void Meteor(vector <meteor> & meteorite,SDL_Surface *screen,int roundNum)
{
	if(roundNum%(40-(int)(roundNum/1500))==0)
	{
		cout<<roundNum/1500<<endl;
		meteor mtr;
		if(rand()%2==0)
		{
			mtr.xp=rand()%450;
			mtr.r=20;
			mtr.yp=2*mtr.r;
			mtr.vy=5;
			mtr.surface=load_image("meteorite-s.png");
		}
		else
		{
			cout<<"hi";
			mtr.xp=rand()%450;
			mtr.r=30;
			mtr.yp=2*mtr.r;
			mtr.vy=3;
			mtr.surface=load_image("meteorite-b.png");
		}
		meteorite.push_back(mtr);
	}
	if(meteorite.size()!=0)
	{
		for(int i=0;i<meteorite.size()-1;i++)
		{
			meteorite[i].yp+=meteorite[i].vy;
			apply_surface(meteorite[i].xp,meteorite[i].yp,meteorite[i].surface,screen);
			if(meteorite[i].yp>=700)
				meteorite.erase(meteorite.begin()+i);
		}
	}
}
void Coin(vector <cn> &coin,SDL_Surface * screen,gft gift5,int xrocket,int yrocket,int roundNum)
{
	int zarib =1;
	if(gift5.time!=0)
		zarib=3;
	if(rand()%(int)(300/zarib)==4)
	{
		int center=rand()%400+25;
		cn con;
		con.surface=load_image("coin.png");
		if(rand()%3==0)
		{
			con.xp=center-15;
			con.yp=-15;
			coin.push_back(con);
			con.xp=center+15;
			con.yp=-15;
			coin.push_back(con);
			con.xp=center+15;
			con.yp=15;
			coin.push_back(con);
			con.xp=center-15;
			con.yp=15;
			coin.push_back(con);
		}
		else
		{
			con.xp=center;
			con.yp=-15;
			coin.push_back(con);
			con.xp=center;
			con.yp=15;
			coin.push_back(con);
			con.xp=center+15;
			con.yp=0;
			coin.push_back(con);
			con.xp=center-15;
			con.yp=0;
			coin.push_back(con);

		}
	}
	if(coin.size()!=0)
	{
		for(int i=0;i<coin.size()-1;i++)
		{
			coin[i].xp+=coin[i].vx;
			coin[i].yp+=coin[i].vy;
			apply_surface(coin[i].xp,coin[i].yp,coin[i].surface,screen);
			if(coin[i].yp==700 || (coin[i].yp==yrocket && coin[i].xp==xrocket))
			{
				coin.erase(coin.begin()+i);
			}
		}
	}
}
bool menu(SDL_Event &event)
{
	if( SDL_PollEvent( &event ) ) //do halat ro barresi mikone k :
   		{ 
        	if( event.type == SDL_KEYDOWN ) //1.jahate safine avaz beshe
        	{ 
            	if( event.key.keysym.sym==SDLK_p) 
    			{
    				int check=false;
    				do
    				{
    					SDL_Delay(500);
    					if(SDL_PollEvent( &event ) && event.type == SDL_KEYDOWN && event.key.keysym.sym==SDLK_s)
    						check=true;
    				}while( check==false);
    			}
    			else if(event.key.keysym.sym==SDLK_q)
    				return false;
    		}
    	}
    	return true;
}
//////////////////////////////
/*
rocket strike:
1.shahab
3.gift
shot:
	1.strike
	2.start point
score:
menu:
	1.sound
graphic:
	1.coin
	2.score
	3.heart
	4.shot
afzayesh deghat va sakhti bazi
*/
