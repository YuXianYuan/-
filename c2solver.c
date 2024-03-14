
/**************************************************************
 *  名称：【二阶魔方求解器】
 *  功能： 用于求解二阶魔方
 *  
 *  2024-03-13, yxy
 * ***********************************************************/
#include "stdio.h"
#include "c2solver.h"
#include "tables.h"

/************************内部函数声明**************************/
CUBE cubestr2cube(u8* cube_in);
u8 valid_check(CUBE cube);
CODE cube2code( CUBE cube );
void IDA_search( u16 pos_code, u16 dir_code);

/************************全局变量声明**************************/
u8 g_solved = 0;
u8 *g_move_out;
u8 g_move_length = 0;
u8 g_max_length = 0;

/************************内部函数实现**************************/
/**
 * 函数功能：求解一个二阶魔方
 * 
 * 以如下顺序定义魔方：
 *         | U1 U2 |
 *         | U3 U4 |
 * | L1 L2 | F1 F2 | R1 R2 | B1 B2 |
 * | L3 L4 | F3 F4 | R3 R4 | B3 B4 |
 *         | D1 D2 |
 *         | D3 D4 |
 * 
 * cube_str    : {U1,U2,U3,U4,R1,R2,R3,R4,F1,F2,F3,F4,D1,D2,D3,D4,L1,L2,L3,L4,B1,B2,B3,B4}
 * move_out    : 输出求解结果
 * move_length : 求解结果的长度
 * 
 * 备注1：由于DBL角块被固定住，因此D3、B4、L3的颜色必须为D、B、L
 * 备注2：move_out在本文件中仅是一个指针，需要在调用前申请成数组。
 */

u8 c2solver(u8* cube_str, u8* move_out, u8* move_length)
{
    //1. ================================================== 检查输入魔方的合理性
    CUBE cube = cubestr2cube(cube_str);
    u8 ret = valid_check(cube);
    if(ret != VALID) return ret;

    //2. ================================================== 魔方结构重新编码
    CODE code = cube2code( cube );

    //3. ================================================== 求解魔方
    g_solved = 0;
    g_move_out = move_out;
    
    for( g_max_length=0; g_max_length<=GOD_NUM; g_max_length++ ) //迭代加深搜索
    {
        g_move_length = 0;
        IDA_search( code.pos_code, code.dir_code );

        if(g_solved == 1)
        {
            *move_length = g_move_length;
            break;
        }
    }

    return SOLVE_OK;
}

/****************************************************
 * 函数功能：使用IDA*算法 求解魔方
 * 
 *
 * **************************************************/
void IDA_search( u16 pos_code, u16 dir_code )
{
    if(pos_code == 0 && dir_code == 0) //魔方已经复原
    {
        g_solved = 1;
        return;
    }
    else
    {
        u8 m;
        for( m=mU1; m<=mF3; m++ )  //9种运动指令
        {
            if( g_move_length>0 )
            {
                //减枝重复动作，如上一个动作是U1，则这一个动作就跳过U2和U3
                if( (g_move_out[g_move_length-1])/3 == m/3 ) continue;
            }

            u16 new_pos_code = pos_move_table[pos_code][m];
            u16 new_dir_code = dir_move_table[dir_code][m];

            u8 pos_distance = pos_prun_table[ new_pos_code ];
            u8 dir_distance = dir_prun_table[ new_dir_code ];
            
            u8 dist = MAX( pos_distance, dir_distance );

            // 使用减枝表进行减枝：
            // g_move_length+1   :  转到这个new_code所需的步数
            // dist              :  复原这个new_code至少需要多少步
            // g_max_length      :  本轮迭代加深允许的最大步数
            if( dist+g_move_length+1 > g_max_length ) continue;

            g_move_out[g_move_length] = m;
            g_move_length++;
            IDA_search( new_pos_code, new_dir_code );

            if( g_solved == 1)  break;
            else                g_move_length--;
        }
    }
}

/****************************************************
 * 函数功能：对魔方重新编码
 *
 * **************************************************/
CODE cube2code( CUBE cube )
{
    int i,j;
    CODE code;

    //1. 对方向重新编码：
    // 每个角块的方向只有3种可能，因此方向重新编码类似于三进制转十进制
    // DBL块已经确定为0, 前6个角块编码后，第7个也就确定了,因此只编码前6个角块
    u16 dir_code = 0;
    for( i=0; i<6; i++ )
    {
        dir_code = 3 * dir_code + cube.dir[i];
    }
    code.dir_code = dir_code;

    //2. 对位置重新编码：康托展开
    static const u16 FAC[] = {1, 1, 2, 6, 24, 120, 720};	// 阶乘
    u16 small_cnt[7] = {0,0,0,0,0,0,0};
    for( i=0; i<7; i++)
    {
        for( j=i+1; j<7; j++ )
        {
            if( cube.pos[j] < cube.pos[i] )
            {
                small_cnt[i]++;
            }
        }
    }

    u16 pos_code = 0;
    for( i=0; i<7; i++ )
    {
        pos_code += small_cnt[i]*FAC[6-i];
    }
    code.pos_code = pos_code;

    return code;
}

/****************************************************
 * 函数功能：根据输入的一串cube_str, 生成CUBE格式的算法内部魔方信息存储结构
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
CUBE cubestr2cube(u8* cube_str)
{
    int i,j,k;
    CUBE cube;

    static u8 facelet[8][3] = {
        { U4, R1, F2 }, { U2, B1, R2 }, { U3, F1, L2 }, { U1, L1, B2 },
        { D2, F4, R3 }, { D4, R4, B3 }, { D1, L4, F3 }, { D3, B4, L3 }
    };
    
    static u8 or2pos[8] = {
        (1<<U) | (1<<R) | (1<<F),
        (1<<U) | (1<<B) | (1<<R),
        (1<<U) | (1<<F) | (1<<L),
        (1<<U) | (1<<L) | (1<<B),
        (1<<D) | (1<<F) | (1<<R),
        (1<<D) | (1<<R) | (1<<B),
        (1<<D) | (1<<L) | (1<<F),
        (1<<D) | (1<<B) | (1<<L)
    };
        
    //计算pos[8]
    for( i=0; i<8; i++ )
    {
        int or = 0;
        for( j=0; j<3; j++ )
        {
            or |= ( 1 << cube_str[ facelet[i][j] ] );
        }
        for( k=0; k<8; k++)
        {
            if( or == or2pos[k] )
            {
                break;
            }
        }
        cube.pos[i] = k; // k若为8,则异常,可在valid_check中判定出
    }

    //计算dir[8]
    for( i=0; i<8; i++ )
    {
        for( j=0; j<3; j++ )
        {
            if ( cube_str[ facelet[i][j] ] == U || cube_str[ facelet[i][j] ] == D )
            {
                break;
            }
        }
        cube.dir[i] = j; // k若为3,则异常,可在valid_check中判定出
    }

    return cube;
}

/****************************************************
 * 函数功能：检查输入魔方的有效性
 * 
 *  #define   DBL_POS_ERR       1   // 输入魔方DLB角块的位置不合理
 *  #define   DBL_DIR_ERR       2   // 输入魔方DLB角块的方向不合理
 *  #define   CUBE_DIR_ERR      3   // 输入魔方的角块朝向不合理
 *  #define   MISS_CORNER_ERR   4   // 丢失某个角块
 *  #define   OTHER_ERR         5   // 其他错误
 * 
 * **************************************************/
u8 valid_check(CUBE cube)
{
    int i;
    //1. 由于DLB角块被固定住，因此DLB位置的角块必须为DLB,且朝向为0
    if( cube.pos[DBL] != DBL) return DBL_POS_ERR;
    if( cube.dir[DBL] != 0  ) return DBL_DIR_ERR;

    //角块方向之和应为3的倍数
    int dir_sum = 0;
    for( i=0; i<8; i++ )
    {
        dir_sum += cube.dir[i];
    }
    if( dir_sum%3 != 0 ) return CUBE_DIR_ERR;

    //每个角块均存在
    u8 mask = 0;
    for( i=0; i<8; i++ )
    {
        mask |= ( 1 << cube.pos[i] );
    }
    if( mask != 0xff ) return MISS_CORNER_ERR;

    //其他的一些错误
    for( i=0; i<8; i++ )
    {
       if( cube.pos[i] > DBL ) return OTHER_ERR;
       if( cube.dir[i] > 3   ) return OTHER_ERR;
    }

    //TODO:实现其他有效性检测
    return VALID;
}



