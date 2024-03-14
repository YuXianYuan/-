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

//������ɫ
#define     U     0
#define     R     1
#define     F     2
#define     D     3
#define     L     4
#define     B     5

//�����ƶ�(�̶�סDBL�ǿ飬��˲�����D�㡢L�㡢B���ת��)
//β׺1Ϊ˳ʱ��90�ȣ�β׺3Ϊ��ʱ��0�ȣ�β׺2Ϊ180��
enum {mU1,mU2,mU3,mR1,mR2,mR3,mF1,mF2,mF3};

typedef enum {URF,UBR,UFL,ULB,DFR,DRB,DLF,DBL} POS;
enum {U1,U2,U3,U4,R1,R2,R3,R4,F1,F2,F3,F4,D1,D2,D3,D4,L1,L2,L3,L4,B1,B2,B3,B4};

//���庯������
#define   SOLVE_OK          0   // �ɹ����
#define   DBL_POS_ERR       1   // ����ħ��DLB�ǿ��λ�ò�����
#define   DBL_DIR_ERR       2   // ����ħ��DLB�ǿ�ķ��򲻺���
#define   CUBE_DIR_ERR      3   // ����ħ���Ľǿ鳯�򲻺���
#define   MISS_CORNER_ERR   4   // ��ʧĳ���ǿ�
#define   OTHER_ERR         5   // ��������

#define   VALID             0

#define GOD_NUM             11  // ����ħ���ϵ�֮��Ϊ11

//�㷨�ڲ�ħ����Ϣ�洢�ṹ
typedef struct 
{
    POS pos[8];      // pos[URF] == ULF, �����ǿ�ULF��URFλ����
    u8  dir[8];      // pos[URF] == 0  , ����URFλ���ϵĽǿ�ĳ���Ϊ0
}CUBE;

//���±�����ħ���ṹ
typedef struct 
{
    u16 pos_code;    // λ�ñ���
    u16 dir_code;    // �������
}CODE;

//���⺯������
extern u8 c2solver(u8* cube_str, u8* move_out, u8* move_length);
extern CUBE cubestr2cube(u8* cube_str);
CODE cube2code( CUBE cube );
#endif