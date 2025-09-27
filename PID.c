#include <stdio.h> 
#include <windows.h>
#include <stdlib.h>
typedef struct
{
	float Kp;
	float Ki;
	float Kd;
	float last_error; // 上一次的误差
	float integral;   // 积分累积值
	float last_number;
	float error_max; //误差最大值
	float integral_max; //积分最大值
	int outcome_max; //输出最大值
}PID;
int a;
void PID_Init(PID* Pid, float Kp, float Ki, float Kd, float error_max, float integral_max, int outcome_max)
{
	Pid->Kp = Kp;
	Pid->Ki = Ki;
	Pid->Kd = Kd;
	Pid->last_error = 0.0; // 初始化上一次的误差为0
	Pid->integral = 0.0;   // 初始化积分累积值为0
	Pid->last_number = 0.0; // 初始化上一次的 number 值为 0
	Pid->error_max = error_max;
	Pid->integral_max = integral_max;
	Pid->outcome_max = outcome_max;

}

int PID_General_Cal(PID* Pid, float tar, float number)
{
	float error = (tar - number); //这一次误差
	if (error > Pid->error_max)
	{
		error = Pid->error_max;
	}
	else if (error < -Pid->error_max)
	{
		error = -Pid->error_max;
	}

	int outcome;

	Pid->integral += error;
	if ((Pid->last_number < tar && number >= tar) || (Pid->last_number > tar && number <= tar))
	{
		Pid->integral = 0.0;
	}
	Pid->last_number = number;
	if (Pid->integral > Pid->integral_max)
	{
		Pid->integral = Pid->integral_max;
	}
	else if (Pid->integral < -Pid->integral_max)
	{
		Pid->integral = -Pid->integral_max;
	}

	float d = error - Pid->last_error;
	outcome = Pid->Kp * error + Pid->Ki * Pid->integral + Pid->Kd * d;
	Pid->last_error = error; //更新上一次误差
	if (outcome > Pid->outcome_max)
	{
		outcome = Pid->outcome_max;
	}
	else if (outcome < -Pid->outcome_max)
	{
		outcome = -Pid->outcome_max;
	}
	return outcome;
}

PID pid1;
PID pid2;
PID pid3;

int main()
{
	float number1 = 0, tar1 = 4000;
	int number2 = 0, tar2 = -4000;
	int number3 = 0, tar3 = 8000;

	PID_Init(&pid1, 18, 0, 0, 4000, 8000.0, 20000);
	PID_Init(&pid2, 5, 0.0135, 0, 4000, 8000.0, 20000);
	PID_Init(&pid3, 5, 0.1, 0, 8000, 8000.0, 20000);

	while (1)
	{
		number1 += 0.1 * PID_General_Cal(&pid1, tar1, number1);
		number1 -= 10;
		printf("现在的值为： %lf\n", number1);
		Sleep(200);//windows.h 中的延时函数,观察数值变化过程。
	}
	system("CLS"); //#include stdlib.h 中清屏函数

	while (number2 != tar2)
	{
		number2 += 0.1 * PID_General_Cal(&pid2, tar2, number2);
		number2 += 10;
		printf("现在的值为： %d\n", number2);
		Sleep(200);//windows.h 中的延时函数,观察数值变化过程。
	}
	system("CLS"); //#include stdlib.h 中清屏函数

	while (number3 != tar3)
	{
		number3 -= 10;
		static int i = 0;
		i++;
		if (PID_General_Cal(&pid3, tar3, number3) > 20000)
		{
			printf("你的无人机因电机转速过快导致无人机坠毁！！！ ");
			break;
		}

		number3 += 0.1 * PID_General_Cal(&pid3, tar3, number3);
		if (i < 20)
		{
			number3 = 0;
		}

		printf("现在的值为： %d\n", number3);
		if (number3 > 10000)
		{
			printf("你的无人机撞到了房顶！！！ ");
			break;
		}
		if (i > 100)
		{
			printf("你的无人机因电量用光，坠毁！！！ ");
			break;
		}

		Sleep(100);//windows.h 中的延时函数,观察数值变化过程。
	}
	return 0;
}
