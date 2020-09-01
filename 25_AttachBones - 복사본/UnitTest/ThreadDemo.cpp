#include "stdafx.h"
#include "ThreadDemo.h"

void ThreadDemo::Initialize()
{
	//Loop();
	//MultiThread();

	//Mutex();

	/*Performance performence;
	performence.Start();
	{
		RaceCondition();
	}
	float t = performence.End();
	printf("t : %f\n", t);*/


	ExecTimer();
}

void ThreadDemo::Update()
{
	m.lock();
	{
		progress += 0.1f;
		ImGui::ProgressBar(progress / 1000.0f);
	}
	m.unlock();
}

void ThreadDemo::Loop()
{
	for (int i = 0; i < 100; i++)
		printf("1 : %d\n", i);
	printf("반복문 1 종료\n");

	for (int i = 0; i < 100; i++)
		printf("2 : %d\n", i);
	printf("반복문 2 종료\n");
}

void ThreadDemo::Loop1()
{
	for (int i = 0; i < 100; i++)
		printf("1 : %d\n", i);
	printf("반복문 1 종료\n");
}

void ThreadDemo::Loop2()
{
	for (int i = 0; i < 10000; i++)
		printf("2 : %d\n", i);
	printf("반복문 2 종료\n");
}

void ThreadDemo::MultiThread()
{
	thread t(bind(&ThreadDemo::Loop1, this));
	thread t2(bind(&ThreadDemo::Loop2, this));

	t2.join();
	printf("t2.join\n");

	t.join();
	printf("t.join\n");
}

void ThreadDemo::Mutex()
{
	thread t([&]()
	{
		while (true)
		{
			Sleep(1000);

			m.lock();
			{
				printf("Progress : %f\n", progress);
			}			
			m.unlock();
		}
	});
	t.detach();
}

void ThreadDemo::Racer(int & counter)
{
	for (int i = 0; i < 1e+6; i++)
	{
		m.lock();
		{
			counter++;
		}
		m.unlock();
	}
}

void ThreadDemo::RaceCondition()
{
	int counter = 0;
	vector<thread> workers;
	for (int i = 0; i < 4; i++)
	{
		function<void(int&)> f = bind(&ThreadDemo::Racer, this, placeholders::_1);
		workers.push_back(thread(f, ref(counter)));
	}

	for (int i = 0; i < 4; i++)
		workers[i].join();

	printf("Counter : %d\n", counter);
}

void ThreadDemo::ExecTimer()
{
	timer.Start([]()
	{
		printf("Timer\n");
	}, 2000, 2);

	timer2.Start([]()
	{
		printf("Timer2\n");
	}, 3000);
}
