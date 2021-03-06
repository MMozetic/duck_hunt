#include "xbasic_types.h"
#include <stdbool.h>

#ifndef BATTLE_CITY_H_
#define BATTLE_CITY_H_

#define MAP_WIDTH           40  // 40
#define MAP_HEIGHT          30 // 30

#define SCREEN_WIDTH		640
#define SCREEN_HEIGHT		480
#define VGA_PERIPH_MEM_mWriteMemory(Address, Data) \
 	Xil_Out32(Address, (Xuint32)(Data))
#define XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR 0x78000000

bool stoljpi();

void battle_city();

#endif /* BATTLE_CITY_H_ */
