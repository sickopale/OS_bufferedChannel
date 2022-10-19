#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>


template<class T>
class bufferedChannel {

private:
	int buffer_Size;
	bool closeFlag;
	std::mutex mutex;
	std::queue<T> queue;
	std::condition_variable var;
public:

	explicit bufferedChannel(int size)
	{
		buffer_Size = size;
		closeFlag = false;
	}

	void send(T value) {

		if (closeFlag) {
			throw new std::runtime_error("Chanel is closed!");
		}

		std::unique_lock<std::mutex> lock(mutex);
		var.wait(lock, [&] {
			return queue.size() < buffer_Size; 
			});
		std::cout <<"Sending: "<< value << "\n";
		queue.push(value);
		var.notify_one();
	}

	std::pair<T, bool> recv() {

		std::unique_lock<std::mutex> lock(mutex);
		var.wait(lock, [&] {
			return !queue.empty();
			});
		T res = queue.front();
		std::cout << res << "\n";
		queue.pop();
		var.notify_one();
		return std::make_pair(res,true);
	}
	
	void Close() {
		closeFlag = true;
		var.notify_all();
	}

};