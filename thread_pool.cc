#include <thread>
#include <functional>
#include <memory>
#include <functional>
#include <atomic>
#include <list>
#include "sync_queue.cc"

const int kMaxTaskCount = 100;

class ThreadPool {
public:
    using Task = std::function<void()>;

	ThreadPool(int num_threads = std::thread::hardware_concurrency()) : queue_(kMaxTaskCount) {
		Start(num_threads);
	}

	~ThreadPool() {
		Stop();
	}

	void Stop() {
		std::call_once(once_flag_, [this]{ StopThreadGroup(); });
	}

	void AddTask(Task&& task) {
		queue_.Put(std::forward<Task>(task));
	}

	void AddTask(const Task& task) {
		queue_.Put(task);
	}
private:
	void Start(int num_threads) {
		running_ = true;
		for (int i = 0; i < num_threads; ++i) {
			thread_group_.push_back(std::make_shared<std::thread>(&ThreadPool::RunInThread, this));
		}
	}

	void RunInThread() {
		while (running_) {
			std::list<Task> list;
			queue_.Take(list);
			for (auto& task : list) {
				if (!running_) {
					return;
				}
				task();
			}
		}
	}

	void StopThreadGroup() {
		queue_.Stop();
		running_ = false;
		for (auto thread : thread_group_) {
			if (thread) {
				thread->join();
			}
		}
		thread_group_.clear();
	}

private:
	std::list<std::shared_ptr<std::thread>> thread_group_;
	SyncQueue<Task> queue_;
	std::atomic_bool running_;
	std::once_flag once_flag_;
};