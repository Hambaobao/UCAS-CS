#include "EHead.h"

int AddQueue(int infloor, Person personi){
	FloorPersonIn *t = &personin[infloor];

	while( t->next!=NULL ){
		t = t->next;
	}

	FloorPersonIn *s;
	s = (FloorPersonIn *)malloc(sizeof(FloorPersonIn));
	s->PersonID = personi.PersonID;
	s->InFloor = personi.InFloor;
	s->OutFloor = personi.OutFloor;
	s->AppearTime = personi.AppearTime;
	s->GiveUpTime = personi.GiveUpTime;
    s->next = NULL;

	t->next = s;
	return 0;
}

void AddAct(int time, void(*fn)(void)){
	time = Time + time;

	Activity *act;
	act=(Activity *)malloc(sizeof(Activity));
	act->next = NULL;
	act->fn = fn;
	act->time = time;

	Activity* p = &activity;
	while(p->next!=NULL){
		if(p->next->time > time)
			break;
		p = p->next;
	}
	act->next = p->next;
	p->next = act;
}

void TestPeople(){
	int i;
	for(i=0; i<MaxFloor; i++){
        FloorPersonIn *pre = &personin[i];
		FloorPersonIn *p = personin[i].next;
		if(p == NULL)
			continue;
		while(p != NULL){
			if(p->GiveUpTime <= Time){
				if(Floor==i && (State>=Idle) )
					break;
				pre->next = p->next;
				printf("Time: %d\t",Time);
				printf("用户%d放弃了等待.\n", p->PersonID);
				free(p);
				p = pre->next;
				continue;
			}
			pre = p;
			p = p->next;
		}
	}
	return;
}

void Input(void){
    int infloor,outfloor,giveuptime,intertime;

	while(1){
		printf("请输入用户的起始楼层:");
		scanf("%d", &infloor);
		printf("请输入用户的目标的楼层:");
		scanf("%d", &outfloor);
		printf("请输入用户的最长容忍时间:");
		scanf("%d", &giveuptime);
		printf("请输入下一个用户的到来时间:");
		scanf("%d", &intertime);
//		if(infloor==0 && outfloor==0 && giveuptime==0 && intertime==0){
//           printf("Input Finished.\n");
//            return;
//        }
		if(!(infloor<0||infloor>MaxFloor-1||outfloor<0||outfloor>MaxFloor-1)&&(infloor!=outfloor))
			break;
        printf("Wrong Input.\n");
        return;
	}

	personnum++;
    person[personnum].PersonID = personnum;
    person[personnum].InFloor = infloor;
    person[personnum].OutFloor = outfloor;
    person[personnum].AppearTime = Time ;
    person[personnum].GiveUpTime = giveuptime + Time;
    person[personnum].InterTime = intertime;

	if(outfloor>infloor)    CallUp[infloor] = 1;
    else                    CallDown[infloor] = 1;

    AddQueue(infloor, person[personnum]);
    AddAct(intertime, Input);

	return;
}


void testinout(void)//检测有无人进出
{
	if(personin[Floor].next || outpos[Floor])
		AddAct(DoorTestTime,testinout);
	else
	{
		State=DoorCloseing;
		CallUp[Floor]=0;
		CallDown[Floor]=0;
		CallCar[Floor]=0;
		AddAct(InOutDoorTime,ElevatorClose);
	}
}

void ElevatorClose(void)//电梯关门
{
	printf("Time: %d\t",Time);
	printf("电梯门关了！\n");
	State=Stop;
}

void ElevatorOpen(void)//电梯开门
{
	printf("Time: %d\t",Time);
	printf("电梯门开了！\n");
	State=DoorOpen;
	AddAct(DoorTestTime,testinout);//检查有无人进出电梯

	if(outpos[Floor])//如果有人要出电梯
		AddAct(InOutPersonTime,PeopleOut);
	else
	{
		if(personin[Floor].next)//如果有人要进电梯
			AddAct(InOutPersonTime,PeopleIn);
	}
}

void PeopleOut(void)// 电梯里的人出
{
	if(outpos[Floor])//如果有人要出电梯
	{
		printf("Time: %d\t",Time);
		printf("用户%d走出了电梯\n",FloorPersonOut[Floor][outpos[Floor]]);
		outpos[Floor]--;
	}

	if(outpos[Floor])//如果还有人要出电梯
	{
		AddAct(InOutPersonTime,PeopleOut);
	}
	else if(personin[Floor].next)//如果没有人出，看有没有人进
	{
		AddAct(InOutPersonTime,PeopleIn);
	}
}

void PeopleIn(void)//电梯进人
{
	if(personin[Floor].next)//如果有人在等电梯
	{
		FloorPersonIn *p=personin[Floor].next;
		personin[Floor].next=p->next;

		int in=p->OutFloor;
		CallCar[in]=1;		//位置请求

		outpos[in]++;
		FloorPersonOut[in][outpos[in]]=p->PersonID;//进入电梯
		
		printf("Time: %d\t",Time);
		printf("用户%d走入了电梯\n",p->PersonID);

	}

	if(personin[Floor].next)//如果还有人在电梯外边
	{
		AddAct(InOutPersonTime,PeopleIn);
	}
}

int GetWhere(void)
{
	static int old=0;
	int isup=0,isdown=0;
	int i;

	for(i=Floor+1;i<MaxFloor;i++)//检查楼上有没有人按电梯
	{
		if(CallCar[i] || CallUp[i] || CallDown[i])
			isup=1;
	}

	for(i=Floor-1;i>=0;i--)//检查楼下有没有人按电梯
	{
		if(CallDown[i] || CallUp[i] || CallCar[i])
			isdown=1;
	}

	if(isup==0 && isdown==0)//如果没有上行也没有下行
		return 0;

	if(old==0)//如果停止
	{
		if(isdown)//有上行信号
			old=GoingDown;
		if(isup)//有下行信号
			old=GoingUp;
		return old;
	}

	if(old == GoingUp && isup)//如果在上行且收到上行信号
		return old;
	else if(old == GoingDown && isdown)//如果如果在下行且收到下行信号
		return old;
	else if(isup)//如果收到上行信号
		old=GoingUp;
	else if(isdown)//如果收到下行信号
		old=GoingDown;
	else
		printf("在选择方向时发生错误\n");

	return old;
}


void ElevatorToFirst(void){//当长时间无人进入时，电梯回到1楼
    if(State!=Idle||Floor==BaseFloor)
        return;
	printf("Time: %d\t",Time);
    printf("长时间无人出入，电梯回到1楼\n");
    CallCar[BaseFloor]=2;//给电梯一个去1层的初始请求
}

void ElevatorStop(void){//电梯停
	printf("Time: %d\t",Time);
    printf("电梯停了，当前为%d层\n",Floor);
    State=Stop;
}

void ElevatorUp(void){//电梯上升
    Floor++;
	printf("Time: %d\t",Time);
    printf("电梯正在上升，当前为%d层\n",Floor);
    if(CallDown[Floor]||CallUp[Floor]||CallCar[Floor]){//如果有人上下电梯，则电梯停靠
        State=SlowUp;
        AddAct(UpDecelerateTime,ElevatorStop);
    }else{
        if(Floor==MaxFloor-1){//如果电梯到顶楼，则电梯停靠
            State=SlowUp;
            AddAct(UpDecelerateTime,ElevatorStop);
        }else{//如果不为上述情况，则继续上升
            AddAct(UpTime,ElevatorUp);
        }
    }
}

void ElevatorDown(void){//电梯下降
    Floor--;
	printf("Time: %d\t",Time);
    printf("电梯正在下降，当前为%d层\n",Floor);
    if(CallDown[Floor]||CallUp[Floor]||CallCar[Floor]){//如果有人上下电梯，则电梯停靠
        State=SlowDown;
        AddAct(DownDecelerateTime,ElevatorStop);
    }else{
        if(Floor==0){//如果电梯到底楼，则电梯停靠
            State=SlowDown;
            AddAct(DownDecelerateTime,ElevatorStop);
        }else{//如果不为上述情况，则继续下降
            AddAct(DownTime,ElevatorDown);
        }
    }
}


void ElevatorMove(void){//用于电梯刚开始完成加速
    if(State==SpeedUp){
		printf("Time: %d\t",Time);
        printf("电梯已完成加速上升\n");
        State=GoingUp;
        AddAct(UpTime,ElevatorUp);
    }else{
		printf("Time: %d\t",Time);
        printf("电梯已完成加速下降\n");
        State=GoingDown;
        AddAct(DownTime,ElevatorDown);
    }
}

void Controler(void){//电梯状态控制
    if(State==Idle||State==Stop){
        if(CallDown[Floor]||CallUp[Floor]||CallCar[Floor]){//当前层有请求
            if(CallCar[BaseFloor]==2){
                CallCar[BaseFloor]=0;
                State=Idle;
		printf("Time: %d\t",Time);
                printf("现在在%d层，无人请求电梯\n",BaseFloor);
                return;
            }
            State=DoorOpening;//将电梯门打开
            AddAct(InOutDoorTime,ElevatorOpen);
        }
        else{//当前层无请求，考虑其他层请求
            int direct=GetWhere();
            if(direct==GoingUp){//电梯启动加速
                State=SpeedUp;
                AddAct(UpAccelerateTime,ElevatorMove);
            }else if(direct==GoingDown){
                State=SpeedDown;
                AddAct(DownAccelerateTime,ElevatorMove);
            }else{
                State=Idle;
                if(Floor!=BaseFloor)
                    AddAct(OverTime,ElevatorToFirst);
            }
        }
    }//电梯正在运行，不改变电梯状态
    return;
}

void Clock(void){//模拟时间
    while(1){
        if(Time>MaxTime){//超出约定时间
            return;
        }
        TestPeople();//检查链表中人的状态
        Controler();
        struct Activity* p = activity.next;
        if(p==NULL){//若活动链表为空，直接结束
            Time=MaxTime;
        }
        if(p&&Time>=p->time){//若不为空，且到达时间点，执行p
            activity.next=p->next;
            p->fn();
            free(p);
        }
        Time++;
    }
}


int main(void){
//	Init();
	Input();
	Clock();
	return 0;
}
