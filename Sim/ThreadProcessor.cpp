#include <iostream>
#include "ThreadProcessor.h"
#include "Random2DFiller.h"

ThreadProcessor::ThreadProcessor() : seed(10) {
	// Support Cores - 2 threads for multiprocessing, with a minimum of 2 cores.
	int minThreads = 2;
	threadCount = std::thread::hardware_concurrency();
	std::cout << "Hardware cores detected: " << threadCount;
	
	threadCount = std::max(threadCount - 2, minThreads);
	std::cout << ". Will use " << threadCount << " threads." << std::endl;

	threadStatus = new std::atomic<ThreadStatus>[threadCount];
	threads = new std::thread* [threadCount];
	processors = (void**)new Random2DFiller* [threadCount];

	for (int i = 0; i < threadCount; i++)
	{
		threadStatus[i].store(ThreadStatus::FREE);
	}
}

void ThreadProcessor::StartThreads(int width, int height, unsigned char* rgbaData)
{
	int columnsPerThread = height / threadCount;
	int currentColumn = 0;
	for (int i = 0; i < threadCount; i++)
	{
		int nextColumn = std::min(currentColumn + columnsPerThread, height);
		if (i == threadCount - 1)
		{
			// Ensure the last thread processes all remining lines to account for rounding discrepancies
			nextColumn = height;
		}

		threadStatus[i].store(ThreadStatus::RUNNING);

		// TODO figure out how to generate and pass in the process function.
		auto filler = new Random2DFiller(12345 + seed);
		seed++;
		threads[i] = new std::thread(&Random2DFiller::Fill, filler, currentColumn, nextColumn, width, rgbaData);
		processors[i] = filler;

		currentColumn = nextColumn;
	}
}

void ThreadProcessor::WaitForThreads() {
	for (int i = 0; i < threadCount; i++)
	{
		threads[i]->join();
		threadStatus[i].store(ThreadStatus::DONE);
		delete threads[i];
		delete processors[i];
	}
}

ThreadProcessor::~ThreadProcessor() {
	delete[] threads;
	delete[] processors;
}