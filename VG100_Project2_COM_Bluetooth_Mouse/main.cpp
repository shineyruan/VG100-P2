 //UARTTest.cpp : 定义控制台应用程序的入口点。
 //Mouse Cursor: x = 1535, y = 855


#include "stdafx.h"
#include "Com.h"
#include "JY901.h"

#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <Winuser.h>
#include <time.h>
#include <iostream>
#include <fstream>
#define PI 3.1415926
#define T 0.1 //读取周期
#define CORRECTION_TERM	30
using namespace std;
unsigned char ucComNo[2] ={0,0};


int direction(double angle1, double angle2) {
	//判断手的移动方向
	//angle1为初始角度，angle2为末角度
	// 1: left, -1: right
	if (angle1*angle2 > 0) {
		if (angle2 < angle1)
			return -1;
		else return 1;
	}
	else{
		if (angle1 > 0)
			if (angle2 > angle1 - 180)
				return -1;
			else return 1;
		else {
			if (angle2 > angle1 + 180)
				return -1;
			else return 1;
		}
	}
}

double angle(double angle1, double angle2, int dir) {
	//计算角度的变化量。其实完全可以和前面那个函数合并在一起的，不过当时临时码的就没多想
	//具体怎么计算的。。。画个圆应该就知道了，过了+180°之后是-180°
	if (dir == 1) {
		if (angle1*angle2 > 0)
			return angle2 - angle1;
		else if (angle1 > 0)
			return 360 - angle1 + angle2;
		else if (angle1 < 0)
			return angle2 - angle1;
	}
	if (dir == -1) {
		if (angle1*angle2 > 0)
			return angle1 - angle2;
		else if (angle1 > 0)
			return angle1 - angle2;
		else if (angle1 < 0)
			return 360 - angle2 + angle1;
	}
}

void filter(double* ang)
{
	/*This function uses the slope (difference) of the 1st & 2nd angles to correct deviations of the 3rd angle.
		The correction term (threshold) has been defined as 50 degree (see #define CORRECTION_TERM).
		---Ruan*/

	double difference = ang[1] - ang[0];
	double prediction = ang[1] + difference;
	if (abs(prediction - ang[2]) > CORRECTION_TERM) ang[2] = prediction;

	//crossing the -180/180 line is not allowed.
	if (ang[0] > 0 && ang[1] >= ang[0] && ang[2] < 0) ang[2] = 180;
	if (ang[0] < 0 && ang[1] <= ang[0] && ang[2] > 0) ang[2] = -180;
}
int _tmain(int argc, _TCHAR* argv[])
{
	//while (1)
	//{
	//	int x = 0, y = 0;
	//	printf("Input x,y");
	//	scanf("%d,%d", &x, &y);
	//	SetCursorPos(x, y);
	//}
	char chrBuffer[2000];
	unsigned short usLength = 0, usCnt = 0;
	unsigned long ulBaud = 9600, ulComNo = 8;
	signed char cResult = 1;
	double angle_x[] = { 0,0,0 }, angle_y[3] = { 0,0,0 }, angle_z[3] = { 0,0,0 };
	double angvel_x = 0, angvel_y = 0, angvel_z = 0;
	int mode = 0, isdrag = 0;
	int flag_slide = 0, flag_move = 0, flag_drag = 0, flag_notDrag = 0;

	
	printf("Input COM No.: \r\nCOM = ");
	scanf("%ld", &ulComNo);
	printf("Input Baud rate: \r\nBaudRate = ");
	scanf("%ld", &ulBaud);
	//printf("Input mode (1 for sliding PPT and 2 for moving cursor): ");
	//scanf("%d", &mode);
	printf("Waiting to open COM %d...\r\n", ulComNo);

	while (cResult != 0)
		cResult = OpenCOMDevice(ulComNo, ulBaud);

	printf("Open Successful! Now turn your wrist fast to the right to switch modes!\n\n");
	Sleep(2000);
	while (1)
	{
		if (usCnt++ >= 0)
		{
			usCnt = 0;

			//Turning the wrist fast to the right to switch modes, using angular velocity w.r.t. y-axis.---Ruan
			usLength = CollectUARTData(ulComNo, chrBuffer);
			if (usLength > 0)
				JY901.CopeSerialData(chrBuffer, usLength);
			angvel_x = (double)JY901.stcGyro.w[0] / 32768 * 2000;
			angvel_y = (double)JY901.stcGyro.w[1] / 32768 * 2000;
			angvel_z = (double)JY901.stcGyro.w[2] / 32768 * 2000;
			if (angvel_y > 800) ++mode;


			if (mode % 2 == 1)  //This is sliding PPT mode---Ruan
			{
				if (flag_slide == 0)
				{
					printf("You're now in slides-switching mode!\n");
					flag_slide = 1;
					Sleep(2000);
				}
				double angle_z1, angle_z2, angle_z3, angle_z4;
				//下面就是每隔 0.1s 读取一个角度值
				usLength = CollectUARTData(ulComNo, chrBuffer);
				if (usLength > 0)
					JY901.CopeSerialData(chrBuffer, usLength);
				angle_z1 = (double)JY901.stcAngle.Angle[2] / 32768 * 180;
				cout << "angle_z1: " << angle_z1 << endl;
//				Sleep(100);

				usLength = CollectUARTData(ulComNo, chrBuffer);
				if (usLength > 0)
					JY901.CopeSerialData(chrBuffer, usLength);
				angle_z2 = (double)JY901.stcAngle.Angle[2] / 32768 * 180;
				cout << "angle_z2: " << angle_z2 << endl;
//				Sleep(100);

				usLength = CollectUARTData(ulComNo, chrBuffer);
				if (usLength > 0)
					JY901.CopeSerialData(chrBuffer, usLength);
				angle_z3 = (double)JY901.stcAngle.Angle[2] / 32768 * 180;
				cout << "angle_z3: " << angle_z3 << endl;
//				Sleep(100);

				usLength = CollectUARTData(ulComNo, chrBuffer);
				if (usLength > 0)
					JY901.CopeSerialData(chrBuffer, usLength);
				angle_z4 = (double)JY901.stcAngle.Angle[2] / 32768 * 180;
				cout << "angle_z4: " << angle_z4 << endl;
//				Sleep(100);

				//ofstream oFile;
				//oFile.open("a.txt");
				////需要将判断结果输出至一个文件里，然后由 AutoHotKey--Slide.exe 来读取并执行相应操作
				//int dir1 = direction(angle_z1, angle_z2), dir2 = direction(angle_z2, angle_z3),
				//	dir3 = direction(angle_z3, angle_z4);
				////这三个变量是用来判断每两个连续读取的角度值是不是都是向一个方向的
				//if (dir1 == dir2&&dir1 == dir2) {
				//	//如果都是朝一个方向的
				//	if (dir1 == 1 && angle(angle_z1, angle_z4, dir1) > 30) {
				//		
				//		//如果都朝左并且总的角度变化量＞30°
				//		oFile << "left" << endl;
				//		Sleep(2000);
				//	}
				//	else if (dir1 == -1 && angle(angle_z1, angle_z4, dir1) > 30) {
				//		
				//		//如果都朝右并且总的角度变化量＞30°
				//		oFile << "right" << endl;
				//		Sleep(2000);
				//	}

				//}
				//oFile.close();
				//oFile.open("a.txt");
				//oFile << "0" << endl;
				////在文件中输入 0，以防程序连续读取连续翻页
				//oFile.close();

				flag_move = 0;

			}
			else if (mode % 2 == 0 && mode != 0)	//This is control mouse cursor mode.---Ruan
			{
				if (flag_move == 0)
				{
					printf("You're now in mouse cursor control mode!\n");
					flag_move = 1;
					Sleep(2000);
				}

				//Turning the wrist downward fast to switch modes: drag or not drag.---Ruan
				usLength = CollectUARTData(ulComNo, chrBuffer);
				if (usLength > 0)
					JY901.CopeSerialData(chrBuffer, usLength);
				angvel_x = (double)JY901.stcGyro.w[0] / 32768 * 2000;
				angvel_y = (double)JY901.stcGyro.w[1] / 32768 * 2000;
				angvel_z = (double)JY901.stcGyro.w[2] / 32768 * 2000;
				if (angvel_x < -500) ++isdrag;

				if (isdrag % 2 == 0) //This is no-dragging mode: just move cursor.---Ruan
				{
					if (flag_notDrag == 0)
					{
						printf("Welcome to the no-dragging mode!\n");
						flag_notDrag = 1;
						Sleep(2000);
					}

					//Mouse cursor control---Read data every 1ms

					usLength = CollectUARTData(ulComNo, chrBuffer);
					if (usLength > 0)
						JY901.CopeSerialData(chrBuffer, usLength);
					angle_x[0] = (double)JY901.stcAngle.Angle[0] / 32768 * 180;
					angle_y[0] = (double)JY901.stcAngle.Angle[1] / 32768 * 180;
					angle_z[0] = (double)JY901.stcAngle.Angle[2] / 32768 * 180;
					cout << "angle1_xyz: " << angle_x[0] << "   " << angle_y[0] << "   " << angle_z[1] << endl;


					usLength = CollectUARTData(ulComNo, chrBuffer);
					if (usLength > 0)
						JY901.CopeSerialData(chrBuffer, usLength);
					angle_x[1] = (double)JY901.stcAngle.Angle[0] / 32768 * 180;
					angle_y[1] = (double)JY901.stcAngle.Angle[1] / 32768 * 180;
					angle_z[1] = (double)JY901.stcAngle.Angle[2] / 32768 * 180;
					cout << "angle2_xyz: " << angle_x[1] << "   " << angle_y[1] << "   " << angle_z[1] << endl;


					usLength = CollectUARTData(ulComNo, chrBuffer);
					if (usLength > 0)
						JY901.CopeSerialData(chrBuffer, usLength);
					angle_x[2] = (double)JY901.stcAngle.Angle[0] / 32768 * 180;
					angle_y[2] = (double)JY901.stcAngle.Angle[1] / 32768 * 180;
					angle_z[2] = (double)JY901.stcAngle.Angle[2] / 32768 * 180;
					cout << "angle3_xyz: " << angle_x[2] << "   " << angle_y[2] << "   " << angle_z[2] << endl;

					//usLength = CollectUARTData(ulComNo, chrBuffer);
					//if (usLength > 0)
					//	JY901.CopeSerialData(chrBuffer, usLength);
					//angvel_x = (double)JY901.stcGyro.w[0] / 32768 * 2000;
					//angvel_y = (double)JY901.stcGyro.w[1] / 32768 * 2000;
					//angvel_z = (double)JY901.stcGyro.w[2] / 32768 * 2000;
					//cout << "angvel_xyz: " << angvel_x << "   " << angvel_y << "   " << angvel_z << endl;

					//if (angvel_y > 1000) Sleep(1000);


					filter(angle_x); filter(angle_y); filter(angle_z);

					double alpha = angle_x[2];
					double beta = angle_y[2];
					double theta = angle_z[2];

					double xx = 767.5, yy = 427.5;
					double dx = -theta / 60 * 767.5;
					double dy = -alpha / 90 * 427.5;
					double x = xx + dx;
					double y = yy + dy;


					//设置鼠标位置

					if (x > 1535) x = 1550;
					if (y > 855) y = 850;

					SetCursorPos(x, y);
					/*				ofstream oFilex, oFiley;
					oFilex.open("x.txt");
					oFiley.open("y.txt");
					oFilex << x << endl;
					oFiley << y << endl;
					*/
					flag_drag = 0;
				}
				else if (isdrag % 2 == 1)	//This is dragging mode.---Ruan
				{
					if (flag_drag == 0)
					{
						printf("Welcome to the dragging mode!\n");
						flag_drag = 1;
						Sleep(2000);
					}


					//float x1, x2, y1, y2;//drag需要两个坐标，drag from (x1,y1) to (x2,y2)
					//bool initial = false;//用来判断第一次输入的值被记录，第一次之后就相当于斐波那契数列。。。
					//					 //把前一次得到的x2,y2作为下一次的x1,y1

					//usLength = CollectUARTData(ulComNo, chrBuffer);
					//if (usLength > 0)
					//	JY901.CopeSerialData(chrBuffer, usLength);
					//if (!initial)
					//{
					//	float alpha = (float)JY901.stcAngle.Angle[0] / 32768 * 180;
					//	float beta = (float)JY901.stcAngle.Angle[1] / 32768 * 180;
					//	float theta = (float)JY901.stcAngle.Angle[2] / 32768 * 180;

					//	float xx = 775, yy = 425;
					//	float dx = (float)beta / 45 * 775;
					//	float dy = -(float)alpha / 45 * 425;
					//	x1 = xx + dx;
					//	y1 = yy + dy;


					//	//设置鼠标位置

					//	if (x1 > 1550) x1 = 1550;
					//	if (y1 > 850) y1 = 850;
					//	initial = true;
					//}
					//float alpha = (float)JY901.stcAngle.Angle[0] / 32768 * 180;
					//float beta = (float)JY901.stcAngle.Angle[1] / 32768 * 180;
					//float theta = (float)JY901.stcAngle.Angle[2] / 32768 * 180;

					//float xx = 775, yy = 425;
					//float dx = (float)beta / 45 * 775;
					//float dy = -(float)alpha / 45 * 425;
					//x2 = xx + dx;
					//y2 = yy + dy;


					////设置鼠标位置
					//if (x2 > 1550) x2 = 1550;
					//if (y2 > 850) y2 = 850;

					////下面这简单的一行就是滤波算法。。。
					//if ((x1 - x2)*(x1 - x2) + (y1 - y2)* (y1 - y2) > 100000)
					//	continue;

					////SetCursorPos(x, y);
					////下面还是把读取的坐标传入文件给 drag.exe 读
					//ofstream oFilex1, oFiley1, oFilex2, oFiley2;
					//oFilex1.open("x1.txt");
					//oFiley1.open("y1.txt");
					//oFilex2.open("x2.txt");
					//oFiley2.open("y2.txt");
					//oFilex1 << x1 << endl;
					//oFiley1 << y1 << endl;
					//oFilex2 << x2 << endl;
					//oFiley2 << y2 << endl;
					//oFilex1.close();
					//oFiley1.close();
					//oFilex2.close();
					//oFiley2.close();
					//x1 = x2;
					//y1 = y2;

					flag_notDrag = 0;
				}

				flag_slide = 0;
			}



		}
	}
	return 0;
}

//printf("Time:20%d-%d-%d %d:%d:%.3f\r\n", (short)JY901.stcTime.ucYear, (short)JY901.stcTime.ucMonth,
//	(short)JY901.stcTime.ucDay, (short)JY901.stcTime.ucHour, (short)JY901.stcTime.ucMinute,
//	(float)JY901.stcTime.ucSecond + (float)JY901.stcTime.usMiliSecond / 1000);

//printf("Acc:%.3f %.3f %.3f\r\n", (float)JY901.stcAcc.a[0] / 32768 * 16, (float)JY901.stcAcc.a[1] / 32768 * 16,
//	(float)JY901.stcAcc.a[2] / 32768 * 16);

//printf("Gyro:%.3f %.3f %.3f\r\n", (float)JY901.stcGyro.w[0] / 32768 * 2000, (float)JY901.stcGyro.w[1] / 32768 * 2000,
//	(float)JY901.stcGyro.w[2] / 32768 * 2000);

//printf("Angle:%.3f %.3f %.3f\r\n", (float)JY901.stcAngle.Angle[0] / 32768 * 180, (float)JY901.stcAngle.Angle[1] / 32768 * 180,
//	(float)JY901.stcAngle.Angle[2] / 32768 * 180);

//printf("Mag:%d %d %d\r\n", JY901.stcMag.h[0], JY901.stcMag.h[1], JY901.stcMag.h[2]);

//printf("Pressure:%lx Height%.2f\r\n", JY901.stcPress.lPressure, (float)JY901.stcPress.lAltitude / 100);

//printf("DStatus:%d %d %d %d\r\n",JY901.stcDStatus.sDStatus[0],JY901.stcDStatus.sDStatus[1],
//	JY901.stcDStatus.sDStatus[2], JY901.stcDStatus.sDStatus[3]);

//printf("Longitude:%ldDeg%.5fm Lattitude:%ldDeg%.5fm\r\n", JY901.stcLonLat.lLon / 10000000,
//	(double)(JY901.stcLonLat.lLon % 10000000) / 1e5, JY901.stcLonLat.lLat / 10000000,
//	(double)(JY901.stcLonLat.lLat % 10000000) / 1e5);

//printf("GPSHeight:%.1fm GPSYaw:%.1fDeg GPSV:%.3fkm/h\r\n\r\n", (float)JY901.stcGPSV.sGPSHeight / 10,
//	(float)JY901.stcGPSV.sGPSYaw / 10, (float)JY901.stcGPSV.lGPSVelocity / 1000);