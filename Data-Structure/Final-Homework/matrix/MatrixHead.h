#ifndef _MATRIXHEAD_H_
#define _MATRIXHEAD_H_


#include <stdio.h>
#include <stdlib.h>


//文件输入输出
FILE *fin,*fout;

//函数是否正常
#define OK 1
#define ERROR 0

//错误情况
#define ADD_ERROR  1
#define SUB_ERROR  2
#define MULT_ERROR 3


//矩阵元素类型
typedef int ElemType;


#define MAXSIZE 400 //稀疏矩阵非零元的最大个数
#define MAXRC 20    //稀疏矩阵的最大行数
typedef struct Triple{
    int i,j;    //该非零元的行下标和列下标
    ElemType e; //该非零元的值
}Triple;
typedef struct{
    Triple data[MAXSIZE+1]; //非零元三元组表
    int rpos[MAXRC];        //各行第一个非零元的位置表
    int mu,nu,tu;           //矩阵的行数、列数和非零元个数
}RLSMatrix;


//定义从文件中读入一行计算式
#define MAXLINE 10000
int mygetline(char *str, int lim);


//定义稀疏矩阵操作函数
//初始化
int InitRLSMatrix(RLSMatrix *M);
//矩阵输入处理
int GetRLSMatrix(RLSMatrix *M, char *line, int start);
//矩阵输出处理
int PrintRLSMatrix(RLSMatrix M);
//矩阵运算+-*
RLSMatrix addMatrix(RLSMatrix M, RLSMatrix N, int *error);
RLSMatrix subMatrix(RLSMatrix M, RLSMatrix N, int *error);
RLSMatrix multiplyMatrix(RLSMatrix M, RLSMatrix N, int *error);


#endif

