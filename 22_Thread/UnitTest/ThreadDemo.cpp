#include "stdafx.h"
#include "ThreadDemo.h"

void ThreadDemo::Initialize()
{
	//Loop();
	//MultiThread();
	/*Mutex();

	Performance performence;
	performence.Start();
	{
		RaceCondition();
	}
	float t = performence.End();
	printf("t:%f\n,", t);
*/
	ExecTimer();
}


void ThreadDemo::Update()
{
	m.lock();
	{
		progress += 0.1f;
		ImGui::ProgressBar(progress/1000);
	}
	m.unlock();

}


void ThreadDemo::Loop()
{
	for (int i = 0; i < 100; i++) {
		printf("1: %d\n", i);
	}
	printf("반복문 1종료\n");
	for (int i = 0; i < 100; i++) {
		printf("2: %d\n", i);
	}
	printf("반복문 2종료\n");
}

void ThreadDemo::Loop1()
{
	for (int i = 0; i < 100; i++) {
		printf("1: %d\n", i);
	}
	printf("반복문 1종료\n");
}

void ThreadDemo::Loop2()
{
	for (int i = 0; i < 100; i++) {
		printf("2: %d\n", i);
	}
	printf("반복문 2종료\n");
}

void ThreadDemo::MultiThread()
{
	thread t(bind(&ThreadDemo::Loop1, this)); //함수 포인터 만들떄 bind 먼저하지
	thread t2(bind(&ThreadDemo::Loop2, this));
	//쓰레드 동기화
	//순서를 제어
	//쓰레드 할당되는 시간이 있기때문에 문제가 생겨 레이스 컨디션
	t2.join();
	printf("t2.join\n");
	//t1이 끝나야 조인된다.
	t.join();
	printf("t.join\n");


}

void ThreadDemo::Mutex()
{
	//= 대입하겠다. 
	thread t([&]()
	{	//대입하면 안바껴, this도 안바껴 따라서 *로 다룬다.
		
		while (true) {
			Sleep(1000);//1초동안 인터럽트 응답없음 뜨지

			//쓰레드 안에 들어있지 해당 쓰레드가 멈추는거야
				//나머지는 정상적으로 동작을 해
			m.lock();
			{
				//progress += 0.05f;
				printf("Progress: %f\n", progress);
			}
			m.unlock(); //이게 없으면 동시에 접근하는 순간에 터져버려
			//깊은 이해가 없으면 왜 터졌늨가 이유를 알수가 없어
		}	
		//progress = 10;
	});
	t.detach();//조인이 되든 말든 아까 조인은 끝났을때 쓰는거고 자기가 자기 조인이 끝나면 자동조인이다.

}

void ThreadDemo::Racer(int& counter)
{
	for (int i = 0; i < 1000; i++)
	{
		//m.lock(); 락을 건상태로 하면 잘 나와
		{
			counter += 1;
		}
		//m.unlock();
	}
}

void ThreadDemo::RaceCondition()
{
	int counter = 0;
	vector<thread> workers;
	for (int i = 0; i < 4; i++) {
		//함수포인터 어떻게 쓰는지 알지? ㅠㅠ 네
		function<void(int&)> f = bind(&ThreadDemo::Racer, this, placeholders::_1);
		workers.push_back(thread(f, ref(counter))); //익명 메소드 식이야 ptr 포인터 붙임
	}

	for (int i = 0; i < 4; i++)
	{
		workers[i].join();
	}
	printf("Counter :%d\n", counter);
}
void ThreadDemo::ExecTimer()
{
	timer.Start([]() //람다식
	{
		printf("Timer\n");
	}, 2000.2); //2초마다 2번실행해라
	//timer 클래스 분석해라
	timer2.Start([]()
	{
		printf("Timer2\n");
	}, 3000); //0이면 무한반복해라

	//getTicount 일단 못써 시스템에 못써 윈도우에 종속되어있다.
	//윈도우 느려질때 
	//time getTime // unsigned int 형 초로치면 3일 이면 끝나
	//쿼리 퍼모먼스 int64형 71일까지 견딜수 있다.

}

