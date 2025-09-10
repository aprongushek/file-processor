#include "fileprocessor.h"

#include <QThread>
#include <QFile>
#include <QFileInfo>
#include <QDir>

FileProcessor::FileProcessor(bool _deleteInput, bool _overrideOutput, bool _isRepeating, QTime _time,
                             QString _inputDir, QString _inputMask, QString _outputDir, qint8 _xorKey)
    : deleteInput(_deleteInput)
    , overrideOutput(_overrideOutput)
    , isRepeating(_isRepeating)
    , inputDir(_inputDir)
    , inputMask(_inputMask)
    , outputDir(_outputDir)
    , xorKey(_xorKey)
    , processedSize(0)
{
    qint64 hour = _time.hour() * 60 * 60 * 1000;
    qint64 minute = _time.minute() * 60 * 1000;
    qint64 second = _time.second() * 1000;
    msecToNextStart = hour + minute + second;
}

void FileProcessor::manage()
{
    processFiles();
    if (isRepeating) {
        unsigned int msec = 0;
        while (isRepeating) {
            QThread::msleep(500);
            msec += 500;
            emit progress(0, msecToNextStart - msec);
            if (msec >= msecToNextStart) {
                processFiles();
                msec = 0;
            }
        }
    }
    emit finished();
}

void FileProcessor::processFiles()
{
    totalSize = 0;
    processedSize = 0;
    outputFilesPaths.clear();
    inputFilesPaths = findFilesByMask(inputDir, inputMask);
    for (int i = 0; i < inputFilesPaths.size(); i++){
        totalSize += getFileSize(inputFilesPaths[i]);
        QFileInfo inputInfo(inputFilesPaths[i]);
        QString fileName = inputInfo.fileName();
        outputFilesPaths.push_back(QDir(outputDir).filePath(fileName));
    }

    for (int i = 0; i < inputFilesPaths.size(); i++) {
        if (overrideOutput)
            processOneFile(inputFilesPaths[i], outputFilesPaths[i]);
        else
            processOneFile(inputFilesPaths[i], generateUniquePath(outputFilesPaths[i]));
        if (deleteInput)
            QFile::remove(inputFilesPaths[i]);
    }
}

void FileProcessor::processOneFile(QString inputPath, QString outputPath)
{
    QFile inputFile(inputPath);
    QFile outputFile(outputPath);

    // Открываем файлы
    if (!inputFile.open(QIODevice::ReadOnly) ||
        !outputFile.open(QIODevice::WriteOnly)) {
        return;
    }

    // Читаем и обрабатываем файл блоками
    const qint64 bufferSize = 8192; // 8KB блоки
    char buffer[bufferSize];

    while (!inputFile.atEnd()) {
        qint64 bytesRead = inputFile.read(buffer, bufferSize);

        // XOR операция с каждым байтом
        for (qint64 i = 0; i < bytesRead; i++) {
            buffer[i] = buffer[i] ^ xorKey; // Циклическое применение ключа
        }

        outputFile.write(buffer, bytesRead);
        processedSize += bytesRead;

        // Отправляем прогресс
        emit progress((int)((double)processedSize / (double)totalSize * 100.0), 0);
    }
}

QString FileProcessor::generateUniquePath(QString originalPath)
{
    if (!QFile::exists(originalPath)) return originalPath;

    // Добавление счетчика: file.txt -> file (1).txt
    QFileInfo info(originalPath);
    QString baseName = info.baseName();
    QString extension = info.completeSuffix();
    QString path = info.path();

    int counter = 1;
    QString newPath;
    do {
        newPath = QString("%1/%2 (%3).%4")
        .arg(path).arg(baseName).arg(counter).arg(extension);
        counter++;
    } while (QFile::exists(newPath));

    return newPath;
}

QStringList FileProcessor::findFilesByMask(QString directoryPath, QString mask)
{
    QDir directory(directoryPath);

    // Устанавливаем фильтр для файлов
    QStringList filters;
    filters << mask; // Например: "*.txt" или "testFile.bin"

    // Настройка фильтров
    directory.setNameFilters(filters);
    directory.setFilter(QDir::Files | QDir::NoDotAndDotDot);

    // Получаем список файлов
    QStringList files = directory.entryList();

    // Добавляем полный путь к каждому файлу
    for (QString& file : files) {
        file = directory.absoluteFilePath(file);
    }

    return files;
}

qint64 FileProcessor::getFileSize(QString filePath)
{
    QFileInfo fileInfo(filePath);

    if (!fileInfo.exists())
        return -1;

    return fileInfo.size();
}
