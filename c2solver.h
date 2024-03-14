#ifndef _C2SLOVER_H_
#define _C2SLOVER_H_

typedef unsigned char                  u8;
typedef   signed char                  s8;
typedef unsigned short                 u16;
typedef   signed short                 s16;
typedef unsigned long                  u32;
typedef   signed long                  s32;

/* MIN/MAX/ABS Macros */
#define MIN(a, b)                      (((a) < (b))?(a):(b))
#define MAX(a, b)                      (((a) > (b))?(a):(b))
#define ABS(x)                         (((x) > 0)?(x):(-(x)))

//定义颜色
#define     U     0
#define     R     1
#define     F     2
#define     D     3
#define     L     4
#define     B     5

//定义移动(固定住DBL角块，因此不考虑D层、L层、B层的转动)
//尾缀1为顺时针90度，尾缀3为逆时针0度，尾缀2为180度
enum {mU1,mU2,mU3,mR1,mR2,mR3,mF1,mF2,mF3};

typedef enum {URF,UBR,UFL,ULB,DFR,DRB,DLF,DBL} POS;
enum {U1,U2,U3,U4,R1,R2,R3,R4,F1,F2,F3,F4,D1,D2,D3,D4,L1,L2,L3,L4,B1,B2,B3,B4};

//定义函数返回
#define   SOLVE_OK          0   // 成功求解
#define   DBL_POS_ERR       1   // 输入魔方DLB角块的位置不合理
#define   DBL_DIR_ERR       2   // 输入魔方DLB角块的方向不合理
#define   CUBE_DIR_ERR      3   // 输入魔方的角块朝向不合理
#define   MISS_CORNER_ERR   4   // 丢失某个角块
#define   OTHER_ERR         5   // 其他错误

#define   VALID             0

#define GOD_NUM             11  // 二阶魔方上帝之数为11

//算法内部魔方信息存储结构
typedef struct 
{
    POS pos[8];      // pos[URF] == UFL, 表明角块UFL在URF位置上
    u8  dir[8];      // pos[URF] == 0  , 表明URF位置上的角块的朝向为0
}CUBE;

//重新编码后的魔方结构
typedef struct 
{
    u16 pos_code;    // 位置编码
    u16 dir_code;    // 方向编码
}CODE;

//对外函数声明
extern u8 c2solver(u8* cube_str, u8* move_out, u8* move_length);
extern CUBE cubestr2cube(u8* cube_str);
CODE cube2code( CUBE cube );
#endif
