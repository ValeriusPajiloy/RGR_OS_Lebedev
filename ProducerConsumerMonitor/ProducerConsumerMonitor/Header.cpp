#include "Header.h"
#include <condition_variable>
#include <windows.h>
#include <conio.h>
#include <random>
#include <time.h>
#include <iostream>

using namespace std;
Monitor::Monitor()
{
	count = false;
	head = count = 0;
}

void Monitor::put(int _data)
{
	lock.Acquire();
	if ((count+1)%BufSize == head) full.wait(lock);
	data[count] = _data;
	count = (count + 1) % BufSize;
	empty.signal();
	lock.Release();
}

void Monitor::get(int& _data)
{
	lock.Acquire();
	if (count==head) empty.wait(lock);
	_data = data[head];
	head = (head + 1) % BufSize;
	full.signal();
	lock.Release();
}

void reader(void* pParams)
{
	while (true)
	{
		int data_;
		monitor.get(data_);
		
		EnterCriticalSection(&cs);
		cout << "Readed data: " << data_ << endl;
		cout.flush();
		LeaveCriticalSection(&cs);

		Sleep(40);
	}
}

void writer(void* pParams)
{
	while (true)
	{
		int data_new = rand() % 10;
		
		monitor.put(data_new);

		EnterCriticalSection(&cs);
		cout << "We can write data " << data_new << endl;
		cout.flush();
		LeaveCriticalSection(&cs);

		Sleep(80);
	}
}
