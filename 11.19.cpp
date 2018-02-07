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
	int width=120;
	SDL_Surface * surface;
};
struct gft
{
	int xp;
	int yp=0;
	int r;
	float vy;
	int state=0;
	int time=20;
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
	int vy=5	;
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
void Gift(gft *gift,int n, SDL_Surface *screen,int roundNum,int level);
bool Rocket(rckt &rocket,vector <ammo> &shot,SDL_Event &event,SDL_Surface * screen,int level);//menu in mixed with this
void OppRocket(oppRckt &oppRocket,vector <ammo> &shot, SDL_Surface *screen,int level);
bool Shot(vector <ammo>& shot,SDL_Surface *screen,oppRckt & oppRocket);//if true the opp is killed
void Meteor(vector <meteor> & meteorite,SDL_Surface *screen,int roundNum,int level);
void Coin(vector <cn> &coin,SDL_Surface * screen,gft gift5,int xrocket,int yrocket,int roundNum,int level);

int main()
{
	int bgX = 0, bgy = 0;
	int roundNum=0;
	int score;
	int BGvelocity=4;
	int level=1;
	int coinNum=0;
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
	bool checkLevel=false;
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
		bgy+=BGvelocity;
		if(bgy>=picture->h)
		{
			bgy=0;
		}
	  	apply_surface( bgX, bgy, picture, screen ); 
      	apply_surface( bgX , bgy-picture->h, picture, screen );
    	Gift(&gift[0],5, screen,roundNum,level);
    	meniu=Rocket(rocket,shot,event,screen,level);
    	Meteor(meteorite,screen,roundNum,level);
    	Coin(coin,screen,gift[5],rocket.xp,rocket.yp,roundNum,level);
    	score=(int)(roundNum/20)+coinNum;
    	if((score%100==0 && score!=0) || oppRocket.state==1)
		{
    		OppRocket(oppRocket,shot,screen,level);
		}
    	checkLevel=Shot(shot,screen,oppRocket);
		if(checkLevel==true)// b marhale bad raftim pas soraat ha bishtar mishe
			level++;
		//cout<<score<<endl<<level<<endl<<endl;
		SDL_Flip(screen);
		SDL_Delay(10);
	}while(meniu==true);
	SDL_Delay(1000);
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
void Gift(gft * gift,int n,SDL_Surface * screen ,int roundNum ,int level)//1.come more distanced    2.timi k assar mikonn kam mishe
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

		}
	}	
			
			
	if(rand()%(300+100*level)==4)
	{
		int i;
		do
		{
			i=rand()%n;
		}while(gift[i].state!=0);
		gift[i].time=20 - level;
		gift[i].xp=rand()%(450-2*gift[i].r);
		gift[i].yp=0;
		gift[i].state=2;
		gift[i].vy=rand()%3+3;

	} 
			
}
bool Rocket(rckt &rocket,vector <ammo> &shot,SDL_Event &event,SDL_Surface * screen,int level) //1.velocity inhanced
{ 
    if(rocket.vy!=0)//in tike baraye avale bazie k safine amoodi harekat mikone 
    { 
    	rocket.yp+=rocket.vy;
    	if(rocket.yp<=rocket.ymax) 
       		rocket.vy=0; 
    } 
    else//harekate ofoghi 
    { 
  		Uint8 *keystates = SDL_GetKeyState( NULL );
    	 if( SDL_PollEvent( &event ) )//do halat ro barresi mikone k :
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
        			sht.vy=-8;
        			sht.surface=load_image("shot-up.png"); 
        			shot.push_back(sht); 
       			}
       			else if( event.key.keysym.sym==SDLK_p) 
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
    	rocket.xp+=(rocket.vx+level);
    }  
    /*if(rocket_strike()==true) 
    { 
       rocket.health--;
    }*/ 
    apply_surface( rocket.xp, rocket.yp, rocket.surface, screen ); 
    return true; 
} 
void OppRocket(oppRckt &oppRocket,vector <ammo> &shot, SDL_Surface *screen,int level)//1. healt will increas 2.shots through in shorter time
{
	if(oppRocket.state==0)
	{
		{
			oppRocket.state=1;
			oppRocket.vy=2;
			oppRocket.health=3+level;
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
		if(rand()%(300-20*level)==4)
		{
			ammo sht;
			sht.xp=oppRocket.xp-oppRocket.vx+(oppRocket.width/2)-2;
			sht.yp=oppRocket.yp+oppRocket.height;
			sht.vy=8;
			sht.surface=load_image("shot-down.png");
			shot.push_back(sht);
		}
	}
	apply_surface(oppRocket.xp,oppRocket.yp,oppRocket.surface,screen);
}
bool Shot(vector <ammo>& shot,SDL_Surface *screen,oppRckt &oppRocket) 	
{
   	if(shot.size()!=0)
		for(int i=0;i<shot.size();i++)
		{
			shot[i].yp+=shot[i].vy;
			apply_surface(shot[i].xp,shot[i].yp,shot[i].surface,screen);
			if((shot[i].vy>0 && shot[i].yp==700-40) || (shot[i].vy<0 && shot[i].yp==0))
				shot.erase(shot.begin()+i);
			if(shot[i].vy<0)
			{
				if(oppRocket.state==1 && shot[i].yp>=oppRocket.ymin)
				{
					oppRocket.health--;
					if(oppRocket.health==0)
					{
						oppRocket.state=0;
						return true;

					}
				}
			}
		}
		return false;
}
void Meteor(vector <meteor> & meteorite,SDL_Surface *screen,int roundNum,int level)//1.come down more
{
	if(roundNum%(60-(int)(roundNum/1500))==0 && rand()%2==0)
	{
		meteor mtr;
		mtr.r=40;
		mtr.xp=rand()%(450-2*mtr.r);
		mtr.yp=0;
		mtr.vy=8;
		mtr.surface=load_image("meteorite-s.png");
		meteorite.push_back(mtr);
	}
	if((roundNum%(60-(int)(roundNum/1500))==0 && rand()%2==0) || meteorite.size()==0)
	{
		meteor mtr;
		mtr.r=50;
		mtr.xp=rand()%(450-2*mtr.r);
		mtr.yp=0;
		mtr.vy=5;
		mtr.surface=load_image("meteorite-b.png");
		meteorite.push_back(mtr);
	}
	if(meteorite.size()!=0)
	{
		for(int i=0;i<meteorite.size();i++)
		{
			meteorite[i].yp+=meteorite[i].vy;
			apply_surface(meteorite[i].xp,meteorite[i].yp,meteorite[i].surface,screen);
			if(meteorite[i].yp>=700)
				meteorite.erase(meteorite.begin()+i);
		}
	}
}
void Coin(vector <cn> &coin,SDL_Surface * screen,gft gift5,int xrocket,int yrocket,int roundNum,int level)//Done
{
	int zarib =1;
	if(gift5.time!=0)
		zarib=3;
	if(rand()%(int)(200/zarib)==4 || coin.size()==0)
	{
		int center=rand()%390+30;
		cn con;
		con.surface=load_image("coin.png");
		if(rand()%2==0)
		{
			con.xp=center-20;
			con.yp=-20;
			coin.push_back(con);
			con.xp=center+20;
			con.yp=-20;
			coin.push_back(con);
			con.xp=center+20;
			con.yp=20;
			coin.push_back(con);
			con.xp=center-20;
			con.yp=20;
			coin.push_back(con);
			//SDL_Delay(5000);
		}
		else
		{
			con.xp=center;
			con.yp=-30;
			coin.push_back(con);
			con.xp=center;
			con.yp=30;
			coin.push_back(con);
			con.xp=center+20;
			con.yp=0;
			coin.push_back(con);
			con.xp=center-20;
			con.yp=0;
			coin.push_back(con);

		}
	}
	if(coin.size()!=0)
	{
		for(int i=0;i<coin.size();i++)
		{
			coin[i].xp+=coin[i].vx; 
			coin[i].yp+=coin[i].vy;
			apply_surface(coin[i].xp,coin[i].yp,coin[i].surface,screen);
			if(coin[i].yp==700)
			{
				coin.erase(coin.begin()+i);
			}
		}
	}
}
//////////////////////////////
/*
=>rocket strike:
	1.shahab
	2.shot ba rocket
	3.gift

=>getting old image free

=>graphic:
	1.coin
	2.score
	3.heart
	4.shot
	5.sound
*/
