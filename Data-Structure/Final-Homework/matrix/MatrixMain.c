#include "MatrixHead.h"
extern FILE *fin,*fout;

int main(void){
	fin = fopen("data.in","rb");
	fout = fopen("data.out","wb");

    int len, symposi, num;
    int error;
    char line[MAXLINE], symbol;
    RLSMatrix M,N,Q;

    num = 0;
    while((len=mygetline(line, MAXLINE))>2){//读入一行line，根据长度len判断是否是运算式
        num++;
        fprintf(fout, "第%d个矩阵计算式:\n", num); //输出计算式序号

        InitRLSMatrix(&M);      //初始化M和N
        InitRLSMatrix(&N);

        for(symposi=0; symposi<len; symposi++){ //判断运算符的位置symposi
            if(line[symposi]=='+' || line[symposi]=='-' || line[symposi]=='*')
                if(line[symposi-1]=='}' && line[symposi+1]=='{')   //判断不是负号
                    break;
        }
        symbol = line[symposi];     //得到运算符

        GetRLSMatrix(&M, line, 0);  //根据起始位置得到M和N，并输出
        PrintRLSMatrix(M);
        GetRLSMatrix(&N, line, symposi+1);
        PrintRLSMatrix(N);

        error = 0;
        switch(symbol){             //根据运算符调用函数并得到结果
            case '+': Q = addMatrix(M,N,&error); break;
            case '-': Q = subMatrix(M,N,&error); break;
            case '*': Q = multiplyMatrix(M,N,&error); break;
            default:  Q = addMatrix(M,N,&error); break;
        }

        if(error==0) PrintRLSMatrix(Q);     //判断运算能否进行，输出结果
        else fprintf(fout, "Error!\n");
        fprintf(fout, "\n");
    }

    fclose(fin);
	fclose(fout);
    return 0;
}
