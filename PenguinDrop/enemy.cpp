/*
	Author:Spencer Scott
*/
#ifndef enemycpp
#define enemycpp
#include<allegro5\allegro.h>
#include<allegro5\allegro_image.h>
#include<allegro5\allegro_audio.h>
#include<allegro5\allegro_acodec.h>
#include<time.h>
#include "enemy.h"
#include "snowball.h"

//constructor
enemy::enemy(){
	live = false;
	shot = false;
	speed = 1;
	x = 0;
	y = 0;
	image = al_load_bitmap("enemy.png");
	if (!image){
		exit(1);
	}
	fire = al_load_bitmap("explosion.png");

	boundx = al_get_bitmap_width(image) / 2;
	boundy = al_get_bitmap_height(image) / 2;
	exploded = al_load_sample("lost_spaceship.wav");
}

//destructor
enemy::~enemy(){
	al_destroy_bitmap(image);
	al_destroy_bitmap(fire);
	al_destroy_sample(exploded);
}

//draws live enemy movement and static explosions for shot enemies
void enemy::Drawenemy(){
	if (live){
		al_draw_bitmap(image, x, y, 0);	//use the normal spaceship		
	}
	if(shot){
		al_draw_bitmap(fire, x, y, 0);  //use the explosion
	}
}

//Spawn enemies at the given position on the display
//param startX is the x coordinate, which varies
//param startY is the y coordinate, which remains above the display
void enemy::Startenemy(int startX, int startY){
	if (!live){
		if (rand() % 600 == 0){
			x = startX;
			y = startY;
			live = true;
			shot = false;
		}
	}
}

//Updates location of enemy, which moves down the screen
void enemy::Updateenemy(){
	if (live)
		y += speed;
}

//Collision test against the 'iceberg' via its 
//param deckY is the upper bound of the 'iceberg'
//param health is used for effecting gameOver state
void enemy::Collideenemy(int &deckY,int &health){
	if (live){
		if (y >= deckY){
			deckY += 8;
			health--;
			live = false;
			shot = false;
			al_play_sample(exploded, 1.0, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		}
	}
}

#endif