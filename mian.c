/**************************************************************
 *  名称：【二阶魔方求解器】测试文件
 *  功能： 测试c2solver的功能，生成表格文件
 *  
 *  2024-03-13, yxy
 * ***********************************************************/

#include "stdio.h"
#include "windows.h"
#include "c2solver.h"
#include "utils.h"
#include "tables.h"

int main()
{
    // 生成表格文件，保存在D盘根目录D:\\tables.h
    //table_generate();

    
/*
 *         | U1 U2 |
 *         | U3 U4 |
 * | L1 L2 | F1 F2 | R1 R2 | B1 B2 |
 * | L3 L4 | F3 F4 | R3 R4 | B3 B4 |
 *         | D1 D2 |
 *         | D3 D4 |
 * 
 * cube_str : {U1,U2,U3,U4,R1,R2,R3,R4,F1,F2,F3,F4,D1,D2,D3,D4,L1,L2,L3,L4,B1,B2,B3,B4}
 * cube_str[U4] = B,即：U4位置上的颜色是B
 */
    //随机生成10组打乱的魔方
    u8 cube_str[24]= {U,R,L,B,D,F,U,F,F,R,L,R,B,B,D,R,F,D,L,U,D,L,U,B};
    u8 moves[GOD_NUM];
    u8 length;
    for(int i=0; i<10; i++ )
    {
        printf("---------------------------------start\n");
        random_cubestr( cube_str );                          // 随机生成一个打乱的魔方
        printf_cubestr( cube_str );                          // 打印
        u8 ret = c2solver( cube_str,  moves,  &length );     // 求解
        printf("slover return:%d\n",ret);

        printf_moves( moves, length );                       // 打印结算结果
        moves_apply_cube_str( cube_str, moves, length);      // 使用求解结果复原打乱的魔方
        printf_cubestr( cube_str );                          // 打印复原后的魔方
        CUBE cube = cubestr2cube( cube_str );                // 转换成cube格式
        printf_cube( cube );                                 // 打印cube格式的魔方

        printf("..\n..\n..\n");
    }

    system("pause");
    return 0;
}


