/*
	Author: Spencer Scott
*/

#ifndef snowballcpp
#define M_PI				3.14159265358979323846 //pi
#define snowballcpp 
#include <iostream>
#include <math.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>
#include<allegro5\allegro_audio.h>
#include<allegro5\allegro_acodec.h>
#include "snowball.h"
#include "enemy.h"
    


//constructor
snowballs::snowballs(){
	x, y = 0;
	live = false;
	speed = 3;
	radian_angle = 0.01;
	image = al_load_bitmap("laser.png");
	if (!image){
		exit(1);
	}

	boundx = al_get_bitmap_width(image) / 2;
	boundy = al_get_bitmap_height(image) / 2;
	pew = al_load_sample("pew.wav");
	boom = al_load_sample("boom.wav");
}

//destructor
snowballs::~snowballs(){
	al_destroy_bitmap(image);
	al_destroy_sample(pew);
	al_destroy_sample(boom);
}

//draws all live snowballs
void snowballs::DrawSnowballs(){
	if (live){
		al_draw_bitmap(image, x, y, 0);
	}
}

//fires snowballs from gun, given its trajectory and pivot points
void snowballs::FireSnowballs(ALLEGRO_BITMAP *gun, double gunAngle, int startX, int startY){
	if (!live){
		al_play_sample(pew, 1.0, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);		//play laser sound
		x = startX;		//originate at base 
		y = startY;	
		gunAngle += (M_PI / 2);		//convert degrees to radians
		radian_angle = gunAngle;
			//(gunAngle / 0.711)*((2 * M_PI) / 360.0);
		
		//start snowballs at end of turret
		x -= (speed*16)*cos(radian_angle);				
		y -= (speed*16)*sin(radian_angle);
		//std::cout << "Fired from (" << x << "," << y << ")!" << std::endl;
		//std::cout << "At " << radian_angle << " radians" << std::endl;
		live = true;
	}
}


//Updates x and y coordinates of snowball
//param WIDTH is width of display
//param HEIGHT is height of display
void snowballs::UpdateSnowball(int WIDTH, int HEIGHT){
	if (live){
		x -= speed*cos(radian_angle);		//continue on x trajectory
		y -= speed*sin(radian_angle);		//continue on y trajectory
		if (x <= 0 || x > WIDTH || y <= 0 || y > HEIGHT){
			live = false;	//if the snowball went off the screen, kill it
		}
	}
}

//Checks for snowball collisions with enemies.
//If there is a collision, kills enemy and toggles the "shot" which adds explosion layer above the enemy
void snowballs::CollideSnowball(enemy enemies[], int cSize){
	if (live){
		for (int i = 0; i < cSize; i++){
			if (enemies[i].getLive()){
				if(x >= (enemies[i].getX() - enemies[i].getBoundX()) &&
				   x <= (enemies[i].getX() + enemies[i].getBoundX()) &&
				   y >= (enemies[i].getY() - enemies[i].getBoundY()) &&
				   y <= (enemies[i].getY() + enemies[i].getBoundY()))
				{
					al_play_sample(boom, 1.0, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
					live = false;				//kill snowball
					enemies[i].setLive(false);	//kill enemy
					enemies[i].setShot(true);	//add explosion
				}
			}
		}
	}
}

#endif