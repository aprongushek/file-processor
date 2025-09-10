#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <QObject>
#include <QTime>
#include <Qstring>

class FileProcessor : public QObject
{
    Q_OBJECT
public:
    FileProcessor(bool _deleteInput, bool _overrideOutput, bool _isRepeating, QTime _time,
                  QString _inputDir, QString _inputMask, QString _outputDir, qint8 _xorKey);

    bool isRepeating;

public slots:
    void manage();

signals:
    void progress(int progressPercent, qint64 msecToNextStart);
    void finished();

private:
    void processFiles();
    void processOneFile(QString inputPath, QString outputPath);
    QString generateUniquePath(QString originalPath);
    QStringList findFilesByMask(QString directoryPath, QString mask);
    qint64 getFileSize(QString filePath);

    bool deleteInput;
    bool overrideOutput;

    qint64 msecToNextStart;
    QString inputDir;
    QString inputMask;
    QString outputDir;
    qint8 xorKey;

    QStringList inputFilesPaths;
    QStringList outputFilesPaths;
    qint64 totalSize;
    qint64 processedSize;
};

#endif // FILEPROCESSOR_H
