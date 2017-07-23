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
unsigned char ucComNo[2] = { 0,0 };


int _tmain(int argc, _TCHAR* argv[])
{
	clock_t start, finish;
	double totaltime;
	start = clock();


	char chrBuffer[2000];
	unsigned short usLength = 0, usCnt = 0;
	unsigned long ulBaud = 9600, ulComNo = 10;
	signed char cResult = 1;
	int temp = 0;

	/*	printf("请输入串口号:\r\nCOM = ");
	scanf("%ld", &ulComNo);
	printf("请输入波特率:(9600、115200或其他)\r\nBaudRate = ");
	scanf("%ld", &ulBaud);
	printf("等待打开串口%d...\r\n", ulComNo);*/

	while (cResult != 0)
		cResult = OpenCOMDevice(ulComNo, ulBaud);

	int flag[2] = { 0,0 };
	int j = 0;
	float a, b, c, a1, b1, c1;
	float al1, al2, be1, be2, th1, th2;
	float al, be, th;

	while (1)
	{
		usLength = CollectUARTData(ulComNo, chrBuffer);

		if (usLength > 0)
			JY901.CopeSerialData(chrBuffer, usLength);


		if (usCnt++ >= 0)
		{
			usCnt = 0;

			///*Method 1: Use acceleration to integrate and get position vectors.*/

			///*a1,b1,c1是测出的三向加速度，al1,be1,th1是校准的角度值，al2,be2,th2是每一时刻的角度值，a,b,c,是转化为校准方向的三向加速度，
			//		vx,vy,vz是校准方向的三向速度，xyz是校准方向的三向位移。*/

			////读取数据
			//a1 = (float)JY901.stcAcc.a[0] / 32768 * 16;
			//b1 = (float)JY901.stcAcc.a[1] / 32768 * 16;
			//c1 = (float)JY901.stcAcc.a[2] / 32768 * 16;
			//
			//if ((flag[0] == 0) || (flag[1] == 0))
			//{
			//	
			//	al1 = (float)JY901.stcAngle.Angle[0] / 32768 * 180;
			//	be1 = (float)JY901.stcAngle.Angle[1] / 32768 * 180;
			//	th1 = (float)JY901.stcAngle.Angle[2] / 32768 * 180;
			//	if (temp + 1 < 3) flag[temp++] = 1;
			//}


			//al2 = (float)JY901.stcAngle.Angle[0] / 32768 * 180;
			//be2 = (float)JY901.stcAngle.Angle[1] / 32768 * 180;
			//th2 = (float)JY901.stcAngle.Angle[2] / 32768 * 180;



			////角度转化
			//al = al1 - al2;
			//be = be1 - be2;
			//th = th1 - th2;
			//a = cos(be)*cos(th)*a1 - (sin(al)*cos(be)*cos(th) + cos(al)*sin(th))*b1 + (sin(al)*sin(th) - cos(al)*sin(be)*cos(th))*c1;
			//b = cos(be)*sin(th)*a1 + (cos(al)*cos(th) - sin(th)*sin(al)*sin(be))*b1 - (cos(th)*sin(al) + sin(th)*cos(al)*sin(be))*c1;
			//c = sin(be)*a1 + sin(al)*cos(be)*b1 + cos(al)*cos(be)*c1-8;
			//

			////积分(Euler method 逼近)
			//static float vx1 = 0;
			//static float vy1 = 0;
			//static float vz1 = 0;
			//static float vx2 = 0;
			//static float vy2 = 0;
			//static float vz2 = 0;
			//float i;
			//i = vx2, vx2 = vx1 + a*T, vx1 = i;
			//i = vy2, vy2 = vy1 + a*T, vy1 = i;
			//i = vz2, vz2 = vz1 + a*T, vz1 = i;

			//static float x1 = 0;
			//static float y1 = 0;
			//static float z1 = 0;
			//static float x2 = 0;
			//static float y2 = 0;
			//static float z2 = 0;
			//i = x2, x2 = x1 + (vx1 + vx2)*0.5*T, x1 = i;
			//i = y2, y2 = y1 + (vy1 + vy2)*0.5*T, y1 = i;
			//i = z2, z2 = z1 + (vz1 + vz2)*0.5*T, z1 = i;



			//printf("vx: %f\n", a);
			//printf("vy: %f\n", b);
			//printf("vz: %f\n", c);
			//printf("\n\n");
			////printf("a: %f\n", al);
			////printf("b: %f\n", be);
			////printf("c: %f\n", th);


			/*Method 2: Use angle directly.*/

			float alpha = (float)JY901.stcAngle.Angle[0] / 32768 * 180;
			float beta = (float)JY901.stcAngle.Angle[1] / 32768 * 180;
			float theta = (float)JY901.stcAngle.Angle[2] / 32768 * 180;

			float xx = 775, yy = 425;
			float dx = (float)beta / 45 * 775;
			float dy = -(float)alpha / 45 * 425;
			float x = xx + dx;
			float y = yy + dy;


			//设置鼠标位置

			if (x > 1550) x = 1550;
			if (y > 850) y = 850;

			//SetCursorPos(x, y);
			ofstream oFilex, oFiley;
			oFilex.open("x.txt");
			oFiley.open("y.txt");
			oFilex << x << endl;
			oFiley << y << endl;
			Sleep(10);
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


/*	int t = 0;
float a[10000], b[10000], c[10000], a1[10000], b1[10000], c1[10000];
float al1[10000], al2[10000], be1[10000], be2[10000], th1[10000], th2[10000];
float al[10000], be[10000], th[10000];
float vx[10000], vy[10000], vz[10000], x[10000], y[10000], z[10000];


for (t = 0; t<100; t++) {
a1[t] = (float)JY901.stcAcc.a[0] / 32768 * 16;
b1[t] = (float)JY901.stcAcc.a[1] / 32768 * 16;
c1[t] = (float)JY901.stcAcc.a[2] / 32768 * 16;
al1[t] = 0, be1[t] = 0, th1[t] = 0;
al2[t] = (float)JY901.stcAngle.Angle[0] / 32768 * 180;
be2[t] = (float)JY901.stcAngle.Angle[1] / 32768 * 180;
th2[t] = (float)JY901.stcAngle.Angle[2] / 32768 * 180;
}

for (t = 0; t < 100; t++) {
al[t] = al1[t] - al2[t];
be[t] = be1[t] - be2[t];
th[t] = th1[t] - th2[t];
a[t] = cos(be[t])*cos(th[t])*a1[t] - (sin(al[t])*cos(be[t])*cos(th[t]) + cos(al[t])*sin(th[t]))*b1[t]
+ (sin(al[t])*sin(th[t]) - cos(al[t])*sin(be[t])*cos(th[t]))*c1[t];

b[t] = cos(be[t])*sin(th[t])*a1[t] + (cos(al[t])*cos(th[t]) - sin(th[t])*sin(al[t])*sin(be[t]))*b1[t]
- (cos(th[t])*sin(al[t]) + sin(th[t])*cos(al[t])*sin(be[t]))*c1[t];

c[t] = sin(be[t])*a1[t] + sin(al[t])*cos(be[t])*b1[t] + cos(al[t])*cos(be[t])*c1[t];
//以上是转化
if (t == 0) {
vx[t] = a[t] * T; vy[t] = b[t] * T; vz[t] = c[t] * T;
x[t] = 0.5*T*vx[t]; y[t] = 0.5*T*vy[t]; z[t] = 0.5*T*vz[t];
}
else {
vx[t] = a[t] * T + vx[t - 1];
vy[t] = b[t] * T + vy[t - 1];
vz[t] = c[t] * T + vz[t - 1];

x[t] = 0.5*T*(vx[t] + vx[t - 1]) + x[t - 1];
y[t] = 0.5*T*(vy[t] + vy[t - 1]) + y[t - 1];
z[t] = 0.5*T*(vz[t] + vz[t - 1]) + z[t - 1];
}
//以上是积分
printf("a[t]: %f\nvx[t]: %f\nx[t]: %f\n", a[t], vx[t], x[t]);
//输出验证
}



finish = clock();
totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
printf("\n此程序的运行时间为%lf秒\n", totaltime);




*/

//
//  main.cpp
//  fdsafdsa
//
//  Created by 周瑞星 on 2017/7/16.
//  Copyright © 2017年 周瑞星. All rights reserved.
//

/*#include <iostream>
#include <cmath>
#include<time.h>
#define PI 3.1415926
#define T 0.1 //读取周期
using namespace std;
int main() {
clock_t start, finish;
double totaltime;
start = clock();



int t = 0;
float a, b, c, a1, b1, c1;
float al1, al2, be1, be2, th1, th2;
float al, be, th;
//a1,b1,c1是测出的三向加速度，al1,be1,th1是校准的角度值，al2,be2,th2是每一时刻的角度值，a,b,c,是转化为校准方向的三向加速度，vx,vy,vz是校准方向的三向速度，xyz是....的三向位移。


//a1 = 0, b1 = t, c1 = t;
//al1 = 0, be1 = 0, th1 = 0;
//al2 = PI / 4, be2 = 0, th2 = -PI / 2;
//这里的数据是我随便给的。


al = al1 - al2;
be = be1 - be2;
th = th1 - th2;
a = cos(be)*cos(th)*a1 - (sin(al)*cos(be)*cos(th) + cos(al)*sin(th))*b1 + (sin(al)*sin(th) - cos(al)*sin(be)*cos(th))*c1;
b = cos(be)*sin(th)*a1 + (cos(al)*cos(th) - sin(th)*sin(al)*sin(be))*b1 - (cos(th)*sin(al) + sin(th)*cos(al)*sin(be))*c1;
c = sin(be)*a1 + sin(al)*cos(be)*b1 + cos(al)*cos(be)*c1;
//以上是转化

static float vx1 = 0;
static float vy1 = 0;
static float vz1 = 0;
static float vx2 = 0;
static float vy2 = 0;
static float vz2 = 0;
float i;
i = vx2, vx2 = vx1 + a*T, vx1 = i;
i = vy2, vy2 = vy1 + a*T, vy1 = i;
i = vz2, vz2 = vz1 + a*T, vz1 = i;

static float x1 = 0;
static float y1 = 0;
static float z1 = 0;
static float x2 = 0;
static float y2 = 0;
static float z2 = 0;
i = x2, x2 = x1 + (vx1 + vx2)*0.5*T, x1 = i;
i = y2, y2 = y1 + (vy1 + vy2)*0.5*T, y1 = i;
i = z2, z2 = z1 + (vz1 + vz2)*0.5*T, z1 = i;
//以上是积分

cout << a << endl;
cout << vx2 << endl;
cout << x2 << endl;
//输出验证


finish = clock();
totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
cout << "\n此程序的运行时间为" << totaltime << "秒！" << endl;



}
*/
//
//  main.cpp
//  fdsafdsa
//
//  Created by 周瑞星 on 2017/7/16.
//  Copyright © 2017年 周瑞星. All rights reserved.
//
/*
#include <iostream>
#include <cmath>
#include<time.h>
#define PI 3.1415926
#define T 0.1 //读取周期
using namespace std;
int main() {
clock_t start, finish;
double totaltime;
start = clock();



int t = 3;
float a, b, c, a1, b1, c1;
float al1, al2, be1, be2, th1, th2;
float al, be, th;
//a1,b1,c1是测出的三向加速度，al1,be1,th1是校准的角度值，al2,be2,th2是每一时刻的角度值，a,b,c,是转化为校准方向的三向加速度，vx,vy,vz是校准方向的三向速度，xyz是....的三向位移。


a1 = 0, b1 = t, c1 = t;
al1 = 0, be1 = 0, th1 = 0;
al2 = PI / 4, be2 = 0, th2 = -PI / 2;
//这里的数据是我随便给的。


al = al1 - al2;
be = be1 - be2;
th = th1 - th2;
a = cos(be)*cos(th)*a1 - (sin(al)*cos(be)*cos(th) + cos(al)*sin(th))*b1 + (sin(al)*sin(th) - cos(al)*sin(be)*cos(th))*c1;
b = cos(be)*sin(th)*a1 + (cos(al)*cos(th) - sin(th)*sin(al)*sin(be))*b1 - (cos(th)*sin(al) + sin(th)*cos(al)*sin(be))*c1;
c = sin(be)*a1 + sin(al)*cos(be)*b1 + cos(al)*cos(be)*c1;
//以上是转化

static float vx1 = 0;
static float vy1 = 0;
static float vz1 = 0;
static float vx2 = 0;
static float vy2 = 0;
static float vz2 = 0;
float i;
i = vx2, vx2 = vx1 + a*T, vx1 = i;
i = vy2, vy2 = vy1 + b*T, vy1 = i;
i = vz2, vz2 = vz1 + c*T, vz1 = i;

static float x1 = 0;
static float y1 = 0;
static float z1 = 0;
static float x2 = 0;
static float y2 = 0;
static float z2 = 0;
i = x2, x2 = x1 + (vx1 + vx2)*0.5*T, x1 = i;
i = y2, y2 = y1 + (vy1 + vy2)*0.5*T, y1 = i;
i = z2, z2 = z1 + (vz1 + vz2)*0.5*T, z1 = i;
//以上是积分

cout << a << endl;
cout << vx2 << endl;
cout << x2 << endl;
//输出验证


finish = clock();
totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
cout << "\n此程序的运行时间为" << totaltime << "秒！" << endl;



}
*/