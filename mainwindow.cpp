#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , fileProcessor(nullptr)
{
    ui->setupUi(this);
    options = {INPUT_KEEP, OUTPUT_OVERRIDE, START_ONCE};
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_startButton_pressed()
{
    if (!fileProcessor) {
        ui->startButton->setEnabled(false);
        QThread *thread = new QThread;
        fileProcessor = new FileProcessor(options.input, options.output, options.start,
                                          ui->startTimeEdit->time(), ui->inputDirLineEdit->text(),
                                          ui->inputMaskLineEdit->text(), ui->outputDirLineEdit->text(),
                                          ui->xorKeyLineEdit->text().toInt());
        fileProcessor->moveToThread(thread);

        connect(thread, &QThread::started, fileProcessor, &FileProcessor::manage);
        connect(fileProcessor, &FileProcessor::finished, thread, &QThread::quit);
        connect(fileProcessor, &FileProcessor::finished, fileProcessor, &FileProcessor::deleteLater);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);

        connect(fileProcessor, &FileProcessor::progress, this, &MainWindow::updateProgress);
        connect(fileProcessor, &FileProcessor::finished, this, &MainWindow::clearProcessor);

        thread->start();
    }
}

void MainWindow::updateProgress(int progressPercent, qint64 msecToNextStart)
{
    ui->progressBar->setValue(progressPercent);
    qint64 hour = msecToNextStart / 1000 / 60 / 60;
    msecToNextStart -= hour * 60 * 60 * 1000;
    qint64 minute = msecToNextStart / 1000 / 60;
    msecToNextStart -= minute * 60 * 1000;
    qint64 second = msecToNextStart / 1000;
    QTime time(hour, minute, second);
    ui->timerLabel->setText("Next start in: " + time.toString("hh:mm:ss"));
}

void MainWindow::clearProcessor()
{
    fileProcessor = nullptr;

    ui->progressBar->setValue(0);
    ui->startButton->setEnabled(true);
    if (ui->startOnceRadio->isChecked())
        ui->timerLabel->setText("Timer is not set");
    else
        ui->timerLabel->setText("Timer will be set on start");
}

void MainWindow::on_inputBrowseButton_pressed()
{
    QString str = QFileDialog::getExistingDirectory(0, "Choose input directory", "");
    ui->inputDirLineEdit->setText(str);
}


void MainWindow::on_outputBrowseButton_pressed()
{
    QString str = QFileDialog::getExistingDirectory(0, "Choose output directory", "");
    ui->outputDirLineEdit->setText(str);
}


void MainWindow::on_inputDeleteRadio_pressed()
{
    options.input = INPUT_DELETE;
}


void MainWindow::on_inputKeepRadio_pressed()
{
    options.input = INPUT_KEEP;
}


void MainWindow::on_outputOverrideRadio_pressed()
{
    options.output = OUTPUT_OVERRIDE;
}


void MainWindow::on_outputCopyRadio_pressed()
{
    options.output = OUTPUT_COPY;
}


void MainWindow::on_startOnceRadio_pressed()
{
    options.start = START_ONCE;
    ui->startTimeEdit->setEnabled(false);
    ui->startTimeEdit->setTime(QTime(0, 0, 0));
    if (!fileProcessor)
        ui->timerLabel->setText("Timer is not set");
}


void MainWindow::on_startTimerRadio_pressed()
{
    options.start = START_TIMER;
    ui->startTimeEdit->setEnabled(true);
    if (!fileProcessor)
        ui->timerLabel->setText("Timer will be set on start");
}


void MainWindow::on_timerResetButton_pressed()
{
    ui->startTimeEdit->setTime(QTime(0, 0, 0));

    if (fileProcessor)
        fileProcessor->isRepeating = false;
}

