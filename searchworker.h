#ifndef SEARCHWORKER_H
#define SEARCHWORKER_H

#include <QObject>
#include <QString>
#include <QRegularExpression>
#include <QFile>
#include <QTextStream>
#include <QThread>

class SearchWorker : public QObject
{
    Q_OBJECT

public:
    SearchWorker(const QString& filePath, const QString& searchString, QObject* parent = nullptr);
    ~SearchWorker();

public slots:
    void run();

signals:
    void searchFinished(const QString& filePath, int numOccurrences, QThread* thread);

private:
    QString filePath;
    QString searchString;
};

#endif // SEARCHWORKER_H
