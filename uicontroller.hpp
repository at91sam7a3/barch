#ifndef UICONTROLLER_HPP
#define UICONTROLLER_HPP

#include "filelistmodel.hpp"
#include "workerthread.hpp"

#include <QObject>

class UIController : public QObject
{
  Q_OBJECT
public:
  explicit UIController(QString currentPath, QObject* parent = nullptr);
  void registerProperties(QQmlApplicationEngine& engine, const QString& name);

  Q_INVOKABLE void convert(QString filename);
signals:

private:
  FileListModel m_model;
};

#endif // UICONTROLLER_HPP
