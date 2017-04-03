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
#define missle_width 1
#define missle_height 1

// GAME STATE
bool game_over = false;
bool update_screen = true;
int score = 0;
bool missle_fired = false;
int lives = 3;

// GLOBAL VARIABLES
void process(void);
void help_view(void);
void collision_prompt(void);
void do_game_over(void);
void draw_border(void);
void draw_game(void);
void game_over_prompt(void);
void draw_spacecraft(void);
void setup_diamond(void);
sprite_id setup_a_diamond(void);
void setup_missle(void);
sprite_id collision_detect(sprite_id sprite, sprite_id sprites[], int num_sprites);
bool collided(sprite_id sprite1, sprite_id sprite2);

sprite_id space_craft;
sprite_id diamond[max_diamond];
sprite_id missle;

// Setup game.
void setup(void)
{
    help_view();
    draw_spacecraft();
    setup_diamond();
    setup_missle();
}

// COLLISION DETECTION METHOD
sprite_id collision_detect(sprite_id sprite, sprite_id sprites[],int num_sprites){

    for(int i = 0; i < num_sprites; i++){
        sprite_id current_sprite = sprites[i];

        if(sprite != current_sprite && collided(sprite, current_sprite)){
            return current_sprite;
        }
    }
    return NULL;
}

// BOOLEAN COLLISION OF SPRITES
bool collided(sprite_id sprite1, sprite_id sprite2) {
    int sprite1_top = round(sprite_y(sprite1)),
        sprite1_bottom = sprite1_top + sprite_height(sprite1) - 1,
        sprite1_left = round(sprite_x(sprite1)),
        sprite1_right = sprite1_left + sprite_width(sprite1) - 1;

    int sprite2_top = round(sprite_y(sprite2)),
        sprite2_bottom = sprite2_top + sprite_height(sprite2) - 1,
        sprite2_left = round(sprite_x(sprite2)),
        sprite2_right = sprite2_left + sprite_width(sprite2) - 1;

    return !(
        sprite1_bottom < sprite2_top
        || sprite1_top > sprite2_bottom
        || sprite1_right < sprite2_left
        || sprite1_left > sprite2_right
        );
}

// PURGE KEYBOARD BUFFER
void purge_buff(void)
{
    while (get_char() >= 0)
    {
    }
}

void collision_prompt(void){
    char *collision_prompt_image = 
             /**/ "########################################"
             /**/ "                                        "
             /**/ "             YOUR SPACE CRAFT           "
             /**/ "     HAS BEEN DESTROYED BY A DIAMOND    "
             /**/ "                                        "
             /**/ "        PRESS ANY KEY TO CONTINUE       "
             /**/ "                                        "
             /**/ "########################################";
    
    // SETS COLLISION PROMPT LOCATION
    int cp_x = screen_width() / 2;
    int cp_y = screen_height() / 2;

    // SETS SPRITE DIMENSIONS
    int cp_width = 40;
    int cp_height = 8;

    // DECLARE SPRITE
    sprite_id collision_prompt;

    // INITIALISE GAME_OVER SPIRTE AND PLACE IN POSITION (CENTER OF SCREEN)
    collision_prompt = sprite_create(cp_x - (cp_width / 2), cp_y - (cp_height / 2), cp_width, cp_height, collision_prompt_image);

    sprite_draw(collision_prompt);
    show_screen();

    wait_char();
    setup();
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

void setup_diamond(void)
{

    for (int i = 0; i < max_diamond; i++)
    {
        diamond[i] = setup_a_diamond();
    }
}

sprite_id setup_a_diamond(void)
{
    char *diamond_image =
             /**/ "  A  "
             /**/ " <|> "
             /**/ "<|*|>"
             /**/ " <|> "
             /**/ "  V  ";

    int w = screen_width();
    int h = screen_height();
    
    int xRange = (w - diamond_width) - 2;
    int diamond_y = h * .2;
    sprite_id diamond = sprite_create(rand() % xRange, diamond_y, diamond_width, diamond_height, diamond_image);
    sprite_turn_to(diamond, 0.05, 0);
    sprite_turn(diamond, rand() % 360);

    return diamond;
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
             /**/ "#      q         : quit                #"
             /**/ "#      h         : help                #"
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
        return;
    }
    if (key == 'n')
    {
        purge_buff();
        clear_screen();
        game_over = true;
        return;
    }
    if (key != 'y' && key != 'n')
    {
        purge_buff();
        clear_screen();
        return;
    }

    wait_char();
}

void draw_border()
{
    // TIMER ??? NOT WORKING PROPERLY :(
    timer_id gameTime;
    gameTime = create_timer(120000);
    int timer = get_current_time();

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
    draw_int(right - screen_width() * .25 + 10, top + 1, timer);

    show_screen();
    return;
}

void draw_game()
{

    int w = screen_width();
    int h = screen_height();

    sprite_draw(space_craft);

    // Draw multiple diamonds
    for (int i = 0; i < max_diamond; i++)
    {
        sprite_draw(diamond[i]);

        // MOVING THE DIAMONDS AND BOUNCING
        int diamond_x = round(sprite_x(diamond[i]));
        int diamond_y = round(sprite_y(diamond[i]));

        double ddx = sprite_dx(diamond[i]);
        double ddy = sprite_dy(diamond[i]);

        if (diamond_x <= 1)
        {
            ddx = fabs(ddx);
        }
        else if (diamond_x >= w - diamond_width)
        {
            ddx = -fabs(ddx);
        }
        if (diamond_y <= 2)
        {
            ddy = fabs(ddy);
        }
        else if (diamond_y >= h - diamond_height)
        {
            ddy = -fabs(ddy);
        }
        //BOUNCE BACK
        if (ddx != sprite_dx(diamond[i]) || ddy != sprite_dy(diamond[i]))
        {
            sprite_back(diamond[i]);
            sprite_turn_to(diamond[i], ddx, ddy);
        }
        sprite_step(diamond[i]);
    }
    //int missle_x = round(sprite_x(missle));
    int missle_y = round(sprite_y(missle));

    double mdx = sprite_dx(missle);
    double mdy = sprite_dy(missle);

    if (missle_fired == true)
    {
        sprite_draw(missle);
        if (missle_y > 0)
        {
            sprite_move(missle, mdx + 0, mdy - .3);
            sprite_step(missle);
        }
    }
    if (missle_y == 0)
    {
        setup_missle();
        missle_fired = false;
        return;
    }

    draw_border();
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
    if (key == 'm' || key == 'M')
    {
        purge_buff();
        missle_fired = true;
        return;
    }
}

void setup_missle(void)
{
    char *missle_image =
        /**/ "*";

    double missle_x = sprite_x(space_craft);
    double missle_y = sprite_y(space_craft);
    missle = sprite_create(missle_x + 2, missle_y - 1, 1, 1, missle_image);
}

void process()
{
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

    if (collision_detect(space_craft, diamond, max_diamond)){
        clear_screen();
        collision_prompt();
        lives -= 1;
        return;
    }

    show_screen();
}

// ENTER PROGRAM
int main(void)
{
    setup_screen();
    setup();

    while (game_over == false)
    {
        process();
        if (!game_over)
        {
            clear_screen();
            draw_game();
            show_screen();
        }
        timer_pause(10);
    }

    cleanup_screen();

    return 0;
}