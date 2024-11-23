#include <hf-risc.h>
#include "vga_drv.h"

/* sprites and sprite drawing */
char monster1a[8][11] = {
	{0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0},
	{2, 0, 0, 2, 0, 0, 0, 2, 0, 0, 2},
	{2, 0, 2, 2, 2, 2, 2, 2, 2, 0, 2},
	{2, 2, 2, 0, 2, 2, 2, 0, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
	{0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0},
	{0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0}
};

char monster1b[8][11] = {
	{0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0},
	{0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0},
	{0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0},
	{0, 2, 2, 0, 2, 2, 2, 0, 2, 2, 0},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 0, 2, 0, 0, 0, 0, 0, 2, 0, 2},
	{0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0}
};

char earthprotector[8][11] = {
	{0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0},
	{0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0},
	{0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
};

char barrier0bl[8][11] = {
	{0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2},
	{0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2},
	{0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}	
};

char barrier0al[8][11] = {
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0},
	{2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0},
	{2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0},
	{2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0}	
};

char barrier0ar[8][11] = {
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2},
	{0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2},
	{0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2},
	{0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2}	
};
char barrier0br[8][11] = {
	{2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0},
	{2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}	
};


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
//              PLAYER MOVEMENT

void player_controls(struct object_s *obj, int speedXDesired, int speedYDesired, int keyboard_input){
    obj->dx = 0;
    obj->dy = 0;

    if((keyboard_input & KEY_LEFT) != 0){
        if((keyboard_input & KEY_RIGHT)){
            obj->dx = 0;
        }else{
            obj->dx = -speedXDesired;
        }
    }

    if((keyboard_input & KEY_RIGHT) != 0){
        if((keyboard_input & KEY_LEFT)){
            obj->dx = 0;
        }else{
            obj->dx = speedXDesired;
        }
    }

    if((keyboard_input & KEY_DOWN) != 0){
        if((keyboard_input & KEY_UP) != 0){
            obj->dy = 0;
        }else{
            obj->dy = speedYDesired;
        }
    }

    if((keyboard_input & KEY_UP) != 0){
        if((keyboard_input & KEY_DOWN) != 0){
            obj->dy = 0;
        }else{
            obj->dy = -speedYDesired;
        }
    }
}

//----------------------------------------------------------------------------------------------------

/* main game loop */
int main(void)
{
	struct object_s enemy1, enemy2, earthprotector_obj;
	struct object_s barrier_l1, barrier_l2, barrier_r1, barrier_r2;

	init_display();
	init_input();

	init_object(&enemy1, monster1a[0], monster1b[0], 0, 11, 8, 30, 35, 1, 1, 3, 3);
	init_object(&earthprotector_obj, earthprotector[0], 0, 0, 11, 8, 150, 206, 1, 1, 3, 3);

	init_object(&barrier_l1, barrier0al[0], 0, 0, 11, 8, 150, 196, 0, 0, 1, 1);
	init_object(&barrier_r1, barrier0ar[0], 0, 0, 11, 8, 161, 196, 0, 0, 1, 1);
	init_object(&barrier_l2, barrier0bl[0], 0, 0, 11, 8, 150, 188, 0, 0, 1, 1);
	init_object(&barrier_r2, barrier0br[0], 0, 0, 11, 8, 161, 188, 0, 0, 1, 1);

	int input_var;
	int speedXPlayer = 0;
	int speedYPlayer = 0;

	while (1) {
		move_object(&enemy1);
		move_object(&enemy2);

		move_object(&barrier_l1);
		move_object(&barrier_r1);
		move_object(&barrier_l2);
		move_object(&barrier_r2);

		input_var = get_input();

        player_controls(&earthprotector_obj, 1, 1, input_var);

		move_object(&earthprotector_obj);
		
		// you can change the direction, speed, etc...
		
		delay_ms(40);
	}

	return 0;
}