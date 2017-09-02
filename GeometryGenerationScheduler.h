#pragma once
#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>
#include <vector>
#include <glm\vec3.hpp>
#include "IGeometryGenerator.h"
#include "Geometry.h"

struct GeometryGenerationTaskData
{
    IGeometryGenerator* geometryGenerator;
    glm::ivec3 geometryId;
    Geometry* geometryToPopulate;
};

// Schedules generation of geometry on worker threads.
// We use a series of worker threads to not starve our main CPU and avoid the overhead of thread spawning for new jobs.
class GeometryGenerationScheduler
{
    int runningThreads;
    std::vector<std::thread> workerThreads;
    std::deque<GeometryGenerationTaskData> geometryGenerationTasks;

    bool isAlive;
    std::mutex taskQueueMutex;
    std::condition_variable threadSleepCondition;

    // Called by each thread to process geometry.
    void ProcessGeometryTask();

public:
    GeometryGenerationScheduler();
    void StartThreads(int maxWorkerThreads);

    // Adds a geometry generation task, returning the number of items queued for generation.
    int AddGeometryGenerationTask(IGeometryGenerator* geometryGenerator, glm::ivec3 geometryId, Geometry* geometryToPopulate);
    
    // Returns the number of jobs queued and running.
    int GetGeometryJobCount();
    ~GeometryGenerationScheduler();
};

