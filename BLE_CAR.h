#ifndef BLE_CAR_H_INCLUDED
#define BLE_CAR_H_INCLUDED

#include"BLE_PORT.h"

class car
{
	ComPort com;
public:
	car():com("\\\\.\\COM3")
	{
	}
	void turnl()
	{
		com.Send('L');
	}
	void turnr()
	{
		com.Send('R');
	}
	void run()
	{
		com.Send('A');
	}
	void stop()
	{
		com.Send('P');
	}
	void back()
	{
		com.Send('B');
	}
};


#endif // BLE_CAR_H_INCLUDED
