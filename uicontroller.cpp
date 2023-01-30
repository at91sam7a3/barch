#include "uicontroller.hpp"

#include "qdir.h"

#include <QImage>
#include <QQmlContext>
#include <QThreadPool>

UIController::UIController(QString currentPath, QObject* parent)
  : QObject{parent}
  , m_model(currentPath)
{}

void UIController::registerProperties(QQmlApplicationEngine& engine, const QString& name)
{
  engine.rootContext()->setContextProperty(name, this);
  m_model.registerProperties(engine, "FileListModel");
}

void UIController::convert(QString filename)
{
  QDir dir(m_model.currentFolder());
  QString fullname = dir.absoluteFilePath(filename);
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
  QObject::connect(newWorker, &WorkerThread::taskCompleted, &m_model, &FileListModel::complete);
  QObject::connect(newWorker, &WorkerThread::progressUpdated, &m_model, &FileListModel::progressUpdated);
  QThreadPool::globalInstance()->start(newWorker);
}
