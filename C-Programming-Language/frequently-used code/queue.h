#define BUFFSIZE 500

	int buffer[BUFFSIZE];
	int front=0;
	int rear=0;

void q_push(int a){
	if(rear<BUFFSIZE)
		buffer[rear++]=a;
}

int q_pop(void){
	if(front!=rear)
		return buffer[front++];
}

int buff_full(void){
	if(rear>=BUFFSIZE)
		return 1;
	else
		return 0;
}

int buff_empty(void){
	if(front==rear)
		return 1;
	else
		return 0;
}