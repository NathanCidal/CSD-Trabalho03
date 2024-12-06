#include <hf-risc.h>
#include "vga_drv.h"
#include "space_invaders.h"	
// Sprites are inside "space_invaders.h"

//----------------------------------------------------------------------------------------------------
//					GLOBAL VARIABLES
//----------------------------------------------------------------------------------------------------
int  playerScore  =  690;
int  highestScore =    0;

#define ENEMIES_LIN     5 
#define ENEMIES_COL    11 

#define ENEMIES_SPEED  10 

int killed[ENEMIES_LIN][ENEMIES_COL] = {0};

//----------------------------------------------------------------------------------------------------
// 											BASE CODE
//----------------------------------------------------------------------------------------------------

void draw_sprite(unsigned int x, unsigned int y, char *sprite,
	unsigned int sizex, unsigned int sizey, int color)
{
	unsigned int px, py;
	
	if (color < 0) {
		for (py = 0; py < sizey; py++)
			for (px = 0; px < sizex; px++)
				display_pixel(x + px, y + py, sprite[py * sizex + px]);
	} else {
		for (py = 0; py < sizey; py++)
			for (px = 0; px < sizex; px++)
				display_pixel(x + px, y + py, color & 0xf);
	}
}

/* sprite based objects */
struct object_s {
	char *sprite_frame[3];
	char spriteszx, spriteszy, sprites;
	int cursprite;
	unsigned int posx, posy;
	int dx, dy;
	int speedx, speedy;
	int speedxcnt, speedycnt;
};

void init_object(struct object_s *obj, char *spritea, char *spriteb,
	char *spritec, char spriteszx, char spriteszy, int posx, int posy, 
	int dx, int dy, int spx, int spy)
{
	obj->sprite_frame[0] = spritea;
	obj->sprite_frame[1] = spriteb;
	obj->sprite_frame[2] = spritec;
	obj->spriteszx = spriteszx;
	obj->spriteszy = spriteszy;
	obj->cursprite = 0;
	obj->posx = posx;
	obj->posy = posy;
	obj->dx = dx;
	obj->dy = dy;
	obj->speedx = spx;
	obj->speedy = spy;
	obj->speedxcnt = spx;
	obj->speedycnt = spy;
}

// draw object on Screen
void draw_object(struct object_s *obj, char chgsprite, int color)
{
	if (chgsprite) {
		obj->cursprite++;
		if (obj->sprite_frame[obj->cursprite] == 0)
			obj->cursprite = 0;
	}
	
	draw_sprite(obj->posx, obj->posy, obj->sprite_frame[obj->cursprite],
		obj->spriteszx, obj->spriteszy, color);
}

// move object on Screen
void move_object(struct object_s *obj)
{
	struct object_s oldobj;
	
	memcpy(&oldobj, obj, sizeof(struct object_s));
	
	if (--obj->speedxcnt == 0) {
		obj->speedxcnt = obj->speedx;
		obj->posx = obj->posx + obj->dx;
	}
	if (--obj->speedycnt == 0) {
		obj->speedycnt = obj->speedy;
		obj->posy = obj->posy + obj->dy;
	}

	if ((obj->speedx == obj->speedxcnt) || (obj->speedy == obj->speedycnt)) {
		draw_object(&oldobj, 0, 0);
		draw_object(obj, 1, -1);
	}
}


/* display and input */
void init_display()
{
	display_background(BLACK);
}

enum {
	KEY_CENTER	= 0x01,		//MASK8
	KEY_UP		= 0x02,		//MASK9
	KEY_LEFT	= 0x04,		//MASK10
	KEY_RIGHT	= 0x08,		//MASK11
	KEY_DOWN	= 0x10		//MASK12
};

void init_input()
{
	/* configure GPIOB pins 8 .. 12 as inputs */
	GPIOB->DDR &= ~(MASK_P8 | MASK_P9 | MASK_P10 | MASK_P11 | MASK_P12);
}

int get_input()
{
	int keys = 0;
	if (GPIOB->IN & MASK_P8)	keys |= KEY_CENTER;
	if (GPIOB->IN & MASK_P9)	keys |= KEY_UP;
	if (GPIOB->IN & MASK_P10)	keys |= KEY_LEFT;
	if (GPIOB->IN & MASK_P11)	keys |= KEY_RIGHT;
	if (GPIOB->IN & MASK_P12)	keys |= KEY_DOWN;
		
	return keys;
}

//----------------------------------------------------------------------------------------------------
//              PLAYER SCORE
//----------------------------------------------------------------------------------------------------

void player_score_converter(int score_value, char * score_string){
	int value01 = score_value % 10;
	int value02 = (score_value / 10) % 10;
	int value03 = (score_value / 100) % 10;
	int value04 = score_value / 1000; 

	score_string[3] = '0' + value01;
	score_string[2] = '0' + value02;
	score_string[1] = '0' + value03;
	score_string[0] = '0' + value04;
}

//----------------------------------------------------------------------------------------------------
//              PLAYER MOVEMENT
//----------------------------------------------------------------------------------------------------

int check_screen_limits(struct object_s *obj, int speedX, int speedY){
	if(obj->posx + speedX <= 0){
		return 0;
	}else if(obj->posx + speedX >= 288){
		return 0;
	}else if(obj->posy + speedY <= 0){
		return 0;
	}else if(obj->posy + speedY >= 209){
		return 0;
	}
	return 1;
}

void player_controls(struct object_s *obj, int speedXDesired, int speedYDesired, int keyboard_input){
    obj->dx = 0;
    obj->dy = 0;

    if((keyboard_input & KEY_LEFT) != 0){
        if((keyboard_input & KEY_RIGHT)){
            obj->dx = 0;
        }else{
			if(check_screen_limits(obj, -speedXDesired, 0)){
            	obj->dx = -speedXDesired;
			}
        }
    }

    if((keyboard_input & KEY_RIGHT) != 0){
        if((keyboard_input & KEY_LEFT)){
            obj->dx = 0;
        }else{
			if(check_screen_limits(obj, speedXDesired, 0)){
            	obj->dx = speedXDesired;
			}
        }
    }

    if((keyboard_input & KEY_DOWN) != 0){
        if((keyboard_input & KEY_UP) != 0){
            obj->dy = 0;
        }else{
			if(check_screen_limits(obj, 0, speedYDesired)){
            	obj->dy = speedYDesired;
			}
        }
    }

    if((keyboard_input & KEY_UP) != 0){
        if((keyboard_input & KEY_DOWN) != 0){
            obj->dy = 0;
        }else{
			if(check_screen_limits(obj, 0, -speedYDesired)){
                obj->dy = -speedYDesired;
			}
        }
    }
}

//----------------------------------------------------------------------------------------------------
//    			BASIC ENEMY MOVEMENT
//----------------------------------------------------------------------------------------------------

void move_enemies(struct object_s obj[ENEMIES_LIN][ENEMIES_COL], int* move_down){

    if(*move_down == 1) {
        for(int i=0; i<ENEMIES_LIN; i++) {
            for(int j=0; j<ENEMIES_COL; j++) {
                 obj[i][j].dy = 13;
                 obj[i][j].speedycnt = 1;
            }    
        }

        *move_down = 0;
    }
    else {
        if((obj[0][ENEMIES_COL-1].posx + obj[0][ENEMIES_COL-1].dx >= 288) || (obj[0][0].posx + obj[0][0].dx <= 3)){
            for(int i=0; i<ENEMIES_LIN; i++) {
                for(int j=0; j<ENEMIES_COL; j++) {
                    obj[i][j].dx = -obj[i][j].dx;
                    // move_down 
                    //move_object(&obj[i][j]);		
                }    
            }

            *move_down = 1;
        } else {
            for(int i=0; i<ENEMIES_LIN; i++) {
                for(int j=0; j<ENEMIES_COL; j++) {
                    obj[i][j].speedycnt = 10;
                    obj[i][j].dy = 0;
                }
            }
        }
    }
}

void draw_enemies(struct object_s enemies[ENEMIES_LIN][ENEMIES_COL]) {
	for(int i=0; i<ENEMIES_LIN; i++) {
		for(int j=0; j<ENEMIES_COL; j++)
			move_object(&enemies[i][j]);
	}
}
//----------------------------------------------------------------------------------------------------
//				SCOREBOARD
//----------------------------------------------------------------------------------------------------

//	int pts;
//  char buff[20];
//  sprintf(buff, "%5d", pts);

void draw_scoreboard() {
	display_print("SCORE<1>",  30,  5, 1, WHITE);
	display_print("HI-SCORE", 120,  5, 1, WHITE);
	display_print("SCORE<2>", 210,  5, 1, WHITE);
}

//----------------------------------------------------------------------------------------------------

/* main game loop */
int main(void)
{
    // enemies initialization 
    struct object_s enemies[ENEMIES_LIN][ENEMIES_COL];
	for(int i=0; i<ENEMIES_LIN; i++) {
		for(int j=0; j<ENEMIES_COL; j++) 
			init_object(&enemies[i][j], alien01_moving_a[0], alien01_moving_b[0], 
						0, 11, 8,  18 + 15*(j+1),  18 + 12*(i+1), 10,  0, ENEMIES_SPEED, 10);
	}

	// player initialization 
	struct object_s spaceship_obj;	
	init_object(&spaceship_obj, spaceship[0],   0, 	0, 11, 8, 150, 186, 1,  0,  2,  2);

	// projectile initialization 
	struct object_s player_shot_obj;
	init_object(&player_shot_obj, player_shot[0],   0, 	0, 11, 8, 150, 186, 1,  0,  2,  2);

	//struct object_s barrier_l1, barrier_l2, barrier_r1, barrier_r2;

	init_display();
	init_input();

	//init_object(&barrier_l1, barrier0al[0], 0, 0, 11, 8, 150, 196, 0, 0, 1, 1);
	//init_object(&barrier_r1, barrier0ar[0], 0, 0, 11, 8, 161, 196, 0, 0, 1, 1);
	//init_object(&barrier_l2, barrier0bl[0], 0, 0, 11, 8, 150, 188, 0, 0, 1, 1);
	//init_object(&barrier_r2, barrier0br[0], 0, 0, 11, 8, 161, 188, 0, 0, 1, 1);

	int input_var;
	int speedPlayer = 3;
	int speedEnemy  = 6;

	int  playerLives  =    3;
	char playerScore_String[5]  = "0000\0";
	char highestScore_String[5] = "0000\0";

	draw_scoreboard();

	//display_frectangle(155, 155, 5, 150, BLUE);

	display_print(playerScore_String,   45, 20, 1, WHITE);
	display_print(highestScore_String, 135, 20, 1 , WHITE);
	player_score_converter(playerScore, playerScore_String);
	player_score_converter(highestScore, highestScore_String);

	//void display_hline(uint16_t x0, uint16_t y0, uint16_t length, uint16_t color);
	display_hline(0, 196, 300, 7);

    int move_down = 0;

	while (1){
        move_enemies(enemies, &move_down);
		draw_enemies(enemies);
        
		input_var = get_input();
        player_controls(&spaceship_obj, speedPlayer, 0, input_var);
		move_object(&spaceship_obj);
		
		// you can change the direction, speed, etc...
		delay_ms(40);
	}

	return 0;
}