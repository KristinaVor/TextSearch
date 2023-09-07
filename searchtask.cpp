#include "searchtask.h"


SearchTask::SearchTask(SearchWorker* worker) : worker(worker)
{
}

SearchTask::~SearchTask()
{
}

void SearchTask::run()
{
    worker->run();
}
