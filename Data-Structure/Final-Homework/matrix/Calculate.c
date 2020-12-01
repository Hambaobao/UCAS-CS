#include "MatrixHead.h"

int InitRLSMatrix(RLSMatrix *M)
{
// 初始化以“行逻辑链接的顺序表”表示的稀疏矩阵M
    int i;
    Triple init;                //init表示行、列、值都为0的元素
    init.i = init.j = 0;
    init.e = 0;
    if(!M) return ERROR;        //若没分配空间，返回ERROR
    for(i=0; i<MAXSIZE+1; i++){ //M元素全部初始化为零元
        M->data[i] = init;
    }
    for(i=0; i<MAXRC+1; i++){   //各行第一个非零元位置表初始化为0
        M->rpos[i] = 0;
    }
    M->mu = M->nu = M->tu = 0;  //矩阵行数、列数、非零元数初始化0

    return OK;
}//InitRLSMatrix

//输出矩阵的和
RLSMatrix addMatrix(RLSMatrix M, RLSMatrix N, int *error)
{
	int m=1, n=1, c=1, i;
	RLSMatrix Q;
	InitRLSMatrix(&Q);

	//判断是否符合加法要求
	if(M.mu!=N.mu || M.nu!=N.nu)
		error=ADD_ERROR;

	Q.mu=M.mu;
	Q.nu=M.nu;
	//根据M和N中非零元，比较其行号和列号
	//判断M当前元和N当前元是否处于同一位置
	//若不同，则将M和N中靠前的元素赋值给Q
	//若相同，则进行加法运算
	while(m<=M.tu){
		while(n<=N.tu){
			if(M.data[m].i<N.data[n].i){
				Q.data[c].i = M.data[m].i;
				Q.data[c].j = M.data[m].j;
				Q.data[c].e = M.data[m].e;
				c++;m++;
				break;
			}else if(M.data[m].i>N.data[n].i){
				Q.data[c].i = N.data[n].i;
				Q.data[c].j = N.data[n].j;
				Q.data[c].e = N.data[n].e;
				c++;n++;
			}else{
				if(M.data[m].j<N.data[n].j){
					Q.data[c].i = M.data[m].i;
					Q.data[c].j = M.data[m].j;
					Q.data[c].e = M.data[m].e;
					c++;m++;
					break;
				}else if(M.data[m].j>N.data[n].j){
					Q.data[c].i = N.data[n].i;
					Q.data[c].j = N.data[n].j;
					Q.data[c].e = N.data[n].e;
					c++;n++;
				}else{
                    //将对应位置上的数相加，并将结果赋值给Q
					Q.data[c].i = N.data[n].i;
					Q.data[c].j = N.data[n].j;
					Q.data[c].e = M.data[m].e+N.data[n].e;
					//判断得到的数是否为0，若不为0，则记录
					if(Q.data[c].e!=0){
						c++;
					}
					m++;n++;
					break;
				}
			}
		}
	}

    //若M或N中有剩余的元素，将其全部给Q
	if(m>M.tu&&n<=N.tu){
		for(i=n;i<=N.tu;i++){
			Q.data[c].i = N.data[n].i;
			Q.data[c].j = N.data[n].j;
			Q.data[c].e = N.data[n].e;
			c++;
		}
	}
	if(n>N.tu&&m<=M.tu){
		for(i=m;m<=M.tu;i++){
			Q.data[c].i = M.data[m].i;
			Q.data[c].j = M.data[m].j;
			Q.data[c].e = M.data[m].e;
			c++;
		}
	}

    //矩阵Q中元素的个数
    Q.tu = c-1;

    return Q;
}

//输出矩阵的差
RLSMatrix subMatrix(RLSMatrix M, RLSMatrix N, int *error)
{
	int m=1;
	int n=1;
	int c=1;
	int i;
	RLSMatrix Q;
	InitRLSMatrix(&Q);

	if(M.mu!=N.mu || M.nu!=N.nu)
		error=ADD_ERROR;//判断是否符合加法要求

	Q.mu=M.mu;
	Q.nu=M.nu;
	while(m<=M.tu){
		while(n<=N.tu){
			if(M.data[m].i<N.data[n].i){
				Q.data[c].i = M.data[m].i;
				Q.data[c].j = M.data[m].j;
				Q.data[c].e = M.data[m].e;
				c++;m++;break;
			}
			else if(M.data[m].i>N.data[n].i){
				Q.data[c].i = N.data[n].i;
				Q.data[c].j = N.data[n].j;
				Q.data[c].e = N.data[n].e;
				c++;n++;
			}
			else{
				if(M.data[m].j<N.data[n].j){
					Q.data[c].i = M.data[m].i;
					Q.data[c].j = M.data[m].j;
					Q.data[c].e = M.data[m].e;
					c++;m++;break;
				}
				else if(M.data[m].j>N.data[n].j){
					Q.data[c].i = N.data[n].i;
					Q.data[c].j = N.data[n].j;
					Q.data[c].e = N.data[n].e;
					c++;n++;
				}
				else{
					Q.data[c].i = N.data[n].i;
					Q.data[c].j = N.data[n].j;
					Q.data[c].e = M.data[m].e-N.data[n].e;//将对应位置上的数相加，并将结果赋值给Q
					if(Q.data[c].e!=0){
						c++;//判断得到的数是否为0，若不为0，则记录
					}
					m++;n++;break;
				}
			}
		}
	}

	if(m>M.tu&&n<=N.tu){
		for(i=n;i<=N.tu;i++){
			Q.data[c].i = N.data[n].i;
			Q.data[c].j = N.data[n].j;
			Q.data[c].e = -N.data[n].e;
			c++;
		}
	}
	if(n>N.tu&&m<=M.tu){
		for(i=m;m<=M.tu;i++){
			Q.data[c].i = M.data[m].i;
			Q.data[c].j = M.data[m].j;
			Q.data[c].e = M.data[m].e;
			c++;
		}
	}
//printf("%d",c);
Q.tu=c-1;//输出矩阵中元素的个数
return Q;
}

//输出矩阵的成积
RLSMatrix multiplyMatrix(RLSMatrix M, RLSMatrix N, int *error)
{
	RLSMatrix Q;
				//行列不符合要求，返回错误
	if(M.nu != N.mu)
    {
		error = MULT_ERROR;
		return Q;
	}
				//Q的初始化
	Q.mu = M.mu;
	Q.nu = N.nu;
	Q.tu = 0;

	int ctemp[Q.nu];
	int tp,t,p,q,arow,brow,ccol;

	if(M.tu * N.tu != 0)//Q不是非零矩阵
	{
		for(arow = 1;arow <= M.mu;++arow)//处理M的每一行
		{
			for(int i = 0;i <= Q.nu;i++)//当前各行元素累加器清零
				ctemp[i]=0;

			Q.rpos[arow] = Q.tu + 1;

			if(arow < M.mu)//求出每一行非零元素个数的上界
				tp = M.rpos[arow + 1];
			else
				tp = M.tu+1;

			for(p = M.rpos[arow];p < tp;++p)//对当前行中每一个非零元找到对应元在N中的行号
			{
				brow = M.data[p].j;

				if(brow < N.mu)//求出矩阵N中第brow行非零元素个数的上界
					t = N.rpos[brow + 1];
				else
					t = N.tu+1;

				for(q = N.rpos[brow];q < t;++q)
				{
					ccol = N.data[q].j;//乘积元素在Q中的列号

					ctemp[ccol] += M.data[p].e * N.data[q].e;
				}//for q

			}//求得Q中第crow(=arow)行的非零元

			for(ccol = 1;ccol < Q.nu;++ccol)//压缩存储该行非零元
			{
				if(ctemp[ccol])
				{
					if(++Q.tu > MAXSIZE)//如果Q中非零元个数超过MASIZE，则出错
					{
						error = MULT_ERROR;
						return Q;
					}

					Q.data[Q.tu].i = arow;
					Q.data[Q.tu].j = ccol;
					Q.data[Q.tu].e = ctemp[ccol];

				}//if
			}
		}//for arow
	}//if
	return Q;
}//matrix_mult
