#include "textsearch.h"
#include "ui_textsearch.h"

TextSearch::TextSearch(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TextSearch)
{
    ui->setupUi(this);
    setWindowTitle("Поиск текста в файлах");

    connect(ui->browseButton, &QPushButton::clicked, this, &TextSearch::browseDirectory);
    connect(ui->startButton, &QPushButton::clicked, this, &TextSearch::startSearch);

    connect(ui->directoryLineEdit, &QLineEdit::textChanged, this, &TextSearch::checkMandatoryData);
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &TextSearch::checkMandatoryData);

    ui->threadSpinBox->setRange(1, QThread::idealThreadCount());

    checkMandatoryData();
}

TextSearch::~TextSearch()
{
    delete ui;
    delete worker;
    delete task;
    delete filePathItem;
    delete numOccurrencesItem;
}

void TextSearch::browseDirectory()
{
    setStatus("Ожидание");

    QString directory = QFileDialog::getExistingDirectory(this, "Выберите каталог");
    QStringList filters;
    filters << "*.txt";
    QDirIterator it(directory, filters, QDir::Files);
    QStringList files;
    while (it.hasNext())
    {
        it.next();
        files.append(it.filePath());
    }

    if (files.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Нет файлов для сканирования.");
        return;
    }

    if (!directory.isEmpty())
        ui->directoryLineEdit->setText(directory);

    checkMandatoryData();
}

void TextSearch::setStatus(const QString &statusText)
{
    ui->statusLabel->setText(statusText);
}

void TextSearch::setControlsEnabled(bool enabled)
{
    ui->startButton->setEnabled(enabled);
    ui->browseButton->setEnabled(enabled);
    ui->directoryLineEdit->setEnabled(enabled);
    ui->searchLineEdit->setEnabled(enabled);
    ui->threadSpinBox->setEnabled(enabled);
}

void TextSearch::checkMandatoryData()
{
    QString directory = ui->directoryLineEdit->text();
    QString searchString = ui->searchLineEdit->text();

    if (directory.isEmpty() || searchString.isEmpty())
        ui->startButton->setEnabled(false);
    else
        ui->startButton->setEnabled(true);
}

void TextSearch::startSearch()
{
    QString directory = ui->directoryLineEdit->text();
    QString searchString = ui->searchLineEdit->text();
    int numThreads = ui->threadSpinBox->value();

    if (directory.isEmpty() || searchString.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля.");
        checkMandatoryData();
        return;
    }

    setStatus("Поиск");
    ui->resultTable->setRowCount(0);
    setControlsEnabled(false);

    QThreadPool* threadPool = QThreadPool::globalInstance();
    threadPool->setMaxThreadCount(numThreads);

    QDirIterator it(directory, QStringList() << "*.txt", QDir::Files);
    while (it.hasNext())
     {
        QString filePath = it.next();
        worker = new SearchWorker(filePath, searchString, this);
        task = new SearchTask(worker);
        connect(worker, &SearchWorker::searchFinished, this, &TextSearch::handleSearchFinished);
        threadPool->start(task);
     }
    setStatus("Поиск окончен");
    setControlsEnabled(true);
}

void TextSearch::handleSearchFinished(const QString& filePath, int numOccurrences, QThread* thread)
{
    int row = ui->resultTable->rowCount();
    ui->resultTable->insertRow(row);

    QFileInfo fileInfo(filePath);
    filePathItem = new QTableWidgetItem(fileInfo.fileName());
    numOccurrencesItem = new QTableWidgetItem(QString::number(numOccurrences));

    ui->resultTable->setItem(row, 0, filePathItem);            // Колонка с именем файла
    ui->resultTable->setItem(row, 1, numOccurrencesItem);       // Колонка с количеством вхождений
    ui->resultTable->resizeColumnsToContents();

    filePathItem->setFlags(filePathItem->flags() & ~Qt::ItemIsEditable);
    numOccurrencesItem->setFlags(numOccurrencesItem->flags() & ~Qt::ItemIsEditable);

    qDebug() << "Поток:" << thread << "обработал файл:" << fileInfo.fileName();
}

