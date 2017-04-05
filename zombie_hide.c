#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX_Z 5

sprite_id zombie [ MAX_Z ];
#define Z_WIDTH 5
#define Z_HEIGHT 5

char z_image[] = "ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ";

sprite_id hero;
#define H_WIDTH 3
#define H_HEIGHT 6

char h_image[] = "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";

bool game_over = false;

void setup_hero() {
		hero = sprite_create( 0, 0, H_WIDTH, H_HEIGHT, h_image );
		sprite_draw(hero);
}

void setup_zombies() {
	for ( int i = 0; i < MAX_Z; i++) {
		zombie[i] = sprite_create( 
			rand() % (screen_width()-Z_WIDTH),
			rand() % (screen_height() - Z_HEIGHT),
			Z_WIDTH,
			Z_HEIGHT,
			z_image 
		);
		sprite_turn_to( zombie[i], 0.015, 0 );
		sprite_turn( zombie[i], rand() % 360 );
		sprite_draw( zombie[i] );
	}
	
}


bool collision ( sprite_id hero, sprite_id zombie ) {
	int lh = round(sprite_x(hero)), rh = lh + H_WIDTH - 1;
	int lz = round(sprite_x(zombie)), rz = lz + Z_WIDTH - 1;
	int th = round(sprite_y(hero)), bh = th + H_HEIGHT - 1;
	int tz = round(sprite_y(zombie)), bz = tz + Z_HEIGHT - 1;
	
	bool collided = true;
	
	     if ( bz < th ) collided = false;
	else if ( bh < tz ) collided = false;
	else if ( rz < lh ) collided = false;
	else if ( rh < lz ) collided = false;
	
	return collided;
}


void process_hero( ) {
		
		int key = get_char();
		
#include <curses.h>

    // Get the current screen coordinates of the hero in integer variables 
    // by rounding the actual coordinates.
    int xh = round(sprite_x(hero));
    int yh = round(sprite_y(hero));

    // Move hero left according to specification.
    if ( ( 'a' == key || KEY_LEFT == key ) && xh > 1 ) {
        sprite_move(hero, -1, 0);
    }

    // Move hero right according to specification.
    else if ( ( 'd' == key || KEY_RIGHT == key ) && xh < screen_width() - sprite_width(hero) - 1 ) {
        sprite_move(hero, +1, 0);
    }

    // Move hero up according to specification.
    else if ( ( 'w' == key || KEY_UP == key ) && yh > 3 ) {
        sprite_move(hero, 0, -1);
    }

    // Move hero down according to specification.
    else if ( ( 's' == key || KEY_DOWN == key ) && yh < screen_height() - sprite_height(hero) - 1 ) {
        sprite_move(hero, 0, +1);
    }

}


void process_zombies() {
	for ( int i = 0; i < MAX_Z; i++ ) {
		if ( zombie[i]->is_visible ){
			sprite_step( zombie[i]);
		
			if ( collision( hero, zombie[i] ) ) {
				sprite_hide( zombie [i] );			
			}
		}
	}
}

void draw_game() {
	clear_screen();
	
	sprite_draw( hero );
	
	for ( int i = 0; i < MAX_Z; i++ ) {
		sprite_draw( zombie[i] );
	}
}

int main() {
	setup_screen();
	setup_hero();
	setup_zombies();
	show_screen();
	
	while ( ! game_over ) {
		process_hero();
		if ( ! game_over ) {
			process_zombies();
		}
		if ( ! game_over ) {
			draw_game();
		}
		
		show_screen();
		timer_pause( 10 );
	}
	
	cleanup_screen();
}