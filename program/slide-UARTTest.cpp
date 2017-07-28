 //UARTTest.cpp : 定义控制台应用程序的入口点。
 //Mouse Cursor: x = 1550, y = 850


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
int angle(double angle1, double angle2, int dir) {
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

int _tmain(int argc, _TCHAR* argv[])
{

	char chrBuffer[2000];
	unsigned short usLength = 0, usCnt = 0;
	unsigned long ulBaud = 9600, ulComNo = 8;
	signed char cResult = 1;
/*	printf("请输入串口号:\r\nCOM = ");
	scanf("%ld", &ulComNo);
	printf("请输入波特率:(9600、115200或其他)\r\nBaudRate = ");
	scanf("%ld", &ulBaud);
	printf("等待打开串口%d...\r\n", ulComNo);*/

	while (cResult != 0)
		cResult = OpenCOMDevice(ulComNo, ulBaud);

	Sleep(2000);
	while (1)
	{
		if (usCnt++ >= 0)
		{
			usCnt = 0;
			double angle_z1, angle_z2, angle_z3, angle_z4;
			//下面就是每隔 0.1s 读取一个角度值
			usLength = CollectUARTData(ulComNo, chrBuffer);
			if (usLength > 0)
				JY901.CopeSerialData(chrBuffer, usLength);
			angle_z1 = (double)JY901.stcAngle.Angle[2] / 32768 * 180;
			cout << "angle_z1: " << angle_z1 << endl;
			Sleep(100);

			usLength = CollectUARTData(ulComNo, chrBuffer);
			if (usLength > 0)
				JY901.CopeSerialData(chrBuffer, usLength);
			angle_z2 = (double)JY901.stcAngle.Angle[2] / 32768 * 180;
			cout << "angle_z2: " << angle_z2 << endl;
			Sleep(100);

			usLength = CollectUARTData(ulComNo, chrBuffer);
			if (usLength > 0)
				JY901.CopeSerialData(chrBuffer, usLength);
			angle_z3 = (double)JY901.stcAngle.Angle[2] / 32768 * 180;
			cout << "angle_z3: " << angle_z3 << endl;
			Sleep(100);

			usLength = CollectUARTData(ulComNo, chrBuffer);
			if (usLength > 0)
				JY901.CopeSerialData(chrBuffer, usLength);
			angle_z4 = (double)JY901.stcAngle.Angle[2] / 32768 * 180;
			cout << "angle_z4: " << angle_z4 << endl;
			Sleep(100);


			ofstream oFile;
			oFile.open("a.txt");
			//需要将判断结果输出至一个文件里，然后由 AutoHotKey--Slide.exe 来读取并执行相应操作
			int dir1 = direction(angle_z1, angle_z2), dir2 = direction(angle_z2, angle_z3),
				dir3 = direction(angle_z3, angle_z4);
				//这三个变量是用来判断每两个连续读取的角度值是不是都是向一个方向的
			if (dir1 == dir2&&dir1 == dir2) {
				//如果都是朝一个方向的
				if (dir1 == 1 && angle(angle_z1,angle_z4,dir1)>30) {
					//如果都朝左并且总的角度变化量＞30°
					oFile << "left" << endl;
					Sleep(2000);
				}
				else if (dir1 == -1 && angle(angle_z1,angle_z4,dir1)>30) {
					//如果都朝右并且总的角度变化量＞30°
					oFile << "right" << endl;
					Sleep(2000);
				}

			}
			oFile.close();
			oFile.open("a.txt");
			oFile << "0" << endl;
			//在文件中输入 0，以防程序连续读取连续翻页
			oFile.close();
			Sleep(100);
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
