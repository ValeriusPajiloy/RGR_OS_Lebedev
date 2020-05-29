#include <condition_variable>
#include <windows.h>
#include <conio.h>
#include <random>
#include <time.h>
#include <iostream>

using namespace std;

#define BufSize 5 

class Monitor
{
public:
	Monitor() {count = 0;}

	void put(int _data);
	void get(int& _data);

private:
	int data[BufSize];
	int count;
	condition_variable full, empty;
	mutex m;
};



Monitor monitor;
bool flag = true;

void Monitor::put(int _data)
{
	unique_lock<mutex>lock(m);

	if (count >= BufSize)
	{
		full.wait(lock);
	}
	data[count] = _data;
	cout << endl << "Producer put: " << _data << endl;
	count = count + 1;
	if (count == BufSize) cout << "Buffer is full" << endl;
	
	cout << "count: " << count << endl;
	cout << "Buffer: ";
	for (int i = 0; i < count; i++)
	{
		cout << data[i] << " ";
	}
	cout << endl << endl;

	if(count==1) empty.notify_one();

}

void Monitor::get(int& _data)
{
	unique_lock<mutex>lock(m);
	
	if (count == 0)
	{
		empty.wait(lock);
	}
	_data = data[count-1];
	cout << endl << "Consumer get: " << _data << endl;
	count = count - 1;
	if (count == 0) 	cout << "Buffer is empty" << endl;

	cout << "count: " << count << endl;
	cout << "Buffer: ";
	for (int i = 0; i < count; i++)
	{
		cout << data[i] << " ";
	}
	cout << endl << endl;

	if(count == BufSize-1) full.notify_one();

}

void Consumer(void* number)
{
	srand(time(0));
	while (flag)
	{
		int data_;

		monitor.get(data_);

		int n = rand() % 3 + 1;
		Sleep(n * 100);
	}
}

void Producer(void* number)
{
	srand(time(0));
	while (flag)
	{
		int data_new = rand() % 10 ;

		monitor.put(data_new);

		int n = rand() % 3 + 1;
		Sleep(n * 100);
	}
}


int main()
{
	string str;
	srand(time(0));

	HANDLE ThreadConsumer;
	DWORD dwThreadConsumer;

	HANDLE ThreadProducer;
	DWORD dwThreadProducer;
	   

	ThreadProducer = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Producer, NULL, 0, &dwThreadProducer);
	ThreadConsumer = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Consumer, NULL, 0, &dwThreadConsumer);
	if (ThreadProducer == NULL || ThreadConsumer == NULL)
	{
		cout << "Error of creating new thread..." << endl;
		return GetLastError();
	}

	_getch();
	flag = false;
	CloseHandle(ThreadProducer);
	CloseHandle(ThreadConsumer);
	cout << "Successful completion" << endl;

	return 0;
}