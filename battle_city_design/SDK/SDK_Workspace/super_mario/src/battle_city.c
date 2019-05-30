#include "battle_city.h"
#include "map.h"
#include "xparameters.h"
#include "xil_io.h"
#include "xio.h"
#include <math.h>
#include <stdlib.h>

/*
 * GENERATED BY BC_MEM_PACKER
 * DATE: Wed Jul 08 21:00:48 2015
 */

// ***** 16x16 IMAGES *****
#define IMG_16x16_bg00			0x00FF
#define IMG_16x16_bg01			0x013F
#define IMG_16x16_bg02			0x017F
#define IMG_16x16_bg10			0x01BF
#define IMG_16x16_bg11			0x01FF
#define IMG_16x16_bg12			0x023F
#define IMG_16x16_bg20			0x027F
#define IMG_16x16_bg21			0x02BF
#define IMG_16x16_bg22			0x02FF
#define IMG_16x16_bg30			0x033F
#define IMG_16x16_bg31			0x037F
#define IMG_16x16_bg32			0x03BF
#define IMG_16x16_bg40			0x03FF
#define IMG_16x16_bg41			0x043F
#define IMG_16x16_bg42			0x047F
#define IMG_16x16_bg50			0x04BF
#define IMG_16x16_bg51			0x04FF
#define IMG_16x16_bg52			0x053F
#define IMG_16x16_black			0x057F
#define IMG_16x16_cursor		0x05BF
#define IMG_16x16_pl00			0x05FF
#define IMG_16x16_pl01			0x063F
#define IMG_16x16_pl10			0x067F
#define IMG_16x16_pl11			0x06BF
#define IMG_16x16_pr00			0x06FF
#define IMG_16x16_pr01			0x073F
#define IMG_16x16_pr10			0x077F
#define IMG_16x16_pr11			0x07BF
#define IMG_16x16_sky			0x07FF
#define IMG_16x16_white			0x083F


// ***** MAP *****
#define MAP_BASE_ADDRESS			2175 // MAP_OFFSET in battle_city.vhd
#define MAP_X							0
#define MAP_X2							640
#define MAP_Y							4
#define MAP_W							64
#define MAP_H							56

#define REGS_BASE_ADDRESS               ( MAP_BASE_ADDRESS + MAP_WIDTH * MAP_HEIGHT )
//#define REGS_BASE_ADDRESS               (6639)

#define BTN_DOWN( b )                   ( !( b & 0x01 ) )
#define BTN_UP( b )                     ( !( b & 0x10 ) )
#define BTN_LEFT( b )                   ( !( b & 0x02 ) )
#define BTN_RIGHT( b )                  ( !( b & 0x08 ) )
#define BTN_SHOOT( b )                  ( !( b & 0x04 ) )

#define TANK1_REG_L                     8
#define TANK1_REG_H                     9
#define TANK_AI_REG_L                   4
#define TANK_AI_REG_H                   5
#define TANK_AI_REG_L2                  6
#define TANK_AI_REG_H2                  7
#define TANK_AI_REG_L3                  2
#define TANK_AI_REG_H3                  3
#define TANK_AI_REG_L4                  10
#define TANK_AI_REG_H4                  11
#define TANK_AI_REG_L5                  12
#define TANK_AI_REG_H5                  13
#define TANK_AI_REG_L6                  14
#define TANK_AI_REG_H6                  15
#define TANK_AI_REG_L7                  16
#define TANK_AI_REG_H7                  17
#define BASE_REG_L						0
#define BASE_REG_H	                    1

int lives = 3;
int score = 0;
int mapPart = 1;
int udario_glavom_skok = 0;
int map_move = 0;
int brojac = 0;
int udario_u_blok = 0;
int direction = 0;
unsigned int buttons;

int broj_promasaja = 0;
unsigned int bool_promasaj = 0;

typedef enum {
	b_false, b_true
} bool_t;

typedef enum {
	DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN, DIR_STILL
} direction_t;

typedef struct {
	unsigned int x;
	unsigned int y;
	direction_t dir;
	unsigned int type;

	bool_t destroyed;

	unsigned int reg_l;
	unsigned int reg_h;
} characters;

characters cursor = { 300,						// x
		200,						// y
		DIR_LEFT,              		// dir
		IMG_16x16_cursor,  		// type

		b_false,                		// destroyed

		TANK_AI_REG_L5,            		// reg_l
		TANK_AI_REG_H5             		// reg_h
		};

characters duck1_right = { 160,	                        // x
		131, 		                     // y
		DIR_RIGHT,              		// dir
		IMG_16x16_pr00,  			// type

		b_false,                		// destroyed

		TANK1_REG_L,            		// reg_l
		TANK1_REG_H             		// reg_h
		};

characters duck2_right = { 178,	                        // x
		131, 		                     // y
		DIR_RIGHT,              		// dir
		IMG_16x16_pr01,  			// type

		b_false,                		// destroyed

		TANK_AI_REG_L2,            		// reg_l
		TANK_AI_REG_H2             		// reg_h
		};

characters duck3_right = { 160,	                        // x
		147, 		                     // y
		DIR_RIGHT,              		// dir
		IMG_16x16_pr10,  			// type

		b_false,                		// destroyed

		TANK_AI_REG_L3,            		// reg_l
		TANK_AI_REG_H3             		// reg_h
		};

characters duck4_right = { 174,	                        // x
		147, 		                     // y
		DIR_RIGHT,              		// dir
		IMG_16x16_pr11,  			// type

		b_false,                		// destroyed

		TANK_AI_REG_L4,            		// reg_l
		TANK_AI_REG_H4             		// reg_h
		};

characters duck1_left = { 162,	                        // x
		131, 		                     // y
		DIR_RIGHT,              		// dir
		IMG_16x16_pl00,  			// type

		b_false,                		// destroyed

		TANK1_REG_L,            		// reg_l
		TANK1_REG_H             		// reg_h
		};

characters duck2_left = { 176,	                        // x
		131, 		                     // y
		DIR_RIGHT,              		// dir
		IMG_16x16_pl01,  			// type

		b_false,                		// destroyed

		TANK_AI_REG_L2,            		// reg_l
		TANK_AI_REG_H2             		// reg_h
		};

characters duck3_left = { 162,	                        // x
		147, 		                     // y
		DIR_RIGHT,              		// dir
		IMG_16x16_pl10,  			// type

		b_false,                		// destroyed

		TANK_AI_REG_L3,            		// reg_l
		TANK_AI_REG_H3             		// reg_h
		};

characters duck4_left = { 176,	                        // x
		147, 		                     // y
		DIR_RIGHT,              		// dir
		IMG_16x16_pl11,  			// type

		b_false,                		// destroyed

		TANK_AI_REG_L4,            		// reg_l
		TANK_AI_REG_H4             		// reg_h
		};


unsigned int rand_lfsr113(void) {
	static unsigned int z1 = 12345, z2 = 12345;
	unsigned int b;

	b = ((z1 << 6) ^ z1) >> 13;
	z1 = ((z1 & 4294967294U) << 18) ^ b;
	b = ((z2 << 2) ^ z2) >> 27;
	z2 = ((z2 & 4294967288U) << 2) ^ b;

	return (z1 ^ z2);
}

static void chhar_spawn(characters * chhar) {
	Xil_Out32(
			XPAR_BATTLE_CITY_PERIPH_0_BASEADDR + 4 * ( REGS_BASE_ADDRESS + chhar->reg_l ),
			(unsigned int )0x8F000000 | (unsigned int )chhar->type);
	Xil_Out32(
			XPAR_BATTLE_CITY_PERIPH_0_BASEADDR + 4 * ( REGS_BASE_ADDRESS + chhar->reg_h ),
			(chhar->y << 16) | chhar->x);

}

static void chhar_spawn_duck(characters *duck1, characters *duck2,
		characters *duck3, characters *duck4) {
	Xil_Out32(
			XPAR_BATTLE_CITY_PERIPH_0_BASEADDR + 4 * ( REGS_BASE_ADDRESS + duck1->reg_l ),
			(unsigned int )0x8F000000 | (unsigned int )duck1->type);
	Xil_Out32(
			XPAR_BATTLE_CITY_PERIPH_0_BASEADDR + 4 * ( REGS_BASE_ADDRESS + duck2->reg_l ),
			(unsigned int )0x8F000000 | (unsigned int )duck2->type);
	Xil_Out32(
			XPAR_BATTLE_CITY_PERIPH_0_BASEADDR + 4 * ( REGS_BASE_ADDRESS + duck3->reg_l ),
			(unsigned int )0x8F000000 | (unsigned int )duck3->type);
	Xil_Out32(
			XPAR_BATTLE_CITY_PERIPH_0_BASEADDR + 4 * ( REGS_BASE_ADDRESS + duck4->reg_l ),
			(unsigned int )0x8F000000 | (unsigned int )duck4->type);
	Xil_Out32(
			XPAR_BATTLE_CITY_PERIPH_0_BASEADDR + 4 * ( REGS_BASE_ADDRESS + duck1->reg_h ),
			(duck1->y << 16) | duck1->x);
	Xil_Out32(
			XPAR_BATTLE_CITY_PERIPH_0_BASEADDR + 4 * ( REGS_BASE_ADDRESS + duck2->reg_h ),
			(duck2->y << 16) | duck2->x);
	Xil_Out32(
			XPAR_BATTLE_CITY_PERIPH_0_BASEADDR + 4 * ( REGS_BASE_ADDRESS + duck3->reg_h ),
			(duck3->y << 16) | duck3->x);
	Xil_Out32(
			XPAR_BATTLE_CITY_PERIPH_0_BASEADDR + 4 * ( REGS_BASE_ADDRESS + duck4->reg_h ),
			(duck4->y << 16) | duck4->x);
}

static void chhar_spawn_duck_move(characters *duck1, characters *duck2,
		characters *duck3, characters *duck4) {

	Xil_Out32(
			XPAR_BATTLE_CITY_PERIPH_0_BASEADDR + 4 * ( REGS_BASE_ADDRESS + duck1->reg_h ),
			(duck1->y << 16) | duck1->x);
	Xil_Out32(
			XPAR_BATTLE_CITY_PERIPH_0_BASEADDR + 4 * ( REGS_BASE_ADDRESS + duck2->reg_h ),
			(duck2->y << 16) | duck2->x);
	Xil_Out32(
			XPAR_BATTLE_CITY_PERIPH_0_BASEADDR + 4 * ( REGS_BASE_ADDRESS + duck3->reg_h ),
			(duck3->y << 16) | duck3->x);
	Xil_Out32(
			XPAR_BATTLE_CITY_PERIPH_0_BASEADDR + 4 * ( REGS_BASE_ADDRESS + duck4->reg_h ),
			(duck4->y << 16) | duck4->x);
}

static void map_update() {
	int x, y;
	long int addr;

	for (y = 0; y < MAP_HEIGHT; y++) {
		for (x = 0; x < MAP_WIDTH; x++) {
			addr = XPAR_BATTLE_CITY_PERIPH_0_BASEADDR
					+ 4 * (MAP_BASE_ADDRESS + y * MAP_WIDTH + x);
			switch (map1[y][x + map_move]) {
			case 0:
				Xil_Out32(addr, IMG_16x16_sky);
				break;
			case 1:
				Xil_Out32(addr, IMG_16x16_cursor);
				break;
			case 2:
				Xil_Out32(addr, IMG_16x16_pr00);
				break;
			case 3:
				Xil_Out32(addr, IMG_16x16_pr01);
				break;
			case 4:
				Xil_Out32(addr, IMG_16x16_pr10);
				break;
			case 5:
				Xil_Out32(addr, IMG_16x16_pr11);
				break;
			case 6:
				Xil_Out32(addr, IMG_16x16_pl00);
				break;
			case 7:
				Xil_Out32(addr, IMG_16x16_pl01);
				break;
			case 8:
				Xil_Out32(addr, IMG_16x16_pl10);
				break;
			case 9:
				Xil_Out32(addr, IMG_16x16_pl11);
				break;
			case 10:
				Xil_Out32(addr, IMG_16x16_pr10);
				break;
			case 11:
				Xil_Out32(addr, IMG_16x16_pr11);
				break;
			case 12:
				Xil_Out32(addr, IMG_16x16_bg00);
				break;
			case 13:
				Xil_Out32(addr, IMG_16x16_bg01);
				break;
			case 14:
				Xil_Out32(addr, IMG_16x16_bg02);
				break;
			case 15:
				Xil_Out32(addr, IMG_16x16_bg10);
				break;
			case 16:
				Xil_Out32(addr, IMG_16x16_bg11);
				break;
			case 17:
				Xil_Out32(addr, IMG_16x16_bg12);
				break;
			case 18:
				Xil_Out32(addr, IMG_16x16_bg20);
				break;
			case 19:
				Xil_Out32(addr, IMG_16x16_bg21);
				break;
			case 20:
				Xil_Out32(addr, IMG_16x16_bg22);
				break;
			case 21:
				Xil_Out32(addr, IMG_16x16_bg30);
				break;
			case 22:
				Xil_Out32(addr, IMG_16x16_bg31);
				break;
			case 23:
				Xil_Out32(addr, IMG_16x16_bg32);
				break;
			case 24:
				Xil_Out32(addr, IMG_16x16_bg40);
				break;
			case 25:
				Xil_Out32(addr, IMG_16x16_bg41);
				break;
			case 26:
				Xil_Out32(addr, IMG_16x16_bg42);
				break;
			case 27:
				Xil_Out32(addr, IMG_16x16_bg50);
				break;
			case 28:
				Xil_Out32(addr, IMG_16x16_bg51);
				break;
			case 29:
				Xil_Out32(addr, IMG_16x16_bg52);
				break;
			default:
				Xil_Out32(addr, IMG_16x16_sky);
				break;
			}
		}
	}
}

static void map_update_shot(){
	int x, y;
		long int addr;

		for (y = 0; y < MAP_HEIGHT; y++) {
			for (x = 0; x < MAP_WIDTH; x++) {
				addr = XPAR_BATTLE_CITY_PERIPH_0_BASEADDR
						+ 4 * (MAP_BASE_ADDRESS + y * MAP_WIDTH + x);
				Xil_Out32(addr, IMG_16x16_black);
				}
		}
}

static void map_reset(unsigned char * map) {

	unsigned int i;

	for (i = 0; i <= 20; i += 2) {
		Xil_Out32(
				XPAR_BATTLE_CITY_PERIPH_0_BASEADDR + 4 * ( REGS_BASE_ADDRESS + i ),
				(unsigned int )0x0F000000);
	}

}

int random_direction() {
	int number;
	number = rand() % 8;
	return number;
}

int random() {
	int number;
	number = rand() % 150 + 50;
	return number;
}

int random_x() {
	return rand() % 600;
}

static void cursor_move() {

	/*buttons = XIo_In32( XPAR_IO_PERIPH_BASEADDR );
	direction_t d = DIR_STILL;
	if (BTN_LEFT(buttons)) {
		d = DIR_LEFT;
	} else if (BTN_RIGHT(buttons)) {
		d = DIR_RIGHT;
	} else if (BTN_UP(buttons)) {
		d = DIR_UP;
	} else if (BTN_DOWN(buttons)) {
		d = DIR_DOWN;
	}

	switch (d) {
	case DIR_DOWN:
		cursor.y += 2;
		break;
	case DIR_UP:
		cursor.y -= 2;
		break;
	case DIR_LEFT:
		cursor.x -= 2;
		break;
	case DIR_RIGHT:
		cursor.x += 2;
		break;
	case DIR_STILL:
		cursor.x = cursor.x;
		cursor.y = cursor.y;
		break;
	default:
		cursor.x = cursor.x;
		cursor.y = cursor.y;
	}
	chhar_spawn(&cursor);*/
}

bool_t crash_detection() {
	buttons = XIo_In32( XPAR_IO_PERIPH_BASEADDR );
	if (BTN_SHOOT(buttons)) {
		if (cursor.x > duck1_left.x && cursor.x < (duck1_left.x + 16)
				&& cursor.y > duck1_left.y && cursor.y < (duck1_left.y + 16)) {
			return b_true;
		} else {
			return b_false;
		}
	}
	return b_false;
}

void reset_duck() {
	duck1_left.y = 320;
	duck2_left.y = 320;
	duck3_left.y = 336;
	duck4_left.y = 336;
	duck1_left.x = random_x();
	duck2_left.x = duck1_left.x + 16;
	duck3_left.x = duck1_left.x;
	duck4_left.x = duck1_left.x + 16;

	duck1_right.y = 320;
	duck2_right.y = 320;
	duck3_right.y = 336;
	duck4_right.y = 336;
	duck1_right.x = duck1_left.x;
	duck2_right.x = duck1_left.x + 16;
	duck3_right.x = duck1_left.x;
	duck4_right.x = duck1_left.x + 16;
}

static void duck_move() {
	int i, j;
	unsigned int y;

	direction = random_direction();

	if (direction == 0) {

		y = random();
		for (i = 0; i < y; i++) {
			duck1_left.y -= 1;
			duck2_left.y -= 1;
			duck3_left.y -= 1;
			duck4_left.y -= 1;
			duck1_left.x -= 1;
			duck2_left.x -= 1;
			duck3_left.x -= 1;
			duck4_left.x -= 1;

			duck1_right.y -= 1;
			duck2_right.y -= 1;
			duck3_right.y -= 1;
			duck4_right.y -= 1;
			duck1_right.x -= 1;
			duck2_right.x -= 1;
			duck3_right.x -= 1;
			duck4_right.x -= 1;
			cursor_move();
			chhar_spawn_duck(&duck1_left, &duck2_left, &duck3_left,
					&duck4_left);
			if (crash_detection() == b_true) {
				//lives--;
				for (j = 0; j < 10000000; j++) {
				}
				//if (lives < 1) {
				//	while (1)
				//		;
				//}
				reset_duck();
			}

			for (j = 0; j < 100000; j++) {
			}
			if (duck1_left.x < 10 || duck1_left.y < 10) {
				return;
			}

		}

	} else if (direction == 1) {
		y = random();
		for (i = 0; i < y; i++) {
			duck1_left.y -= 1;
			duck2_left.y -= 1;
			duck3_left.y -= 1;
			duck4_left.y -= 1;
			duck1_right.y -= 1;
			duck2_right.y -= 1;
			duck3_right.y -= 1;
			duck4_right.y -= 1;
			chhar_spawn_duck(&duck1_left, &duck2_left, &duck3_left,
					&duck4_left);
			cursor_move();
			if (crash_detection() == b_true) {
				//lives--;
				for (j = 0; j < 10000000; j++) {
				}
				//if (lives < 1) {
				//	while (1)
				//		;
				//}
				reset_duck();
			}
			for (j = 0; j < 100000; j++) {
			}
			if (duck1_left.y < 10) {
				return;
			}

		}

	} else if (direction == 2) {

		y = random();
		for (i = 0; i < y; i++) {
			duck1_left.y -= 1;
			duck2_left.y -= 1;
			duck3_left.y -= 1;
			duck4_left.y -= 1;
			duck1_right.y -= 1;
			duck2_right.y -= 1;
			duck3_right.y -= 1;
			duck4_right.y -= 1;

			duck1_left.x += 1;
			duck2_left.x += 1;
			duck3_left.x += 1;
			duck4_left.x += 1;
			duck1_right.x += 1;
			duck2_right.x += 1;
			duck3_right.x += 1;
			duck4_right.x += 1;
			chhar_spawn_duck(&duck1_right, &duck2_right, &duck3_right,
					&duck4_right);
			cursor_move();
			if (crash_detection() == b_true) {
				lives--;
				for (j = 0; j < 10000000; j++) {
				}
				if (lives < 1) {
					//while (1)
						//;
				}
				reset_duck();
			}
			for (j = 0; j < 100000; j++) {
			}
			if (duck1_right.x > 600 || duck1_right.y < 10) {
				return;
			}

		}

	} else if (direction == 3) {
		y = random();
		for (i = 0; i < y; i++) {
			duck1_right.x -= 1;
			duck2_right.x -= 1;
			duck3_right.x -= 1;
			duck4_right.x -= 1;
			duck1_left.x -= 1;
			duck2_left.x -= 1;
			duck3_left.x -= 1;
			duck4_left.x -= 1;

			chhar_spawn_duck(&duck1_left, &duck2_left, &duck3_left,
					&duck4_left);
			cursor_move();
			if (crash_detection() == b_true) {
				lives--;
				for (j = 0; j < 10000000; j++) {
				}
				if (lives < 1) {
					//while (1)
					//	;
				}
				reset_duck();
			}
			for (j = 0; j < 100000; j++) {
			}
			if (duck1_left.x < 10) {
				return;
			}

		}

	} else if (direction == 4) {
		y = random();
		for (i = 0; i < y; i++) {
			duck1_left.x += 1;
			duck2_left.x += 1;
			duck3_left.x += 1;
			duck4_left.x += 1;
			duck1_right.x += 1;
			duck2_right.x += 1;
			duck3_right.x += 1;
			duck4_right.x += 1;

			chhar_spawn_duck(&duck1_right, &duck2_right, &duck3_right,
					&duck4_right);
			cursor_move();
			if (crash_detection() == b_true) {
				lives--;
				for (j = 0; j < 10000000; j++) {
				}
				if (lives < 1) {
					//while (1)
					//	;
				}
				reset_duck();
			}
			for (j = 0; j < 100000; j++) {
			}

			if (duck1_right.x > 600) {
				return;
			}
		}

	} else if (direction == 5) {

		y = random();
		for (i = 0; i < y; i++) {
			duck1_right.x -= 1;
			duck2_right.x -= 1;
			duck3_right.x -= 1;
			duck4_right.x -= 1;
			duck1_left.x -= 1;
			duck2_left.x -= 1;
			duck3_left.x -= 1;
			duck4_left.x -= 1;
			duck1_left.y += 1;
			duck2_left.y += 1;
			duck3_left.y += 1;
			duck4_left.y += 1;
			duck1_right.y += 1;
			duck2_right.y += 1;
			duck3_right.y += 1;
			duck4_right.y += 1;

			chhar_spawn_duck(&duck1_left, &duck2_left, &duck3_left,
					&duck4_left);
			cursor_move();
			if (crash_detection() == b_true) {
				//lives--;
				for (j = 0; j < 10000000; j++) {
				}
				if (lives < 1) {
					//while (1)
					//	;
				}
				reset_duck();
			}
			for (j = 0; j < 100000; j++) {
			}
			if (duck1_right.x < 10 || duck1_right.y > 320) {
				return;
			}

		}

	} else if (direction == 6) {
		y = random();

		for (i = 0; i < y; i++) {
			duck1_left.y += 1;
			duck2_left.y += 1;
			duck3_left.y += 1;
			duck4_left.y += 1;
			duck1_right.y += 1;
			duck2_right.y += 1;
			duck3_right.y += 1;
			duck4_right.y += 1;

			chhar_spawn_duck(&duck1_left, &duck2_left, &duck3_left,
					&duck4_left);
			cursor_move();
			if (crash_detection() == b_true) {
				//lives--;
				for (j = 0; j < 10000000; j++) {
				}
				if (lives < 1) {
					//while (1)
					//	;
				}
				reset_duck();
			}
			for (j = 0; j < 100000; j++) {
			}
			if (duck1_right.y > 320) {
				return;
			}

		}

	} else if (direction == 7) {

		y = random();
		for (i = 0; i < y; i++) {
			duck1_left.x += 1;
			duck2_left.x += 1;
			duck3_left.x += 1;
			duck4_left.x += 1;
			duck1_right.x += 1;
			duck2_right.x += 1;
			duck3_right.x += 1;
			duck4_right.x += 1;
			duck1_left.y += 1;
			duck2_left.y += 1;
			duck3_left.y += 1;
			duck4_left.y += 1;
			duck1_right.y += 1;
			duck2_right.y += 1;
			duck3_right.y += 1;
			duck4_right.y += 1;

			chhar_spawn_duck(&duck1_right, &duck2_right, &duck3_right,
					&duck4_right);
			cursor_move();
			if (crash_detection() == b_true) {
				//lives--;
				for (j = 0; j < 10000000; j++) {
				}
				//if (lives < 1) {
				//	while (1)
				//		;
				//}
				reset_duck();
			}
			for (j = 0; j < 100000; j++) {
			}
			if (duck1_right.x > 600 || duck1_right.y > 320) {
				return;
			}

		}

	}

}

bool stoljpi(){
			u32 output;
			static int j=0;
	    	//postavljanje o_pwr na 0 (citanje okidaca)

	    	bool o_pwr = 0;
	    	output = ((u32)o_pwr << 2);
	    	Xil_Out32(XPAR_IO_PERIPH_BASEADDR, output);

	   		u32 input = Xil_In32(XPAR_IO_PERIPH_BASEADDR);
	   		bool in_btn_0 = input & 0x4;
	   		bool i_sig;

	   		//zastita za rafal
	   		while(in_btn_0 == 0x0){
	   			duck_move();

	   			output = ((u32)in_btn_0 <<1);
	   			Xil_Out32(XPAR_IO_PERIPH_BASEADDR, output);
	   			input = Xil_In32(XPAR_IO_PERIPH_BASEADDR);
	   			in_btn_0 = input & 0x4;

	   		}
	   		//stoji u while-u dok se ne pritisne okidac
	   		while(in_btn_0 == 0x1){
	   			duck_move();

	   			output = ((u32)in_btn_0 << 1);
	   			Xil_Out32(XPAR_IO_PERIPH_BASEADDR, output);
	   			input = Xil_In32(XPAR_IO_PERIPH_BASEADDR);
	   			in_btn_0 = input & 0x4;
	   		}

	   		map_update_shot();
	   		output = ((u32)in_btn_0 <<1);
			Xil_Out32(XPAR_IO_PERIPH_BASEADDR, output);
			input = Xil_In32(XPAR_IO_PERIPH_BASEADDR);
			in_btn_0 = input & 0x4;

	   		//postavljanje o_pwr na 1 (citanje sa senzora)
	   		o_pwr = 1;

	   		output = ((u32)o_pwr << 2);

	   		Xil_Out32(XPAR_IO_PERIPH_BASEADDR, output);
	   		int i;
	   		for(i = 0; i < 1000000;i++);
	   		input = Xil_In32(XPAR_IO_PERIPH_BASEADDR);
	   		i_sig = input & 0x4;


	   		if(!i_sig){
	   			printf("belo\n");
	   		}

	   		output = (((u32)i_sig << 1) | ((u32)o_pwr));
	   		Xil_Out32(XPAR_IO_PERIPH_BASEADDR, output);

	   		return i_sig;
}


void battle_city() {

	unsigned int buttons, tmpBtn = 0, tmpUp = 0;
	int i, change = 0, jumpFlag = 0;
	int block;
	//lives = 3;
	broj_promasaja = 0;

	map_reset(map1);
	map_update();

	//chhar_spawn(&cursor);
	chhar_spawn_duck(&duck1_right, &duck2_right, &duck3_right, &duck4_right);

	while (1) {
		stoljpi();
		map_update();
	}
}

