#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>
#include <stdio.h>

// CONFIGURATIONS
//#define lives 10
#define DELAY (10000)
#define sc_width 5
#define sc_height 3
#define diamond_width 5
#define diamond_height 5
#define max_diamond 5
#define max_missles 100
#define missle_width 1
#define missle_height 1

// GAME STATE
bool game_over = false;
bool update_screen = true;
int score = 0;
int lives = 3;
bool missle_fired = false;

// GLOBAL FUNCTIONS
void setup();
void help_view();
void purge_buff();
void game_over_prompt();
void draw_border();
void draw_game();
void draw_spacecraft();

// SPRITE DECLARATIONS
sprite_id space_craft;

// Setup game.
void setup(void)
{
    help_view();
    draw_spacecraft();
    show_screen(); 
}

// PURGE KEYBOARD BUFFER
void purge_buff(void)
{
    while (get_char() >= 0)
    {
    }
}

void draw_spacecraft(void)
{

    char *space_craft_image =
             /**/ "  |  "
             /**/ ".[*]."
             /**/ "|_Y_|";

    // SETS SPACE CRAFT SPRITE INITIAL POSITION
    int space_craft_x = screen_width() / 2;
    int space_craft_y = screen_height() - 4;

    space_craft = sprite_create(space_craft_x, space_craft_y, 5, 3, space_craft_image);

    sprite_draw(space_craft);
    show_screen();
}

void update_space_craft(int key)
{
    // SPACE CRAFT Current screen coordinates
    int sc_x = round(sprite_x(space_craft));

    // MOVE SPACE CRAFT
    if ((key == 'a' && sc_x > 1) || (key == 'A' && sc_x > 1))
    {
        purge_buff();
        clear_screen();
        sprite_move(space_craft, -1.0, 0.0);
        return;
    }
    if ((key == 'd' && sc_x < screen_width() - 6) || (key == 'D' && sc_x < screen_width() - 6))
    {
        purge_buff();
        clear_screen();
        sprite_move(space_craft, 1.0, 0.0);
        return;
    }
    // if (key == 'm' || key == 'M')
    // {
    //     purge_buff();
    //     missle_fired = true;
    //     setup_a_missle();
    //     return;
    // }
    show_screen();
}

void help_view(void)
{

    // SPRITE IMAGE
    char *help_image =
             /**/ "########################################"
             /**/ "#         CAB202 Assignment 1          #"
             /**/ "#        The Diamonds of Doom          #"
             /**/ "#          Jonathan Gonzalez           #"
             /**/ "#              n9821112                #"
             /**/ "########################################"
             /**/ "#              Controls                #"
             /**/ "#              q : quit                #"
             /**/ "#              h : help                #"
             /**/ "#      Arrow keys: move left/right     #"
             /**/ "#     Space, z, x, c : shoot bullet    #"
             /**/ "########################################"
             /**/ "#       Press a key to play...         #"
             /**/ "########################################";

    // SETS HELP DETAILS SPRITE POSITION
    int details_x = screen_width() / 2;
    int details_y = screen_height() / 2;

    // SETS SPRITE DIMENSIONS
    int details_width = 40;
    int details_height = 14;

    // DECLARE SPRITE
    sprite_id help_details;

    // INITIALISE HELP SPIRTE AND PLACE IN POSITION (CENTER OF SCREEN)
    help_details = sprite_create(details_x - (details_width / 2), details_y - (details_height / 2), details_width, details_height, help_image);
    sprite_draw(help_details);

    // PAINT SCREEN
    show_screen();

    wait_char();
    clear_screen();
}


void game_over_prompt()
{
    char *game_over_msg =
            /**/ "########################################"
             /**/ "                                        "
             /**/ "              - GAME OVER -             "
             /**/ "      Would you like to play again?     "
             /**/ "                                        "
             /**/ "                  Y / N                 "
             /**/ "                                        "
             /**/ "########################################";
    // SETS GAME OVER DETAILS SPRITE POSITION
    int details_x = screen_width() / 2;
    int details_y = screen_height() / 2;

    // SETS SPRITE DIMENSIONS
    int details_width = 40;
    int details_height = 8;

    // DECLARE SPRITE
    sprite_id game_over_sprite;

    // INITIALISE GAME_OVER SPIRTE AND PLACE IN POSITION (CENTER OF SCREEN)
    game_over_sprite = sprite_create(details_x - (details_width / 2), details_y - (details_height / 2), details_width, details_height, game_over_msg);

    sprite_draw(game_over_sprite);

    show_screen();

    int key = getchar();
    if (key == 'y')
    {
        purge_buff();
        clear_screen();
        lives = 3;
        score = 0;
        setup();
        return;
    }
    if (key == 'n')
    {
        purge_buff();
        clear_screen();
        game_over = true;
        return;
    }
}

void draw_border()
{
    //Set screen width and height variables
    int w = screen_width(), h = screen_height();

    //Set left, right, top and bottom edges
    int left = 0, right = w - 1, top = 0, bottom = h - 1;

    //Set ch variable
    char ch = 'T';

    // Draw play field border
    draw_line(left, top, right, top, '-');
    draw_line(left, bottom, right, bottom, ch);
    draw_line(left, top + 1, left, bottom - 1, '|');
    draw_line(right, top + 1, right, bottom - 1, '|');

    //Draw play field status serparator
    draw_line(left + 1, top + 2, right - 1, top + 2, '_');

    //Status bar text
    draw_string(left + 2, top + 1, "# Lives = ");

    //Draws number of lives
    draw_int(left + 13, top + 1, lives);

    //Draws Score heading
    draw_string((screen_width() * .5) - 5, top + 1, "# Score = ");

    //Draws actual score
    draw_int((screen_width() * .5 + 5), top + 1, score);

    //Draws TIMER heading
    draw_string(right - screen_width() * .25, top + 1, "# Time = ");

    //Draws elapsed TIME
    //draw_int(right - screen_width() * .25 + 10, top + 1, game_time);
    

    show_screen();
    return;
}

void draw_game(){
    clear_screen();
    sprite_draw(space_craft);
}

void process(){

    draw_border();

    int key = get_char();
    update_space_craft(key);

    if (key == 'q' || lives == 0)
    {
        purge_buff();
        clear_screen();
        game_over_prompt();
        return;
    }

    if (key == 'h')
    {
        purge_buff();
        clear_screen();
        help_view();
        return;
    }
    
    show_screen();
}

int main(){
    setup_screen();
    setup();

    while(!game_over){
       
        process();
        show_screen();
    }

    cleanup_screen();
}