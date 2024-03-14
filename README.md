## - 简介 introduction
使用迭代加深算法求解二阶魔方最优解<br />
IDA* is used to solve the second-order Rubik's cube optimal solution<br /><br />
纯C语言版本<br />
Pure C language version<br /><br />
需要大约110k的flash空间，在stm32f103(64Mhz)上运行平均时间150ms<br />
About 110k of flash space is required, and the average running time on the stm32f103(64Mhz) is 150ms<br />

## - 测试 test
```C
int main()
{
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
 * cube_str[U4] = B, that is, the color at position U4 is B

 * 备注1：由于DBL角块被固定住，因此D3、B4、L3的颜色必须为D、B、L。
 * Note 1: Since DBL corner blocks are fixed, the colors of D3, B4, and L3 must be D, B, and L
 * 备注2：move_out在求解器函数中仅是一个指针，需要在调用前申请成数组。
 * Note 2: move_out is just a pointer in the solver function and needs to be converted into an array before being called.
 */
    //随机生成10组打乱的魔方
   // Randomly generate 10 groups of scrambled cubes
    u8 cube_str[24]= {U,R,L,B,D,F,U,F,F,R,L,R,B,B,D,R,F,D,L,U,D,L,U,B};
    u8 moves[GOD_NUM];
    u8 length, i;
    for(i=0; i<10; i++ )
    {
        printf("---------------------------------start\n");
        random_cubestr( cube_str );                          // 随机生成一个打乱的魔方  Randomly generate a scrambled Rubik's cube
        printf_cubestr( cube_str );                          // 打印 printf
        u8 ret = c2solver( cube_str,  moves,  &length );     // 求解 slove
        printf("slover return:%d\n",ret);

        printf_moves( moves, length );                       // 打印解算结果   Print the result of the solution
        moves_apply_cube_str( cube_str, moves, length);      // 使用求解结果复原打乱的魔方 Use the solution results to restore the scrambled Rubik's cube
        printf_cubestr( cube_str );                          // 打印复原后的魔方  Print the restored Rubik's cube
        CUBE cube = cubestr2cube( cube_str );                // 转换成cube格式  Convert to cube format
        printf_cube( cube );                                 // 打印cube格式的魔方  Print the cube in cube format

        printf("..\n..\n..\n");
    }

    system("pause");
    return 0;
}
```
