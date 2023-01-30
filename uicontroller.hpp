#ifndef UICONTROLLER_HPP
#define UICONTROLLER_HPP

#include "filelistmodel.hpp"
#include "messageboxcontroller.hpp"
#include <QObject>
#include <QThreadPool>

class UIController : public QObject
{
  Q_OBJECT
public:
  explicit UIController(const QString& currentPath, QObject* parent = nullptr);
  void registerProperties(QQmlApplicationEngine& engine, const QString& name);

  Q_INVOKABLE void convert(const QString& filename);
signals:

private:
  FileListModel m_model;
  MessageBoxController m_messageBoxController;
  QThreadPool* pool;
};

#endif // UICONTROLLER_HPP
