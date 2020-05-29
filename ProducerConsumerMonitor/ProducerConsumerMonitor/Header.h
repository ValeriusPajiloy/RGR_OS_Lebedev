#pragma once
#ifndef HEADER_H
#define HEADER_H

#define BufSize 5

CRITICAL_SECTION cs;

class Monitor
{
public:
	Monitor();
	void put(int _data);
	void get(int& _data);

private:
	int data[BufSize];
	int count, head;
	condition_variable full, empty;
	mutex lock;

};
Monitor monitor;

void reader(void* pParams);
void writer(void* pParams);

#endif