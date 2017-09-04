#include "GeometryGenerationScheduler.h"

GeometryGenerationScheduler::GeometryGenerationScheduler()
    : runningThreads(0), isAlive(true)
{
}

void GeometryGenerationScheduler::StartThreads(int maxWorkerThreads)
{
    runningThreads = maxWorkerThreads; // Decremented by 1 by each thread started.
    for (int i = 0; i < maxWorkerThreads; i++)
    {
        workerThreads.push_back(std::thread(&GeometryGenerationScheduler::ProcessGeometryTask, this));
    }
}

void GeometryGenerationScheduler::ProcessGeometryTask()
{
    while (isAlive)
    {
        std::unique_lock<std::mutex> uniqueLock(taskQueueMutex);
        if (geometryGenerationTasks.empty())
        {
            // Unlock while we wait for jobs, locking when we return.
            --runningThreads;
            threadSleepCondition.wait(uniqueLock);
            ++runningThreads;
        }

        // We may be unblocked spuriously, as per C++ 11 docs. 
        if (isAlive && !geometryGenerationTasks.empty())
        {
            GeometryGenerationTaskData taskData = geometryGenerationTasks.front();
            geometryGenerationTasks.pop_front();

            // Ensure we don't hold the mutex while running our jobs.
            uniqueLock.unlock();

            taskData.geometryGenerator->GenerateGeometry(taskData.geometryGenerationData, taskData.geometryToPopulate);
            taskData.geometryToPopulate->SetAsGenerated();
        }
    }
}

int GeometryGenerationScheduler::AddGeometryGenerationTask(IGeometryGenerator* geometryGenerator, GeometryGenerationData* geometryGenerationData, Geometry* geometryToPopulate)
{
    // Queue
    GeometryGenerationTaskData geometryGenerationTaskData;
    geometryGenerationTaskData.geometryGenerator = geometryGenerator;
    geometryGenerationTaskData.geometryGenerationData = geometryGenerationData;
    geometryGenerationTaskData.geometryToPopulate = geometryToPopulate;

    taskQueueMutex.lock();
    geometryGenerationTasks.push_back(geometryGenerationTaskData);
    int tasksQueued = (int)geometryGenerationTasks.size();
    taskQueueMutex.unlock();
    
    // If threads are stopped, ping one. Has no effect if all are running
    threadSleepCondition.notify_one();
    return tasksQueued;
}

int GeometryGenerationScheduler::GetGeometryJobCount()
{
    std::unique_lock<std::mutex> uniqueLock(taskQueueMutex);
    return (int)geometryGenerationTasks.size() + runningThreads;
}

GeometryGenerationScheduler::~GeometryGenerationScheduler()
{
    // Stop all geometry generation threads, waiting for generation to complete.
    isAlive = false;
    threadSleepCondition.notify_all();
    for (std::thread& workerThread : workerThreads)
    {
        workerThread.join();
    }
}
