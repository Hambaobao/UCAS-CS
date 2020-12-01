#include "MatrixHead.h"
extern FILE *fin,*fout;

//读入一行计算式
int mygetline(char *str, int lim)
{
    int len=0,c;
    while((--lim>0) && ((c=getc(fin))!=EOF) && (c!='\n'))
        str[len++]=c;
    if(c=='\n')
        str[len++]='\n';
    str[len]='\0';
    return len;
}


int GetRLSMatrix(RLSMatrix *M, char *line, int start)
{
//根据读入计算表达式line和主函数中确定的矩阵起始位置start
//得到稀疏矩阵M，并返回

    int k;  //k表示当前字符在line中的下标
    int i,j,e,mu,nu,tu;
    int ispositive; //ispositive判断值的符号
    int s;  //s表示当前得到数据在三元组()的位置
    int isrpos;
    char c;

    k = start;

    //得到矩阵的行数和列数
    mu = 0; k++;
    while((c=*(line+k)) != ';'){    //mu读入结束
        if(c>='0' && c<='9') { mu = mu*10+c-'0'; }
        k++;
    }
    nu = 0; k++;
    while((c=*(line+k)) != ';'){    //nu读入结束
        if(c>='0' && c<='9') { nu = nu*10+c-'0'; }
        k++;
    }

    // 得到矩阵非零元的个数，和稀疏矩阵的元素（行逻辑排列）
    tu = 0; k = k+2;
    while((c=*(line+k)) != ']'){    //矩阵读入结束
        if(c == '('){           //一个三元组读入开始，初始化
            tu++;
            i = j = e = 0;
            s = 1;
            ispositive = 1;
        }else if(c == '-'){     //读到值为负的元素
            ispositive = -1;
        }else if(c == ','){     //i和j读入
            if(s==1)      M->data[tu].i = i;
            else if(s==2) M->data[tu].j = j;
            s++;
        }else if(c == ')'){     //e读入
            M->data[tu].e = ispositive*e;
        }else{                  //计算i,j,e
            if(s==1)      i = i*10+c-'0';
            else if(s==2) j = j*10+c-'0';
            else if(s==3) e = e*10+c-'0';
        }
        k++;
    }

    //将mu,nu,tu放入稀疏矩阵M中
    M->mu = mu; M->nu = nu; M->tu = tu;

    //得到稀疏矩阵M中的rpos
    int rposi[MAXRC+1];     //rposi表示每一行中非零元的个数
    for(k=0; k<=MAXRC; k++) //初始化rposi
        rposi[k] = 0;
    for(k=1; k<=tu; k++)    //计算每一行中非零元的个数
        rposi[M->data[k].i]++;

    for(i=1; i<=mu; i++)    //找到第一个非零元的行号
        if(rposi[i]){
            M->rpos[i]=1;
            break;
        }
    for(k=i+1; k<=mu; k++)  //根据rposi判断之后让rpos
        M->rpos[k] = M->rpos[k-1] + rposi[k-1];

    return OK;
}//GetRLSMatrix


int PrintRLSMatrix(RLSMatrix M)
{
//输出稀疏矩阵
    int i,j,k;
    k = 1;
    //遍历稀疏矩阵M的每一行和列
    for(i=1; i<=M.mu; i++){
        for(j=1; j<=M.nu; j++){
            //判断当前位置是否有非零元
            if(M.data[k].i == i && M.data[k].j == j){
                fprintf(fout, "%d\t", M.data[k].e);
                k++;
            }else
                fprintf(fout, "%d\t", 0);
        }
        fprintf(fout, "\n");
    }

    fprintf(fout, "\n");
    return 0;
}//PrintRLSMatrix
