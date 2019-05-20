#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <future>
#include <vector>
#include <functional>
#include <unistd.h>
using namespace std;
using namespace std::placeholders; 
class ThreadPool
{
public:
	using Task = function<void()>;

	ThreadPool(int num);
	~ThreadPool();
	void enqueue(Task &&task);

private:
	void worker();
	vector<thread>pool;
	queue<Task>task_queue;
	int nums;
	mutex queue_mtx;
	condition_variable  condition;
	bool stop;
};

ThreadPool::ThreadPool(int num)
{
	nums = num;
	stop = false;
	for(int i = 0; i < num; i++)
	{
		pool.emplace_back(bind(&ThreadPool::worker,this));
	}
}

ThreadPool::~ThreadPool()
{
	{
		unique_lock<mutex>lock(queue_mtx);
		stop = true;
	}
	condition.notify_all();
	for(auto &i:pool)
		i.join();
}

void ThreadPool::worker()
{
	cout<<"Create worker Thread: "<<this_thread::get_id()<<endl;
	while(!stop)
	{
		Task task;
		{
			unique_lock<mutex>lock(queue_mtx);
			condition.wait(lock);
			if(this->stop && task_queue.empty())
			{
				cout<<"exit "<<this_thread::get_id()<<endl;
				return;
			}
			task = task_queue.front();
			task_queue.pop();
		}
		task();
	}
}

void ThreadPool::enqueue(Task && task)
{
	{
		unique_lock<mutex>lock(queue_mtx);
		task_queue.push(move(task));
	}
	condition.notify_one();
}

void func(int i)
{
	cout<<"I am :"<<this_thread::get_id()<<"\t"<<i<<endl;
}

int main()
{
	cout<<"Main Thread Start   :"<<this_thread::get_id()<<endl;
	ThreadPool tp(8);
	sleep(1);

	for(int i = 0; i < 30; i++)
	{
		tp.enqueue(bind(func,i));
		sleep(1);
	}
}
