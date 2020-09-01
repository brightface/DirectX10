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
	printf("�ݺ��� 1����\n");
	for (int i = 0; i < 100; i++) {
		printf("2: %d\n", i);
	}
	printf("�ݺ��� 2����\n");
}

void ThreadDemo::Loop1()
{
	for (int i = 0; i < 100; i++) {
		printf("1: %d\n", i);
	}
	printf("�ݺ��� 1����\n");
}

void ThreadDemo::Loop2()
{
	for (int i = 0; i < 100; i++) {
		printf("2: %d\n", i);
	}
	printf("�ݺ��� 2����\n");
}

void ThreadDemo::MultiThread()
{
	thread t(bind(&ThreadDemo::Loop1, this)); //�Լ� ������ ���鋚 bind ��������
	thread t2(bind(&ThreadDemo::Loop2, this));
	//������ ����ȭ
	//������ ����
	//������ �Ҵ�Ǵ� �ð��� �ֱ⶧���� ������ ���� ���̽� �����
	t2.join();
	printf("t2.join\n");
	//t1�� ������ ���εȴ�.
	t.join();
	printf("t.join\n");


}

void ThreadDemo::Mutex()
{
	//= �����ϰڴ�. 
	thread t([&]()
	{	//�����ϸ� �ȹٲ�, this�� �ȹٲ� ���� *�� �ٷ��.
		
		while (true) {
			Sleep(1000);//1�ʵ��� ���ͷ�Ʈ ������� ����

			//������ �ȿ� ������� �ش� �����尡 ���ߴ°ž�
				//�������� ���������� ������ ��
			m.lock();
			{
				//progress += 0.05f;
				printf("Progress: %f\n", progress);
			}
			m.unlock(); //�̰� ������ ���ÿ� �����ϴ� ������ ��������
			//���� ���ذ� ������ �� �����o�� ������ �˼��� ����
		}	
		//progress = 10;
	});
	t.detach();//������ �ǵ� ���� �Ʊ� ������ �������� ���°Ű� �ڱⰡ �ڱ� ������ ������ �ڵ������̴�.

}

void ThreadDemo::Racer(int& counter)
{
	for (int i = 0; i < 1000; i++)
	{
		//m.lock(); ���� �ǻ��·� �ϸ� �� ����
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
		//�Լ������� ��� ������ ����? �Ф� ��
		function<void(int&)> f = bind(&ThreadDemo::Racer, this, placeholders::_1);
		workers.push_back(thread(f, ref(counter))); //�͸� �޼ҵ� ���̾� ptr ������ ����
	}

	for (int i = 0; i < 4; i++)
	{
		workers[i].join();
	}
	printf("Counter :%d\n", counter);
}
void ThreadDemo::ExecTimer()
{
	timer.Start([]() //���ٽ�
	{
		printf("Timer\n");
	}, 2000.2); //2�ʸ��� 2�������ض�
	//timer Ŭ���� �м��ض�
	timer2.Start([]()
	{
		printf("Timer2\n");
	}, 3000); //0�̸� ���ѹݺ��ض�

	//getTicount �ϴ� ���� �ý��ۿ� ���� �����쿡 ���ӵǾ��ִ�.
	//������ �������� 
	//time getTime // unsigned int �� �ʷ�ġ�� 3�� �̸� ����
	//���� �۸�ս� int64�� 71�ϱ��� �ߵ��� �ִ�.

}

