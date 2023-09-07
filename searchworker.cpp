#include "searchworker.h"

SearchWorker::SearchWorker(const QString& filePath, const QString& searchString, QObject* parent)
    : QObject(parent), filePath(filePath), searchString(searchString)
{
}

SearchWorker::~SearchWorker()
{
}

void SearchWorker::run()
{
    QThread* currentThread = QThread::currentThread();

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        in.setCodec("UTF-8");

        QRegExp rx(searchString, Qt::CaseInsensitive);
        QString text; // Для объединения строк
        int numOccurrences = 0;

        while (!in.atEnd())
        {
            QString line = in.readLine();
            text += line + "\n";
        }

        int pos = 0;
        while ((pos = rx.indexIn(text, pos)) != -1)
        {
            numOccurrences++;
            pos += rx.matchedLength();
        }

        file.close();
        emit searchFinished(filePath, numOccurrences, currentThread);
    }
}



