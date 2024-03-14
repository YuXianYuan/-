/**************************************************************
 *  ���ƣ�������ħ��������������ļ�
 *  ���ܣ� ����c2solver�Ĺ��ܣ����ɱ���ļ�
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
    // ���ɱ���ļ���������D�̸�Ŀ¼D:\\tables.h
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
 * cube_str[U4] = B,����U4λ���ϵ���ɫ��B
 */
    //�������10����ҵ�ħ��
    u8 cube_str[24]= {U,R,L,B,D,F,U,F,F,R,L,R,B,B,D,R,F,D,L,U,D,L,U,B};
    u8 moves[GOD_NUM];
    u8 length;
    for(int i=0; i<10; i++ )
    {
        printf("---------------------------------start\n");
        random_cubestr( cube_str );                          // �������һ�����ҵ�ħ��
        printf_cubestr( cube_str );                          // ��ӡ
        u8 ret = c2solver( cube_str,  moves,  &length );     // ���
        printf("slover return:%d\n",ret);

        printf_moves( moves, length );                       // ��ӡ������
        moves_apply_cube_str( cube_str, moves, length);      // ʹ���������ԭ���ҵ�ħ��
        printf_cubestr( cube_str );                          // ��ӡ��ԭ���ħ��
        CUBE cube = cubestr2cube( cube_str );                // ת����cube��ʽ
        printf_cube( cube );                                 // ��ӡcube��ʽ��ħ��

        printf("..\n..\n..\n");
    }

    system("pause");
    return 0;
}


