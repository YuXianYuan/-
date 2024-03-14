
#ifndef _UTILS_H_
#define _UTILS_H_

#include "c2solver.h"


#ifndef POS_NUM
#define POS_NUM    5040     // 6*6! + 5*5! + 4*4! + 3*3! + 2*2! + 1*1! + 1
#endif

#ifndef DIR_NUM
#define DIR_NUM    729      // 3^6
#endif

#ifndef MOVE_NUM
#define MOVE_NUM   9        // {mU1,mU2,mU3,mR1,mR2,mR3,mF1,mF2,mF3}
#endif
     
//对外函数声明
extern void cube2cubestr(CUBE cube, u8* cube_str);
extern void printf_cubestr(u8* cube_str);
extern void printf_cube(CUBE cube);

extern void cube_move(CUBE* cube, u8 m);
extern CUBE code2cube( u16 pos_code, u16 dir_code );
extern void table_generate();
extern void random_cubestr(u8* cube_str);
extern void printf_moves( u8* moves, u8 move_length );
extern void moves_apply_cube( CUBE* cube, u8* moves, u8 length );
extern void moves_apply_cube_str( u8* cube_str, u8* moves, u8 length );

#endif