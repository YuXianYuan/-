
/**************************************************************
 *  名称：【二阶魔方求解器】工具函数
 *  备注：此文件不是求解器所必须的, 在移植时可以删除
 *  
 *  2024-03-13, yxy
 * ***********************************************************/
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "c2solver.h"
#include "utils.h"

/************************内部函数声明**************************/
void cubestr_move(u8* cube_str, u8 m);
void printf_cubestr(u8* cube_str);
void cube2cubestr(CUBE cube, u8* cube_str);
void printf_cube(CUBE cube);
void cube_move(CUBE* cube, u8 m);
CUBE code2cube( u16 pos_code, u16 dir_code );
CUBE random_cube();
void random_cubestr(u8* cube_str);

/****************************************************
 * 函数功能：转动cube_str格式的魔方
 * 
 *         | U1 U2 |
 *         | U3 U4 |
 * | L1 L2 | F1 F2 | R1 R2 | B1 B2 |
 * | L3 L4 | F3 F4 | R3 R4 | B3 B4 |
 *         | D1 D2 |
 *         | D3 D4 |
 * 
 * cube_str : {U1,U2,U3,U4,R1,R2,R3,R4,F1,F2,F3,F4,D1,D2,D3,D4,L1,L2,L3,L4,B1,B2,B3,B4}
 * 
 * **************************************************/
void cubestr_move(u8* cube_str, u8 m)
{
    u8 temp;
    if(m == mU1)
    {
        temp = cube_str[U1];
        cube_str[U1] = cube_str[U3];
        cube_str[U3] = cube_str[U4];
        cube_str[U4] = cube_str[U2];
        cube_str[U2] = temp;

        temp = cube_str[F1];
        cube_str[F1] = cube_str[R1];
        cube_str[R1] = cube_str[B1];
        cube_str[B1] = cube_str[L1];
        cube_str[L1] = temp;

        temp = cube_str[F2];
        cube_str[F2] = cube_str[R2];
        cube_str[R2] = cube_str[B2];
        cube_str[B2] = cube_str[L2];
        cube_str[L2] = temp;
    }
    if(m == mU2)
    {
        cubestr_move(cube_str, mU1);
        cubestr_move(cube_str, mU1);
    }
    if(m == mU3)
    {
        cubestr_move(cube_str, mU1);
        cubestr_move(cube_str, mU1);
        cubestr_move(cube_str, mU1);
    }

    if(m == mR1)
    {
        temp = cube_str[R1];
        cube_str[R1] = cube_str[R3];
        cube_str[R3] = cube_str[R4];
        cube_str[R4] = cube_str[R2];
        cube_str[R2] = temp;

        temp = cube_str[F2];
        cube_str[F2] = cube_str[D2];
        cube_str[D2] = cube_str[B3];
        cube_str[B3] = cube_str[U2];
        cube_str[U2] = temp;

        temp = cube_str[F4];
        cube_str[F4] = cube_str[D4];
        cube_str[D4] = cube_str[B1];
        cube_str[B1] = cube_str[U4];
        cube_str[U4] = temp;
    }
    if(m == mR2)
    {
        cubestr_move(cube_str, mR1);
        cubestr_move(cube_str, mR1);
    }
    if(m == mR3)
    {
        cubestr_move(cube_str, mR1);
        cubestr_move(cube_str, mR1);
        cubestr_move(cube_str, mR1);
    }

    if(m == mF1)
    {
        temp = cube_str[F1];
        cube_str[F1] = cube_str[F3];
        cube_str[F3] = cube_str[F4];
        cube_str[F4] = cube_str[F2];
        cube_str[F2] = temp;

        temp = cube_str[R1];
        cube_str[R1] = cube_str[U3];
        cube_str[U3] = cube_str[L4];
        cube_str[L4] = cube_str[D2];
        cube_str[D2] = temp;

        temp = cube_str[R3];
        cube_str[R3] = cube_str[U4];
        cube_str[U4] = cube_str[L2];
        cube_str[L2] = cube_str[D1];
        cube_str[D1] = temp;
    }
    if(m == mF2)
    {
        cubestr_move(cube_str, mF1);
        cubestr_move(cube_str, mF1);
    }
    if(m == mF3)
    {
        cubestr_move(cube_str, mF1);
        cubestr_move(cube_str, mF1);
        cubestr_move(cube_str, mF1);
    }
}
/****************************************************
 * 函数功能：转动cube格式的魔方
 * 
 * **************************************************/
void cube_move(CUBE* cube, u8 m)
{
    u8 cube_str[24];
    cube2cubestr( *cube, cube_str );
    cubestr_move( cube_str, m );
    *cube = cubestr2cube( cube_str ); 
}

/****************************************************
 * 函数功能：生成表格
 * 备    注：表格存放在D盘根目录
 * 
 * **************************************************/
void table_generate()
{
    int i,j;
    FILE *fp = NULL;

    unsigned short ram_pos_move_table[POS_NUM][MOVE_NUM];
    unsigned short ram_dir_move_table[DIR_NUM][MOVE_NUM];
    unsigned char  ram_pos_prun_table[POS_NUM];
    unsigned char  ram_dir_prun_table[DIR_NUM];
    
    fp = fopen("D:\\tables.h", "w");

    fprintf(fp, "#ifndef _TABLES_H_\n");
    fprintf(fp, "#define _TABLES_H_\n\n");
    fprintf(fp, "//此文件由程序自动生成, 包含4个数组, 需要109611byte的flash空间。\n"); 
    
    fprintf(fp, "#define   POS_NUM   5040     // 6*6! + 5*5! + 4*4! + 3*3! + 2*2! + 1*1! + 1\n");
    fprintf(fp, "#define   DIR_NUM   729      // 3^6\n");
    fprintf(fp, "#define   MOVE_NUM  9        // {mU1,mU2,mU3,mR1,mR2,mR3,mF1,mF2,mF3}\n");
    fprintf(fp,"\n\n");

    //1. ===================================================== pos_move_table
    printf("生成pos_move_table......\n");
    for( i=0; i<POS_NUM; i++ )
    {
        for( j=0; j<MOVE_NUM; j++ )
        {
            CUBE cube = code2cube( i, 0 );            // 逆编码
            cube_move( &cube, j );                    // 在cube坐标下拧动
            CODE code = cube2code( cube );            // 编码
            ram_pos_move_table[i][j] = code.pos_code; // 赋值
        }
    }
    printf("将pos_move_table写入文件......\n");
    fprintf(fp, "static const unsigned short pos_move_table[POS_NUM][MOVE_NUM] = {\n");
    for( i=0; i<POS_NUM; i++ )
    {
        fprintf(fp,"{");
        for( j=0; j<MOVE_NUM; j++ )
        {
            fprintf(fp, "%4d", ram_pos_move_table[i][j]);
            if(j != MOVE_NUM-1 )  fprintf(fp,",");
        }
        fprintf(fp,"}");
        if(i != POS_NUM-1 )  fprintf(fp,",");
        if(i%3 == 2)         fprintf(fp,"\n");
    }
    fprintf(fp,"};\n\n\n");

    //2. ===================================================== dir_move_table
    printf("生成dir_move_table......\n");
    for( i=0; i<DIR_NUM; i++ )
    {
        for( j=0; j<MOVE_NUM; j++ )
        {
            CUBE cube = code2cube( 0, i );            // 逆编码
            cube_move( &cube, j );                    // 在cube坐标下拧动
            CODE code = cube2code( cube );            // 编码
            ram_dir_move_table[i][j] = code.dir_code; // 赋值
        }
    }
    printf("将dir_move_table写入文件......\n");
    fprintf(fp, "static const unsigned short dir_move_table[DIR_NUM][MOVE_NUM] = {\n");
    for( i=0; i<DIR_NUM; i++ )
    {
        fprintf(fp,"{");
        for( j=0; j<MOVE_NUM; j++ )
        {
            fprintf(fp, "%4d", ram_dir_move_table[i][j]);
            if(j != MOVE_NUM-1 )  fprintf(fp,",");
        }
        fprintf(fp,"}");
        if(i != DIR_NUM-1 )  fprintf(fp,",");
        if(i%3 == 2)         fprintf(fp,"\n");
    }
    fprintf(fp,"};\n\n\n");

    //3. ===================================================== pos_prun_table
    printf("生成pos_prun_table......\n");
    for( i=0; i<POS_NUM; i++ )  ram_pos_prun_table[i] = 255;

    ram_pos_prun_table[0] = 0;   //目标状态所需步数为0
    int done = 1;
    int depth = 0;           //父节点所需步数
    while( done < POS_NUM )
    {
        for( i=0; i<POS_NUM; i++ )
        {
            if( ram_pos_prun_table[i] == depth )
            {
                for( j=0; j<MOVE_NUM; j++ )
                {
                    int child_pos_code = ram_pos_move_table[i][j];
                    if( ram_pos_prun_table[child_pos_code] == 255 ) //未赋值
                    {
                        ram_pos_prun_table[child_pos_code] = depth + 1;
                        done++;
                    }
                }
            }
        }
        depth++;
    }

    printf("将pos_prun_table写入文件......\n");
    fprintf(fp, "static const unsigned char pos_prun_table[POS_NUM]= {\n");
    for( i=0; i<POS_NUM; i++ )
    {
        fprintf( fp, "%2d", ram_pos_prun_table[i] );
        if(i != POS_NUM-1 )  
        {
            fprintf(fp,",");
        }
        if(i%45 == 44)
        {
            fprintf(fp,"\n");
        }
    }
    fprintf(fp,"};\n\n\n");

    //4. ===================================================== dir_prun_table
    printf("生成dir_prun_table......\n");
    for( i=0; i<DIR_NUM; i++ )  ram_dir_prun_table[i] = 255;

    ram_dir_prun_table[0] = 0;   //目标状态所需步数为0
    done = 1;
    depth = 0;           //父节点所需步数
    while( done < DIR_NUM )
    {
        for( i=0; i<DIR_NUM; i++ )
        {
            if( ram_dir_prun_table[i] == depth )
            {
                for( j=0; j<MOVE_NUM; j++ )
                {
                    int child_pos_code = ram_dir_move_table[i][j];
                    if( ram_dir_prun_table[child_pos_code] == 255 ) //未赋值
                    {
                        ram_dir_prun_table[child_pos_code] = depth + 1;
                        done++;
                    }
                }
            }
        }
        depth++;
    }

    printf("将dir_prun_table写入文件......\n");
    fprintf(fp, "static const unsigned char dir_prun_table[DIR_NUM]= {\n");
    for( i=0; i<DIR_NUM; i++ )
    {
        fprintf( fp, "%2d", ram_dir_prun_table[i] );
        if(i != DIR_NUM-1 )
        {
            fprintf(fp,",");
        }
        if(i%27 == 26)
        {
            fprintf(fp,"\n");
        }
    }
    fprintf(fp,"};\n\n\n");

    fprintf(fp, "#endif\n\n\n");
    fclose(fp);
    printf("文件写入完成, 保存在【D:\\tables.h】文件夹。\n");
}


/****************************************************
 * 函数功能：CUBE格式的魔方转换成cube_str格式
 * 
 * **************************************************/
void cube2cubestr(CUBE cube, u8* cube_str)
{
    int i,j;
    static u8 corner_color[8][3] = {
        { U, R, F }, { U, B, R }, { U, F, L }, { U, L, B },
        { D, F, R }, { D, R, B }, { D, L, F }, { D, B, L }
    };

    static u8 facelet[24] = {
         U4, R1, F2 ,  U2, B1, R2 ,  U3, F1, L2 ,  U1, L1, B2 ,
         D2, F4, R3 ,  D4, R4, B3 ,  D1, L4, F3 ,  D3, B4, L3 
    };

    //逐个位置进行处理
    for( i=0; i<8; i++)
    {
        u8 cor = cube.pos[i];  //corner
        u8 dir = cube.dir[i];
        for( j=0; j<3; j++)
        {
            
            u8 color = corner_color[ cor ][ (j+3-dir)%3 ];  // 第i个位置的第j个色片的颜色
            u8 cube_idx = i*3 + j;                          // cube格式下的索引
            u8 str_idx  = facelet[ cube_idx ];              // cube_str格式下的索引
            
            cube_str[ str_idx ] = color;
        }
    }
}

/****************************************************
 * 函数功能：随机生成一个cube_str格式的魔方
 * 
 * **************************************************/
void random_cubestr(u8* cube_str)
{
    CUBE cube = random_cube();
    cube2cubestr( cube, cube_str );
}
/****************************************************
 * 函数功能：随机生成一个CUBE格式的魔方
 * 
 * **************************************************/
CUBE random_cube()
{
    int i;
    CUBE cube;
    //生成一个复原的魔方
    for( i=0; i<8; i++ )
    {
        cube.dir[i] = 0;
        cube.pos[i] = i;
    }

    //随机打乱
    //srand((unsigned)time(NULL));
    for( i=0; i<GOD_NUM*2; i++ )
    {
        u8 m = rand()%MOVE_NUM;
        cube_move( &cube, m );
    }
    return cube;
}

/****************************************************
 * 函数功能：打印一个cube_str格式的魔方到控制台
 * 
 * **************************************************/
void printf_cubestr(u8* cube_str)
{
    int i;
    printf("cube_str: ");
    for( i=0; i<24; i++ )
    {
        printf( "%c", "URFDLB"[ cube_str[i] ] );
        if(i!=23) printf(",");
    }
    printf("\n");
}

/****************************************************
 * 函数功能：打印一个cube格式的魔方到控制台
 * 
 * **************************************************/
void printf_cube(CUBE cube)
{
    int i;
    printf("cube.pos:");
    for( i=0; i<8; i++ )
    {
        printf( "%d",cube.pos[i] );
    }

    printf("  cube.dir:");
    for( i=0; i<8; i++ )
    {
        printf( "%d",cube.dir[i] );
    }
    printf("\n");
}

/****************************************************
 * 函数功能：打印一串moves
 * 
 * **************************************************/
void printf_moves( u8* moves, u8 move_length )
{
    int i;
    printf("moves: ");
    for( i=0; i<move_length; i++ )
    {
        u8 f = moves[i] / 3;
        u8 d = moves[i] % 3;
        printf("%c%c ","URF"[f],"123"[d] );
    }
    printf(" (%d)\n",move_length);
}

/****************************************************
 * 函数功能：逆编码
 *
 * **************************************************/
CUBE code2cube( u16 pos_code, u16 dir_code )
{
    int i,j,k;
    CUBE cube;

    //1. 对方向逆编码
    u8 dir_sum = 0;
    for( i=5; i>=0; i-- )
    {
        cube.dir[i] = dir_code%3;
        dir_code = dir_code / 3;

        dir_sum +=  cube.dir[i];
    }
    cube.dir[6] = (3 - dir_sum%3)%3;
    cube.dir[7] = 0;

    //2. 对位置逆编码：逆康托展开
    static const u16 FAC[] = {1, 1, 2, 6, 24, 120, 720};	// 阶乘
    u8 used[7] = {0,0,0,0,0,0,0}; //0,1,2,3,4,5,6
    u8 unused_rank[7] = {0,1,2,3,4,5,6};
    for ( i=0; i<7; i++ )
    {
        u16 t = pos_code / FAC[6-i];
        // 对未使用的数字排序
        k = 0;
        for( j=0; j<7; j++ )
        {
            if( used[j] == 0 )
            {
                unused_rank[k] = j;
                k++;
            }
        }
        //对未使用的数字中，从小到大第t个
        cube.pos[i] = unused_rank[t];
        used[ unused_rank[t] ] = 1;

        pos_code = pos_code % FAC[6-i];
    }

    cube.pos[7] = DBL;

    return cube;
}

/****************************************************
 * 函数功能：将moves作用到cube上
 * 
 * **************************************************/
void moves_apply_cube( CUBE* cube, u8* moves, u8 length )
{
    int i;
    for( i=0; i<length; i++)
    {
        cube_move( cube, moves[i] );
    }
}

/****************************************************
 * 函数功能：将moves作用到cube_str上
 * 
 * **************************************************/
void moves_apply_cube_str( u8* cube_str, u8* moves, u8 length )
{
    int i;
    for( i=0; i<length; i++)
    {
        cubestr_move( cube_str, moves[i] );
    }
}


