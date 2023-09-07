#ifndef TEXTSEARCH_H
#define TEXTSEARCH_H

#include <QMainWindow>
#include <QtWidgets>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDirIterator>
#include <QThreadPool>

#include "searchworker.h"
#include "searchtask.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TextSearch; }
QT_END_NAMESPACE

class TextSearch : public QMainWindow
{
    Q_OBJECT

public:
    TextSearch(QWidget *parent = nullptr);
    ~TextSearch();

public slots:
    void handleSearchFinished(const QString& filePath, int numOccurrences, QThread* thread);

private slots:
    void browseDirectory();
    void startSearch();
    void checkMandatoryData();

private:
    Ui::TextSearch *ui;
    SearchWorker* worker;
    SearchTask* task;
    QTableWidgetItem* filePathItem;
    QTableWidgetItem* numOccurrencesItem;

    void setControlsEnabled(bool enabled);
    void setStatus(const QString &statusText);


};
#endif // TEXTSEARCH_H
