#ifndef turretcpp
#define turretcpp
#include<allegro5\allegro.h>
#include<allegro5\allegro_image.h>
#include"turret.h"
turret::turret(int boardX, int boardY){
	x = boardX;
	y = boardY;
	angle = 0;
	image = al_load_bitmap("magisterFit.png");
	alive = true;

	boundx = al_get_bitmap_width(image)*.5;
	boundy = al_get_bitmap_height(image)*.5;
}

turret::~turret(){
	al_destroy_bitmap(image);
}

void turret::drawTurret(){
	al_draw_rotated_bitmap(image, x+boundx,y+boundy,x,y,angle,0);
}

void turret::rotateTurret(float adjust){
	angle += adjust;
	if (angle < 0){
		angle = 256;
	}
	if (angle < 171 && angle > 85){
		angle = 171;
	}
}
#endif