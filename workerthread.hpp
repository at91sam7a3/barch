#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QObject>
#include <QRunnable>

class WorkerThread
  : public QObject
  , public QRunnable
{
  Q_OBJECT
public:
  WorkerThread(const QString& file, const QString& path, const QString& newPath);
signals:
  void progressUpdated(int, QString);
  void taskCompleted(QString);

protected:
  void run();

private:
  void convert();
  void packFile(QString path);
  void unpackFile(QString path);
  QString fileName;
  QString filePath;
  QString newFilePath;
};

#endif // WORKERTHREAD_H
