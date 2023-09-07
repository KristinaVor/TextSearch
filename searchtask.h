#ifndef SEARCHTASK_H
#define SEARCHTASK_H

#include <QRunnable>

#include "searchworker.h"

class SearchTask : public QRunnable
{
public:
    SearchTask(SearchWorker* worker);
    ~SearchTask();

    void run() override;

private:
    SearchWorker* worker;
};

#endif // SEARCHTASK_H
