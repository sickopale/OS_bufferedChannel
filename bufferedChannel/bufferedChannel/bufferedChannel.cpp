#include <iostream>
#include "bufferedChannel.h"


void sendTest(bufferedChannel<int>& channel)
{
	channel.send(2323);
	channel.send(5345);
	channel.send(46454);
	channel.send(24123);
	channel.send(5395995);
}

void recvTest(bufferedChannel<int>& channel)
{
	channel.recv();
	channel.recv();
	channel.recv();
	channel.recv();
	channel.recv();
}

int main() {
	bufferedChannel<int> channel(5);
	std::vector<std::thread> threads;
	for (int i = 0; i < 5; ++i) {
		threads.emplace_back(sendTest, std::ref(channel));
		threads.emplace_back(recvTest, std::ref(channel));
	}
	for (auto& thread : threads) { thread.join(); }
	return 0;
}
