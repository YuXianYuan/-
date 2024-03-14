
/**************************************************************
 *  ���ƣ�������ħ���������
 *  ���ܣ� ����������ħ��
 *  
 *  2024-03-13, yxy
 * ***********************************************************/
#include "stdio.h"
#include "c2solver.h"
#include "tables.h"

/************************�ڲ���������**************************/
CUBE cubestr2cube(u8* cube_in);
u8 valid_check(CUBE cube);
CODE cube2code( CUBE cube );
void IDA_search( u16 pos_code, u16 dir_code);

/************************ȫ�ֱ�������**************************/
u8 g_solved = 0;
u8 *g_move_out;
u8 g_move_length = 0;
u8 g_max_length = 0;

/************************�ڲ�����ʵ��**************************/
/**
 * �������ܣ����һ������ħ��
 * 
 * ������˳����ħ����
 *         | U1 U2 |
 *         | U3 U4 |
 * | L1 L2 | F1 F2 | R1 R2 | B1 B2 |
 * | L3 L4 | F3 F4 | R3 R4 | B3 B4 |
 *         | D1 D2 |
 *         | D3 D4 |
 * 
 * cube_str    : {U1,U2,U3,U4,R1,R2,R3,R4,F1,F2,F3,F4,D1,D2,D3,D4,L1,L2,L3,L4,B1,B2,B3,B4}
 * move_out    : ��������
 * move_length : ������ĳ���
 * 
 * ��ע1������DBL�ǿ鱻�̶�ס�����D3��B4��L3����ɫ����ΪD��B��L
 * ��ע2��move_out�ڱ��ļ��н���һ��ָ�룬��Ҫ�ڵ���ǰ��������顣
 */

u8 c2solver(u8* cube_str, u8* move_out, u8* move_length)
{
    //1. ================================================== �������ħ���ĺ�����
    CUBE cube = cubestr2cube(cube_str);
    u8 ret = valid_check(cube);
    if(ret != VALID) return ret;

    //2. ================================================== ħ���ṹ���±���
    CODE code = cube2code( cube );

    //3. ================================================== ���ħ��
    g_solved = 0;
    g_move_out = move_out;
    
    for( g_max_length=0; g_max_length<=GOD_NUM; g_max_length++ ) //������������
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
 * �������ܣ�ʹ��IDA*�㷨 ���ħ��
 * 
 *
 * **************************************************/
void IDA_search( u16 pos_code, u16 dir_code )
{
    if(pos_code == 0 && dir_code == 0) //ħ���Ѿ���ԭ
    {
        g_solved = 1;
        return;
    }
    else
    {
        u8 m;
        for( m=mU1; m<=mF3; m++ )  //9���˶�ָ��
        {
            if( g_move_length>0 )
            {
                //��֦�ظ�����������һ��������U1������һ������������U2��U3
                if( (g_move_out[g_move_length-1])/3 == m/3 ) continue;
            }

            u16 new_pos_code = pos_move_table[pos_code][m];
            u16 new_dir_code = dir_move_table[dir_code][m];

            u8 pos_distance = pos_prun_table[ new_pos_code ];
            u8 dir_distance = dir_prun_table[ new_dir_code ];
            
            u8 dist = MAX( pos_distance, dir_distance );

            // ʹ�ü�֦����м�֦��
            // g_move_length+1   :  ת�����new_code����Ĳ���
            // dist              :  ��ԭ���new_code������Ҫ���ٲ�
            // g_max_length      :  ���ֵ�����������������
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
 * �������ܣ���ħ�����±���
 *
 * **************************************************/
CODE cube2code( CUBE cube )
{
    int i,j;
    CODE code;

    //1. �Է������±��룺
    // ÿ���ǿ�ķ���ֻ��3�ֿ��ܣ���˷������±���������������תʮ����
    // DBL���Ѿ�ȷ��Ϊ0, ǰ6���ǿ����󣬵�7��Ҳ��ȷ����,���ֻ����ǰ6���ǿ�
    u16 dir_code = 0;
    for( i=0; i<6; i++ )
    {
        dir_code = 3 * dir_code + cube.dir[i];
    }
    code.dir_code = dir_code;

    //2. ��λ�����±��룺����չ��
    static const u16 FAC[] = {1, 1, 2, 6, 24, 120, 720};	// �׳�
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
 * �������ܣ����������һ��cube_str, ����CUBE��ʽ���㷨�ڲ�ħ����Ϣ�洢�ṹ
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
        
    //����pos[8]
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
        cube.pos[i] = k; // k��Ϊ8,���쳣,����valid_check���ж���
    }

    //����dir[8]
    for( i=0; i<8; i++ )
    {
        for( j=0; j<3; j++ )
        {
            if ( cube_str[ facelet[i][j] ] == U || cube_str[ facelet[i][j] ] == D )
            {
                break;
            }
        }
        cube.dir[i] = j; // k��Ϊ3,���쳣,����valid_check���ж���
    }

    return cube;
}

/****************************************************
 * �������ܣ��������ħ������Ч��
 * 
 *  #define   DBL_POS_ERR       1   // ����ħ��DLB�ǿ��λ�ò�����
 *  #define   DBL_DIR_ERR       2   // ����ħ��DLB�ǿ�ķ��򲻺���
 *  #define   CUBE_DIR_ERR      3   // ����ħ���Ľǿ鳯�򲻺���
 *  #define   MISS_CORNER_ERR   4   // ��ʧĳ���ǿ�
 *  #define   OTHER_ERR         5   // ��������
 * 
 * **************************************************/
u8 valid_check(CUBE cube)
{
    int i;
    //1. ����DLB�ǿ鱻�̶�ס�����DLBλ�õĽǿ����ΪDLB,�ҳ���Ϊ0
    if( cube.pos[DBL] != DBL) return DBL_POS_ERR;
    if( cube.dir[DBL] != 0  ) return DBL_DIR_ERR;

    //�ǿ鷽��֮��ӦΪ3�ı���
    int dir_sum = 0;
    for( i=0; i<8; i++ )
    {
        dir_sum += cube.dir[i];
    }
    if( dir_sum%3 != 0 ) return CUBE_DIR_ERR;

    //ÿ���ǿ������
    u8 mask = 0;
    for( i=0; i<8; i++ )
    {
        mask |= ( 1 << cube.pos[i] );
    }
    if( mask != 0xff ) return MISS_CORNER_ERR;

    //������һЩ����
    for( i=0; i<8; i++ )
    {
       if( cube.pos[i] > DBL ) return OTHER_ERR;
       if( cube.dir[i] > 3   ) return OTHER_ERR;
    }

    //TODO:ʵ��������Ч�Լ��
    return VALID;
}



