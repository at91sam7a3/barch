#include "uicontroller.hpp"
#include "workerthread.hpp"
#include "qdir.h"

#include <QImage>
#include <QQmlContext>
#include <QThreadPool>

UIController::UIController(const QString& currentPath, QObject* parent)
  : QObject{parent}
  , m_model(currentPath)
{
    pool = new QThreadPool(this);
    pool->setMaxThreadCount(10);
}

void UIController::registerProperties(QQmlApplicationEngine& engine, const QString& name)
{
  engine.rootContext()->setContextProperty(name, this);
  m_model.registerProperties(engine, "FileListModel");
  m_messageBoxController.registerProperties(engine, "MessageBoxController");
}

void UIController::convert(const QString& filename)
{
  if(!filename.toLower().contains(".bmp") && !filename.toLower().contains(".png") && !filename.toLower().contains(".barch"))
  {
      m_messageBoxController.DisplayMessage("Sorry, wrong file type");
      return;
  }

  const QDir dir(m_model.currentFolder());
  const QString fullname = dir.absoluteFilePath(filename);
  bool compress = filename.toLower().contains(".bmp");
  QString newName(fullname);
  if (compress)
  {
    if (newName.contains("_unpacked"))
    {
      newName.replace(QString("_unpacked.png"), QString("_packed.barch"));
      newName.replace(QString("_unpacked.bmp"), QString("_packed.barch"));
    }
    else
    {
      newName.replace(QString(".png"), QString("_packed.barch"));
      newName.replace(QString(".bmp"), QString("_packed.barch"));
    }
  }
  else
  {
    if (newName.contains("_packed"))
    {
      newName.replace(QString("_packed.barch"), QString("_unpacked.bmp"));
    }
    else
    {
      newName.replace(QString(".barch"), QString("_unpacked.bmp"));
    }
  }

  WorkerThread* newWorker = new WorkerThread(filename, fullname, newName);
  newWorker->setAutoDelete(true);
  QObject::connect(newWorker, &WorkerThread::taskCompleted, &m_model, &FileListModel::complete,Qt::QueuedConnection);
  QObject::connect(newWorker, &WorkerThread::progressUpdated, &m_model, &FileListModel::progressUpdated,Qt::QueuedConnection);
  QObject::connect(newWorker, &WorkerThread::errorMessage, &m_messageBoxController, &MessageBoxController::DisplayMessage,Qt::QueuedConnection);
  pool->tryStart(newWorker);
}
