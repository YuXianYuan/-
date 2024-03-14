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
## - 测试输出 test out
```C
---------------------------------start
cube_str: F,U,B,R,U,B,D,F,D,B,F,F,L,L,D,D,U,R,L,U,L,R,R,B
slover return:0
moves: R3 F3 R2 U3 F1 R3 F2 R3 U3  (9)
cube_str: U,U,U,U,R,R,R,R,F,F,F,F,D,D,D,D,L,L,L,L,B,B,B,B
cube.pos:01234567  cube.dir:00000000
..
..
..
---------------------------------start
cube_str: R,U,D,L,B,L,R,L,F,U,U,B,R,U,D,D,B,R,L,F,F,D,F,B
slover return:0
moves: R3 U2 R3 F3 U3 F3 R1 F2 U1  (9)
cube_str: U,U,U,U,R,R,R,R,F,F,F,F,D,D,D,D,L,L,L,L,B,B,B,B
cube.pos:01234567  cube.dir:00000000
..
..
..
---------------------------------start
cube_str: R,U,L,F,U,B,B,L,F,R,R,R,D,D,D,F,U,D,L,F,L,B,U,B
slover return:0
moves: R1 F2 U1 F2 R3 F3 U3 R1  (8)
cube_str: U,U,U,U,R,R,R,R,F,F,F,F,D,D,D,D,L,L,L,L,B,B,B,B
cube.pos:01234567  cube.dir:00000000
..
..
..
---------------------------------start
cube_str: U,R,U,F,R,B,L,F,L,D,B,D,D,F,D,R,F,B,L,R,U,L,U,B
slover return:0
moves: R1 F2 R2 U3  (4)
cube_str: U,U,U,U,R,R,R,R,F,F,F,F,D,D,D,D,L,L,L,L,B,B,B,B
cube.pos:01234567  cube.dir:00000000
..
..
..
---------------------------------start
cube_str: F,U,B,F,U,R,L,B,U,R,F,F,D,U,D,R,R,L,L,L,B,D,D,B
slover return:0
moves: R3 U1 R3 U2 R3 F3 U2 F1  (8)
cube_str: U,U,U,U,R,R,R,R,F,F,F,F,D,D,D,D,L,L,L,L,B,B,B,B
cube.pos:01234567  cube.dir:00000000
..
..
..
---------------------------------start
cube_str: L,U,F,D,L,B,U,F,R,F,D,R,R,B,D,R,U,D,L,B,L,F,U,B
slover return:0
moves: U3 R1 F2 U3 R2 U1 F3 R2  (8)
cube_str: U,U,U,U,R,R,R,R,F,F,F,F,D,D,D,D,L,L,L,L,B,B,B,B
cube.pos:01234567  cube.dir:00000000
..
..
..
---------------------------------start
cube_str: B,F,D,L,F,R,F,D,R,D,R,U,U,L,D,R,U,B,L,B,U,L,F,B
slover return:0
moves: U1 R1 U1 R3 U1 F1 R2 F1 U3  (9)
cube_str: U,U,U,U,R,R,R,R,F,F,F,F,D,D,D,D,L,L,L,L,B,B,B,B
cube.pos:01234567  cube.dir:00000000
..
..
..
---------------------------------start
cube_str: U,R,L,B,D,F,U,F,F,R,L,R,B,B,D,R,F,D,L,U,D,L,U,B
slover return:0
moves: R1 U1 R2 F1 R3 U1 R2 F1  (8)
cube_str: U,U,U,U,R,R,R,R,F,F,F,F,D,D,D,D,L,L,L,L,B,B,B,B
cube.pos:01234567  cube.dir:00000000
..
..
..
---------------------------------start
cube_str: L,B,R,F,R,L,F,R,B,D,R,L,U,D,D,U,U,D,L,B,U,F,F,B
slover return:0
moves: U3 F2 R2 F1 R1 U2 F3 R2 U1 R3  (10)
cube_str: U,U,U,U,R,R,R,R,F,F,F,F,D,D,D,D,L,L,L,L,B,B,B,B
cube.pos:01234567  cube.dir:00000000
..
..
..
---------------------------------start
cube_str: F,U,D,F,U,R,R,R,L,R,B,D,U,B,D,F,L,F,L,L,B,U,D,B
slover return:0
moves: R3 F3 U3 F1 U3 F2 U2  (7)
cube_str: U,U,U,U,R,R,R,R,F,F,F,F,D,D,D,D,L,L,L,L,B,B,B,B
cube.pos:01234567  cube.dir:00000000
```

