#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "fileprocessor.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

const bool INPUT_DELETE = true;
const bool INPUT_KEEP = false;
const bool OUTPUT_OVERRIDE = true;
const bool OUTPUT_COPY = false;
const bool START_ONCE = false;
const bool START_TIMER = true;

struct FileProcessorOptions {
    bool input;
    bool output;
    bool start;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_startButton_pressed();
    void updateProgress(int progressPercent, qint64 msecToNextStart);
    void clearProcessor();
    void on_inputBrowseButton_pressed();
    void on_outputBrowseButton_pressed();
    void on_inputDeleteRadio_pressed();
    void on_inputKeepRadio_pressed();
    void on_outputOverrideRadio_pressed();
    void on_outputCopyRadio_pressed();
    void on_startOnceRadio_pressed();
    void on_startTimerRadio_pressed();
    void on_timerResetButton_pressed();

private:
    FileProcessorOptions options;
    Ui::MainWindow *ui;
    FileProcessor *fileProcessor;
};
#endif // MAINWINDOW_H
