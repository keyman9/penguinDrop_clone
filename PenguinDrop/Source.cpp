/*
Author: Spencer Scott
*/

#include <iostream>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include<allegro5\allegro_audio.h>
#include<allegro5\allegro_acodec.h>
#include "enemy.h"
#include "snowball.h"


int main(int argc, char **argv){
	
	//screen variables
	const int WIDTH = 800;
	const int HEIGHT = 400;

	//game variables
	bool done = false;
	bool redraw = false;
	const int FPS = 60;
	enum KEYS{UP, DOWN, LEFT, RIGHT, SPACE};
	bool keys[5] = { false, false, false, false, false };
	const int NUM_SNOWBALLS = 360;
	const int NUM_ENEMIES = 20;
	srand(time(NULL));


	//allegro pointers
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *background = NULL;
	ALLEGRO_BITMAP *deck = NULL;
	ALLEGRO_BITMAP *weapon = NULL;
	ALLEGRO_SAMPLE *endgame = NULL;

	if (!al_init()){
		std::cerr << "Allegro failed to initialize." << std::endl;
		return -1;
	}

	display = al_create_display(WIDTH, HEIGHT);

	if (!display){
		std::cerr << "Allegro display failed." << std::endl;
	}

	//allegro inits
	al_init_primitives_addon();
	al_install_keyboard();
	al_init_image_addon();
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(4);

	//allegro pointer inits
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);
	
	deck = al_load_bitmap("base.png");
	if (!deck){
		return -1;
	}
	weapon = al_load_bitmap("magisterFit.png");
	if (!weapon){
		return -1;
	}
	background = al_load_bitmap("space.png");
	if (!background){
		return -1;
	}
	endgame = al_load_sample("endgame.wav");
	if (!endgame){
		return -1;
	}

	//weapon variables
	double angle = 0.0;
	int weapCX = (WIDTH / 2);
	int weapCY = HEIGHT*.8;
	int weapXPivot = al_get_bitmap_width(weapon) / 2;
	int weapYPivot = al_get_bitmap_width(weapon)*.75;

	//deck variables
	int deckX = weapCX - (al_get_bitmap_width(deck) / 2);
	int deckY = weapCY - (al_get_bitmap_height(weapon) / 2);

	//objects
	snowballs mySnowballs[NUM_SNOWBALLS];
	enemy myEnemies[NUM_ENEMIES];
	int health = 5;				//number of "lives" until turret falls
	int snowBallToFire = 0;		//upcoming snowball index for firing consecutive snowballs
	bool shoot = false;			//used to apply shot per frame, if necessary

	//register everything 
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	
	//use display
	al_set_target_bitmap(al_get_backbuffer(display));
	
	//start clock
	al_start_timer(timer);
	
	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;
			//rotate left
			if (keys[LEFT]){
				if (angle-0.06 > -1.2)	//wide arc to the left
					angle -= 0.06;
			}
			//rotate right
			if (keys[RIGHT]){
				if (angle+.06 < 1.27)	//wide arc to the right
					angle += .06;
			}
			//Update Snowballs
			for (int i = 0; i < NUM_SNOWBALLS; i++){
				mySnowballs[i].UpdateSnowball(WIDTH, HEIGHT);
			}
			//Start	Enemies	Dropping
			for (int i = 0; i < NUM_ENEMIES; i++){
				int x = rand() % (WIDTH/NUM_ENEMIES)*(i + 1);	
				int y = 0 - myEnemies[i].getBoundY();
				myEnemies[i].Startenemy(x, y);
			}
			//Update	Penguins	Dropping
			for (int i = 0; i < NUM_ENEMIES; i++){
				myEnemies[i].Updateenemy();
			}
			//Collide	Snowballs
			for (int i = 0; i < NUM_SNOWBALLS; i++){
				mySnowballs[i].CollideSnowball(myEnemies, NUM_ENEMIES);
			}
			//Collide	Penguins
			for (int i = 0; i < NUM_ENEMIES; i++){
				myEnemies[i].Collideenemy(deckY, health);
			}
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				shoot = true;
				//mySnowballs[snowBallToFire].FireSnowballs(weapon, angle, WIDTH / 2, weapCY);
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				shoot = false;
				break;
			}
		}
		if (redraw	&&	al_is_event_queue_empty(event_queue))
		{
			if (shoot){		//if the space button is down
				//fire snowballs from the end of the turret
				mySnowballs[snowBallToFire].FireSnowballs(weapon, angle,(deckX+al_get_bitmap_width(deck)/2)-8,weapCY-16);
			}
			snowBallToFire = (snowBallToFire % NUM_SNOWBALLS) + 1; //move to next snowball
			redraw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));
			//draw	background
			al_draw_bitmap(background, 0, 0, 0);
			//draw firing deck
			al_draw_bitmap(deck, deckX, deckY, 0);
			
			//draw weapon
			//std::cout << "Drew at angle: " << angle << std::endl;
			al_draw_rotated_bitmap(weapon,weapXPivot, weapYPivot, weapCX, weapCY, angle, 0);
			//draw	snowballs
			for (int i = 0; i < NUM_SNOWBALLS; i++){
				mySnowballs[i].DrawSnowballs();
			}
			//draw	penguinDropping
			for (int i = 0; i < NUM_ENEMIES; i++){
				myEnemies[i].Drawenemy();
			}
			al_flip_display();
			
		}
		if (health == 0){		//if health is depleted, end game
			al_play_sample(endgame, 1.0, 0, 1.0, ALLEGRO_PLAYMODE_LOOP, 0);
			done = true;
			al_rest(5.0);
		}
			
	}

	al_destroy_display(display);
	al_destroy_bitmap(background);
	al_destroy_bitmap(weapon);
	al_destroy_bitmap(deck);
	al_destroy_sample(endgame);

	return 0;
}