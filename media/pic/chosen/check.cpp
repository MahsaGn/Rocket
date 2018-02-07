#include<iostream>
#include<SDL/SDL_image.h>
#include<SDL/SDL_mixer.h>
#include<SDL/SDL.h>
#include<SDL/SDL_gfxPrimitives.h>
#include<string>
#include<SDL/SDL_ttf.h>
using namespace std;
int main()
{
	SDL_Surface *screen = SDL_SetVideoMode(450,600,32,0);
	SDL_Surface *img = IMG_Load("htc.jpg");
	//SDL_Surface *b = IMG_Load("background.png");
    SDL_SetColorKey( img, SDL_SRCCOLORKEY, SDL_MapRGB( img->format,0, 0, 254 ) );
	SDL_Rect offset;
    offset.x = 0;
    offset.y = 0;
    //SDL_BlitSurface( b, NULL, screen, &offset );
    SDL_BlitSurface( img, NULL, screen, &offset ); 
    SDL_Flip(screen);
	SDL_Delay(3000);
	return 0;
}
