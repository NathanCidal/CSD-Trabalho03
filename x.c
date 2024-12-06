#include <hf-risc.h>

#include "vga_drv.h"
#include "space_invaders.h"	

typedef struct {
	char *sprite_frame[3];
	char spriteszx, spriteszy, sprites;
	int cursprite;
	unsigned int posx, posy;
	int dx, dy;
	int speedx, speedy;
	int speedxcnt, speedycnt;
    int alive;
    int visible;
} object_s;

typedef struct {
    int xo;
    int yo;
    int w;
    int h;
    int hp;
} barrier_t;

//----------------------------------------------------------------------------------------------------
//                              GLOBAL VARIABLES
//----------------------------------------------------------------------------------------------------
#define WIDTH         300
#define HEIGHT        218

#define ENEMIES_LIN     5 
#define ENEMIES_COL    11 
#define NUM_BARRIERS    3

object_s player_proj;
object_s enemies[ENEMIES_LIN][ENEMIES_COL];
object_s enemies_proj[ENEMIES_LIN][ENEMIES_COL];
object_s earthprotector_obj;
object_s enemy_explosion_obj;

object_s player_death_obj;

object_s hearts[3];

barrier_t b1 = {WIDTH*0.10, HEIGHT*0.7, 50, 15, 4};
barrier_t b2 = {WIDTH*0.45, HEIGHT*0.7, 50, 15, 4};
barrier_t b3 = {WIDTH*0.80, HEIGHT*0.7, 50, 15, 4};

int score = 0;
const int player_speed = 5;
const int max_enemy_shots = 3;
int enemy_shots = 0;
int lives = 3;
char old_score_str[10];
char score_str[10];

const int player_xo = 150;
const int player_yo = 200;

//----------------------------------------------------------------------------------------------------
//                      BASE CODE
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

void init_object(object_s *obj, char *spritea, char *spriteb, char *spritec, 
                 char spriteszx, char spriteszy, int posx, int posy, 
	             int dx, int dy, int spx, int spy, int alive, int visible)
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
    obj->alive = alive;
    obj->visible = visible;
}

void draw_object(object_s *obj, char chgsprite, int color)
{
	if (chgsprite) {
		obj->cursprite++;
		if (obj->sprite_frame[obj->cursprite] == 0)
			obj->cursprite = 0;
	}
	
	draw_sprite(obj->posx, obj->posy, obj->sprite_frame[obj->cursprite],
		obj->spriteszx, obj->spriteszy, color);
}

void move_object(object_s *obj)
{
    object_s oldobj;
	
	memcpy(&oldobj, obj, sizeof(object_s));
	
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

int check_x(const object_s* obj) {
    const unsigned int px = obj->posx + obj->dx;
    return (px <= 0 || px >= 288);
}

int check_y(const object_s* obj) {
    const unsigned int py = obj->posy + obj->dy;
    //return (py <= 0 || py >= 209);
    return (py <= 18 || py >= 210);
}

int check_screen_limits(const object_s* obj) {
    return !(check_x(obj) || check_y(obj));
}

void init_display() {
	display_background(BLACK);
}

//-----------------------------------------------------------
//                    INPUT HANDLER
//-----------------------------------------------------------
enum {
	KEY_CENTER	= 0x01,		//MASK8
	KEY_UP		= 0x02,		//MASK9
	KEY_LEFT	= 0x04,		//MASK10
	KEY_RIGHT	= 0x08,		//MASK11
	KEY_DOWN	= 0x10		//MASK12
};

void init_input() {
	/* configure GPIOB pins 8 .. 12 as inputs */
	GPIOB->DDR &= ~(MASK_P8 | MASK_P9 | MASK_P10 | MASK_P11 | MASK_P12);
}

int get_input() {
	int keys = 0;
	if (GPIOB->IN & MASK_P8)	keys |= KEY_CENTER;
	if (GPIOB->IN & MASK_P9)	keys |= KEY_UP;
	if (GPIOB->IN & MASK_P10)	keys |= KEY_LEFT;
	if (GPIOB->IN & MASK_P11)	keys |= KEY_RIGHT;
	if (GPIOB->IN & MASK_P12)	keys |= KEY_DOWN;
		
	return keys;
}

//----------------------------------------------------------------------------
//                      COLLISION LOGIC 
//----------------------------------------------------------------------------
int check_collision_new(const object_s* obj1, const object_s* obj2) {
    return (
        (obj1->posx <= obj2->posx) &&
        (obj1->posx + obj1->spriteszx-1 >= obj2->posx) &&
        (obj1->posy <= obj2->posy + obj2->spriteszy-1) &&
        (obj1->posy + obj1->spriteszy-1 >= obj2->posy + obj2->spriteszy-1)
    );
}
int check_collision(const object_s* obj, const object_s* enemy) {
    const unsigned int px = obj->posx + obj->dx;
    const unsigned int py = obj->posy + obj->dy;
    return (
        (px >= enemy->posx && px <= enemy->posx + enemy->spriteszx) &&
        (py >= enemy->posy && py <= enemy->posy + enemy->spriteszy) &&
        enemy->alive
    );
}

//----------------------------------------------------------------
//                       SCOREBOARD
//----------------------------------------------------------------
void display_scoreboard() {
    strcpy(old_score_str, score_str);
    sprintf(score_str, "%5d", score);
	display_print("SCORE:",  WIDTH*0.1,  5, 1, WHITE);
    display_print(old_score_str, WIDTH*0.25, 5, 1, BLACK);
    display_print(score_str, WIDTH*0.25, 5, 1, LGREEN);
	display_print("LIVES:",  WIDTH*0.6,  5, 1, WHITE);
    for(int i=0; i<3; i++) {
        draw_object(&hearts[i], 0, (lives - i > 0) ? -1 : 0);
    }
}

//-------------------------------------------------------------------------------------
//                          PROJECTILE LOGIC
//-------------------------------------------------------------------------------------
void create_proj(object_s* proj, const int posx, const int posy, const int speed){
	proj->dy     = speed;
	proj->posx   = posx;
	proj->posy   = posy;
    proj->visible = 1;
}

void destroy_proj(object_s* proj) {
	draw_object(proj, 1, 0);
    proj->dy = 0;
    proj->visible = 0;
	proj->posx = 0;
    proj->posy = 0;
}

void check_player_hit() {
    for(int i=0; i<ENEMIES_LIN; i++)
    {
        for(int j=0; j<ENEMIES_COL; j++)
        {
            if(check_collision(&player_proj, &enemies[i][j]))
            {
               enemies[i][j].alive = 0; 
               destroy_proj(&player_proj);
               score += 10;
			   display_frectangle(45, 20, 150, 10, BLACK);
            }
        }
    }
}

//-----------------------------------------------------------------------------------
//                      PLAYER MOVEMENT
//-----------------------------------------------------------------------------------
void player_controls(const int input){
    earthprotector_obj.dx = 0;
    earthprotector_obj.dy = 0;

    // left
    if((input & KEY_LEFT) != 0) {
        if(input & KEY_RIGHT) {
            earthprotector_obj.dx = 0;
        }
        else {
            if(check_screen_limits(&earthprotector_obj)) {
                earthprotector_obj.dx = -player_speed;
            }
        }
    } 
    
    // right 
    if((input & KEY_RIGHT) != 0) {
        if(input & KEY_LEFT) {
            earthprotector_obj.dx = 0;
        }
        else if(check_screen_limits(&earthprotector_obj)) {
            earthprotector_obj.dx = player_speed;
        }
    } 

    // shoot 
    if((input & KEY_CENTER && !player_proj.visible) != 0) {
        create_proj(&player_proj, earthprotector_obj.posx + 6, earthprotector_obj.posy - 10, -5);
    }
}

//----------------------------------------------------------------------------------------
//                                 ENEMY LOGIC
//----------------------------------------------------------------------------------------
void enemies_init()
{
    int i=0;
    for(int j=0; j<ENEMIES_COL; j++) {
			init_object(
                &enemies[i][j],         
                alien02_moving_a[0], alien02_moving_b[0], 0,                     
                11, 8,                  
                18 + 15*(j+1), 18 + 12*(i+1),          
                0, 0, 10, 10, 1, 1
            );

            init_object(
                &enemies_proj[i][j],
                player_shoot[0], 0, 0,
                1, 8,
                enemies[i][j].posx + 6, enemies[i][j].posy + 10,
                0, 5, 0, 1, 0, 0
            );
    } i++; 

    for(int j=0; j<ENEMIES_COL; j++) {
			init_object(
                &enemies[i][j],         
                alien01_moving_a[0], alien01_moving_b[0], 0,                     
                11, 8,                  
                18 + 15*(j+1),  18 + 12*(i+1),          
                0, 0, 10, 10, 1, 1
            );

            init_object(
                &enemies_proj[i][j],
                player_shoot[0], 0, 0,
                1, 8,
                enemies[i][j].posx + 6, enemies[i][j].posy + 10,
                0, 5, 0, 1, 0, 0
            );
    } i++; 

    for(int j=0; j<ENEMIES_COL; j++) {
			init_object(
                &enemies[i][j],         
                alien01_moving_a[0], alien01_moving_b[0], 0,                     
                11, 8,                  
                18 + 15*(j+1),  18 + 12*(i+1),          
                0, 0, 10, 10, 1, 1
            );

            init_object(
                &enemies_proj[i][j],
                player_shoot[0], 0, 0,
                1, 8,
                enemies[i][j].posx + 6, enemies[i][j].posy + 10,
                0, 5, 0, 1, 0, 0
            );
    } i++; 

    for(int j=0; j<ENEMIES_COL; j++) {
			init_object(
                &enemies[i][j],         
                alien03_moving_a[0], alien03_moving_b[0], 0,                     
                11, 8,                  
                18 + 15*(j+1),  18 + 12*(i+1),          
                0, 0, 10, 10, 1, 1
            );

            init_object(
                &enemies_proj[i][j],
                player_shoot[0], 0, 0,
                1, 8,
                enemies[i][j].posx + 6, enemies[i][j].posy + 10,
                0, 5, 0, 1, 0, 0
            );
    } i++; 

    for(int j=0; j<ENEMIES_COL; j++) {
			init_object(
                &enemies[i][j],         
                alien03_moving_a[0], alien03_moving_b[0], 0,                     
                11, 8,                  
                18 + 15*(j+1),  18 + 12*(i+1),          
                0, 0, 10, 10, 1, 1
            );

            init_object(
                &enemies_proj[i][j],
                player_shoot[0], 0, 0,
                1, 8,
                enemies[i][j].posx + 6, enemies[i][j].posy + 10,
                0, 5, 0, 1, 0, 0
            );
    } i++; 
}
void enemy_fire(const int i, const int j)
{
    const int r = random();
    
    if(enemies_proj[i][j].visible) {
        if(check_screen_limits(&enemies_proj[i][j])) {
            move_object(&enemies_proj[i][j]);
        }
        else {
            destroy_proj(&enemies_proj[i][j]);
            enemy_shots--;
        }
    }
    else {
        if(r % 333 == 0 && enemy_shots < max_enemy_shots) {
            enemy_shots++;
            create_proj(&enemies_proj[i][j], enemies[i][j].posx + 5, 
                        enemies[i][j].posy + 10, 5);
            //create_proj(&enemies_proj[i][j], enemies[i][j].posx + 6, 
            //            enemies[i][j].posy + 10, 5);
        }
    }
}

void draw_enemies(){
    const int out = check_x(&enemies[0][0]) || check_x(&enemies[0][ENEMIES_COL-1]);

    draw_object(&enemy_explosion_obj, 1, 0);
	enemy_explosion_obj.posx = 0;
	enemy_explosion_obj.posy = 0;

	for(int i=0; i<ENEMIES_LIN; i++) {
        for(int j=0; j<ENEMIES_COL; j++){
			if(enemies[i][j].alive) {
                enemies[i][j].dy = out * 13;  
                enemies[i][j].dx *= (1 - (out << 1)); 
                enemies[i][j].speedycnt = (out) ? 1 : 5;
				move_object(&enemies[i][j]);

                enemy_fire(i, j);
			}
            else if(enemies[i][j].visible) {
                // enemy death animation 
				draw_object(&enemies[i][j], 2, 0);
				enemy_explosion_obj.posx = enemies[i][j].posx;
				enemy_explosion_obj.posy = enemies[i][j].posy;
				draw_object(&enemies[i][j], 2, 0);
				draw_object(&enemy_explosion_obj, 0, -1);
				enemies[i][j].visible = 0;
			}
        }
    }

}

int player_death()
{
    for(int i=0; i<ENEMIES_LIN; i++)
    {
        for(int j=0; j<ENEMIES_COL; j++)
        {
            if(
               earthprotector_obj.posx <= enemies_proj[i][j].posx && 
               earthprotector_obj.posx+10 >= enemies_proj[i][j].posx &&
               earthprotector_obj.posy <= (enemies_proj[i][j].posy + 8) &&
               earthprotector_obj.posy+7 >= (enemies_proj[i][j].posy + 8)
            ) { return (--lives == 0) ? 2 : 1; }
            if(
               earthprotector_obj.posx <= enemies[i][j].posx &&
               earthprotector_obj.posx+earthprotector_obj.spriteszx-1 >= enemies[i][j].posx &&
               earthprotector_obj.posy <= (enemies[i][j].posy + enemies[i][j].spriteszy-1) &&
               earthprotector_obj.posy+earthprotector_obj.spriteszy-1 >= (enemies[i][j].posy + enemies[i][j].spriteszy-1)
            ) { return 2; } 
        }
    }

    return 0;
}

void restart()
{
    //delay_ms(1500);

    player_death_obj.posx = earthprotector_obj.posx;
    player_death_obj.posy = earthprotector_obj.posy;

    destroy_proj(&player_proj);
    draw_object(&earthprotector_obj, 0, BLACK);

    for(int i=0; i<100; i++)
    {
        //draw_object(&player_death_obj, 0, -1);
        move_object(&player_death_obj);
        delay_ms(40);
    }

  //  draw_object(&player_death_obj, 0, -1);
    earthprotector_obj.posx = player_xo;
    earthprotector_obj.posy = player_yo;
    draw_object(&player_death_obj, 0, BLACK);
    move_object(&earthprotector_obj);

    for(int i=0; i<ENEMIES_LIN; i++)
    {
        for(int j=0; j<ENEMIES_COL; j++)
        {
            destroy_proj(&enemies_proj[i][j]);
        }
    }

    enemy_shots = 0;
}

void finish() {
        display_print("GAME OVER",  WIDTH*0.35,  HEIGHT/2, 1, RED);
}

//-----------------------------------------------------------------------------
//                          "BARRIER" LOGIC
//-----------------------------------------------------------------------------
void draw_barrier(const barrier_t* b, const int color) {
    display_frectangle(b->xo, b->yo, b->w, b->h, color);
}

void check_barrier_hit(const object_s* proj, barrier_t* barr) {
    if  (
        (proj->posy + proj->spriteszy == barr->yo) &&
        (proj->posx >= barr->xo) &&
        (proj->posx <= barr->xo + barr->w)
    ) { barr->hp--; }
}

int main()
{
	init_display();
	init_input();

    enemies_init();

    // player initialization 
	init_object(
        &earthprotector_obj, 
        earthprotector[0], 
        0, 
        0, 
        11, 8, 
        150,   // 150 
        200,   // 186 
        3, 0, 
        2, 2, 
        1, 1
    );
    
    // player projectile initialization
	init_object(
        &player_proj,
        player_shoot[0],     
        0,                   
        0, 
        1, 8, 
        155, 
        172, 
        0, -5, 
        0, 1, 
        0, 0
    );

    // enemy death animation 
	init_object(
            &enemy_explosion_obj, 
            enemy_explosion[0], 
            0, 
            0, 
            11, 8, 
            0, 
            0, 
            0, 0, 
            0, 0, 
            0, 0
    );

    // player death animation
	init_object(
            &player_death_obj, 
            player_death1[0], 
            player_death2[0], 
            0, 
            11, 8, 
            0, 
            0, 
            0, 0, 
            5, 5, 
            0, 0
    );

    for(int i=0; i<3; i++) {
        init_object(
                &hearts[i], 
                heart[0], 
                0, 
                0, 
                11, 10, 
                WIDTH*0.77 + (i*14), 
                5, 
                0, 0, 
                0, 0, 
                0, 0
        );
    }

    // game loop
    while(1)
    {
        const int status = player_death();
        display_scoreboard();

        if(status == 1) {
            restart();
        }
        else if(status == 2) {
            finish();
            break;
        }

        for(int i=0; i<ENEMIES_LIN; i++) {
            for(int j=0; j<ENEMIES_COL; j++) {
               check_barrier_hit(&enemies_proj[i][j], &b1);
               check_barrier_hit(&enemies_proj[i][j], &b2);
               check_barrier_hit(&enemies_proj[i][j], &b3);
            }
        }

        if(b1.hp > 0) {
            draw_barrier(&b1, GREEN);
        } else {
            draw_barrier(&b1, BLACK);
        }

        if(b2.hp > 0) {
            draw_barrier(&b2, GREEN);
        } else {
            draw_barrier(&b2, BLACK);
        }

        if(b3.hp > 0) {
            draw_barrier(&b3, GREEN);
        } else {
            draw_barrier(&b3, BLACK);
        }

    //    move_object(&barrier1_l1);
    //    move_object(&barrier1_r1);
    //    move_object(&barrier1_l2);
    //    move_object(&barrier1_r2);

        const int input = get_input();
        player_controls(input);
        move_object(&earthprotector_obj);

        check_player_hit();

        //move_enemies();
        draw_enemies();

        if(player_proj.visible) {
            if(check_screen_limits(&player_proj)) {
                move_object(&player_proj);
            }
            else {
                destroy_proj(&player_proj);
            }
        }

        delay_ms(40);
    }

    return 0;
}
