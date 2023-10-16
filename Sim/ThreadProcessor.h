#pragma once
#include <atomic>
#include <functional>
#include <thread>

enum ThreadStatus {
	FREE,
	RUNNING,
	DONE,
};

// Performs an operation across several CPU threads.
class ThreadProcessor
{
	int seed;

	int threadCount;
	std::atomic<ThreadStatus>* threadStatus; // [] array
	std::thread** threads; // [] array

	void** processors; // [] array

public:
	ThreadProcessor();
	~ThreadProcessor();

	void StartThreads(int width, int height, unsigned char* rgbaData);
	void WaitForThreads();
};