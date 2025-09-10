#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <QObject>
#include <QTime>

class FileProcessor : public QObject
{
    Q_OBJECT
public:
    FileProcessor(bool _isRepeating, QTime _time);

    bool isRepeating;

public slots:
    void manage();

signals:
    void progress(int progressPercent, unsigned long int msecToNextStart);
    void finished();

private:
    void processFiles();

    unsigned long int time;
};

#endif // FILEPROCESSOR_H
