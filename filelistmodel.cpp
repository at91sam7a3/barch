#include "filelistmodel.hpp"

#include <QDebug>
#include <QDir>
#include <QQmlContext>

FileListModel::FileListModel(const QString& currentPath)
  : m_currentFolder(currentPath)
{
  updateModel();
}

void FileListModel::registerProperties(QQmlApplicationEngine& engine, const QString& name)
{
  engine.rootContext()->setContextProperty(name, this);
}

int FileListModel::rowCount(const QModelIndex& parent) const
{
  return m_lstValues.count();
}

QVariant FileListModel::data(const QModelIndex& index, int role) const
{
  if ((index.row() < 0) || (index.row() >= rowCount()))
  {
    return QVariant();
  }
  QString filename = m_lstValues[index.row()];
  switch (role)
  {
    case RoleName: return filename;
    case RoleType:
      if (filename.toLower().contains(".bmp"))
      {
        return "BMP";
      }
      else
      {
        return "BARCH";
      }
    case RoleSize:
    {
      QDir dir(m_currentFolder);
      QFile file;
      file.setFileName(dir.absoluteFilePath(filename));
      QLocale locale;
      return locale.formattedDataSize(file.size());
    }
    case RoleProgress:
    {
      if (m_filesInProgress.contains(filename))
      {
        return m_filesInProgress[filename];
      }
      else
      {
        return 0;
      }
    }
    default: break;
  }
  return 0;
}

void FileListModel::updateModel()
{
  //assume the directory exists and contains some files and you want all png and barch files
  QDir directory(m_currentFolder);
  beginResetModel();
  m_lstValues = directory.entryList(QStringList() << "*.png"
                                                  << "*.PNG"
                                                  << "*.bmp"
                                                  << "*.BMP"
                                                  << "*.barch"
                                                  << "*.BARCH",
                                    QDir::Files);
  endResetModel();
  qWarning() << "Model with elements: " << m_lstValues.size();
}

QHash<int, QByteArray> FileListModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[FileRoles::RoleName] = "Name";
  roles[FileRoles::RoleSize] = "Size";
  roles[FileRoles::RoleType] = "Type";
  roles[FileRoles::RoleProgress] = "Progress";
  return roles;
}

QString FileListModel::currentFolder() const
{
  return m_currentFolder;
}

void FileListModel::setCurrentFolder(const QString& newCurrentFolder)
{
  if (m_currentFolder == newCurrentFolder)
    return;
  m_currentFolder = newCurrentFolder;
  updateModel();
  emit currentFolderChanged();
}

void FileListModel::progressUpdated(int progress, QString filename)
{
  m_filesInProgress[filename] = progress;

  for (int i = 0; i < m_lstValues.size(); ++i)
  {
    if (m_lstValues[i] == filename)
    {
      emit dataChanged(index(i), index(i));
      break;
    }
  }
}

void FileListModel::complete(QString filename)
{
  m_filesInProgress.remove(filename);
  updateModel();
}
