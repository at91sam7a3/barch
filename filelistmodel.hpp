#ifndef FILELISTMODEL_HPP
#define FILELISTMODEL_HPP
#include <QAbstractListModel>
#include <QObject>
#include <QQmlApplicationEngine>

enum FileRoles
{
  RoleName = Qt::UserRole + 1,
  RoleSize,
  RoleType,
  RoleProgress
};

class FileListModel : public QAbstractListModel
{
  Q_OBJECT
  Q_PROPERTY(QString currentFolder READ currentFolder WRITE setCurrentFolder NOTIFY currentFolderChanged)
public:
  FileListModel(const QString& currentPath);

  void registerProperties(QQmlApplicationEngine& engine, const QString& name);
  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  void updateModel();

  QString currentFolder() const;
  void setCurrentFolder(const QString& newCurrentFolder);

public slots:
  void progressUpdated(int, QString);
  void complete(QString);

signals:
  void currentFolderChanged();

protected:
  QHash<int, QByteArray> roleNames() const;

private:
  QStringList m_lstValues;
  QString m_currentFolder;
  QMap<QString, int> m_filesInProgress;
};

#endif // FILELISTMODEL_HPP
