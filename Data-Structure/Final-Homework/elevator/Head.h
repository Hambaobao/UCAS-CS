#ifndef _ELEVATORHEAD_H_
#define _ELEVATORHEAD_H_

#include <stdio.h>
#include <stdlib.h>

// 时间
#define DoorTestTime 40         //有人进出测试时间
#define InOutDoorTime 20        //电梯开门关门时间
#define InOutPersonTime 25      //人进出电梯时间
#define OverTime 300            //电梯停候超时时间
#define UpAccelerateTime 15     //电梯上升的加速时间
#define UpTime 51               //电梯上升的匀速时间
#define UpDecelerateTime 14     //电梯上升的减速时间
#define DownAccelerateTime 15   //电梯下降的加速时间
#define DownTime 61             //电梯下降的匀速时间
#define DownDecelerateTime 23   //电梯下降的减速时间
#define MaxTime	10000		//最长时间

//电梯状态
#define GoingUp 1       //匀速上升
#define GoingDown 2     //匀速下降
#define SpeedUp 3       //加速上升
#define SpeedDown 4     //加速下降
#define SlowUp 5        //减速上升
#define SlowDown 6      //减速下降
#define Idle 7          //空闲
#define Stop 8          //停止且已关门
#define DoorOpen 9      //停止且门已打开
#define DoorOpening 10
#define DoorCloseing 11


#define MaxPerson 50
#define MaxFloor 5
#define BaseFloor 1

//输入
typedef struct Person{
    int PersonID;
    int InFloor, OutFloor;
    int GiveUpTime, InterTime,AppearTime;
}Person;
int personnum = 0;
Person person[MaxPerson];

//每一层的进入链表
typedef struct FloorPersonIn{
    int PersonID;
    int InFloor, OutFloor;
    int AppearTime, GiveUpTime;
    struct FloorPersonIn *next;
}FloorPersonIn;
FloorPersonIn personin[5];

//电梯中的人
int FloorPersonOut[5][MaxPerson+1];
int outpos[5];  //电梯中去第i层的人数

//Activity
typedef struct Activity{
	int time;
	void(*fn)(void);
	struct Activity* next;
}Activity;

Activity activity={0,NULL,NULL};

int Time = 0;
//电梯
int CallUp[MaxFloor];
int CallDown[MaxFloor];
int CallCar[MaxFloor];
int Floor = BaseFloor;
int State = Idle;
int D1=0, D2=0, D3=0;


int AddQueue(int infloor, Person personi);
void AddAct(int time, void(*fn)(void));
void TestPeople();
void Input(void);
void testinout(void);
void ElevatorClose(void);
void ElevatorOpen(void);
void PeopleOut(void);
void PeopleIn(void);
int GetWhere(void);
void tofirst(void);
void ElevatorStop(void);
void ElevatorUp(void);
void ElevatorDown(void);
void ElevatorMove(void);
void Controler(void);

#endif
