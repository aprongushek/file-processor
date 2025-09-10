#include "fileprocessor.h"

#include <QThread>

FileProcessor::FileProcessor(bool _isRepeating, QTime _time)
    : isRepeating(_isRepeating)
{
    unsigned long int hour = _time.hour() * 60 * 60 * 1000;
    unsigned long int minute = _time.minute() * 60 * 1000;
    unsigned long int second = _time.second() * 1000;
    time = hour + minute + second;
}

void FileProcessor::manage()
{
    processFiles();
    if (isRepeating) {
        unsigned int msec = 0;
        while (isRepeating) {
            QThread::msleep(500);
            msec += 500;
            emit progress(0, time - msec);
            if (msec >= time) {
                processFiles();
                msec = 0;
            }
        }
    }
    emit finished();
}

void FileProcessor::processFiles()
{
  for (int i = 0; i <= 100; ++i) {
        QThread::msleep(50);
        emit progress(i, time);
    }
}
