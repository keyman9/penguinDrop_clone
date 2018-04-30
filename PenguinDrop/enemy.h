/*
Author: Spencer Scott
*/

#ifndef enemyh
#define enemyh
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>

class enemy
{
public:
	enemy();
	~enemy();
	void Drawenemy();
	void Startenemy(int startX, int startY);
	void Updateenemy();
	void Collideenemy(int &deckY,int &health);
	int getBoundX(){ return boundx; }
	int getBoundY(){ return boundy; }
	int getX(){ return x; }
	int getY(){ return y; }
	bool getLive()	{ return live; }
	void setLive(bool l){ live = l; }
	void setShot(bool s){ shot = s; }
private:
	int x;
	int y;
	bool live;
	bool shot;
	int speed;
	int boundx;
	int boundy;
	ALLEGRO_BITMAP *image;
	ALLEGRO_BITMAP *fire;
	ALLEGRO_SAMPLE *exploded;
};

#endif