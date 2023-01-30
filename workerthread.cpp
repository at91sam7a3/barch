#include "workerthread.hpp"

#include "barch/barch.hpp"

#include <QDebug>
#include <QDir>
#include <QImage>
#include <fstream>


WorkerThread::WorkerThread(const QString& file, const QString& path, const QString& newPath)
  : fileName(file)
  , filePath(path)
  , newFilePath(newPath)
{}

void WorkerThread::run()
{
  convert();
}

void WorkerThread::convert()
{
  if (fileName.isEmpty() || filePath.isEmpty())
  {
    qWarning() << "no file name to convert";
    return;
  }
  if (!fileName.toLower().contains(".barch"))
  {
    packFile(filePath);
  }
  else
  {
    unpackFile(filePath);
  }
}

void WorkerThread::packFile(QString path)
{
  //Why test images are in indexed format?
  QImage img = QImage(path);

  if ((img.format() != QImage::Format_Indexed8) && (img.format() != QImage::Format_Grayscale8))
  {
      emit errorMessage("Only 8 bit birmats are supported");
      return;
  }

  if (img.format() != QImage::Format_Indexed8)
  {
    img = img.convertToFormat(QImage::Format_Grayscale8);
  }
  img = img.convertToFormat(QImage::Format_Grayscale8);
  barch::Bitmap bitmap;
  bitmap.width = img.width();
  bitmap.height = img.height();
  unsigned long bufSize = bitmap.height * bitmap.width;
  bitmap.data = (unsigned char*)malloc(bufSize);
  //next magic was needed
  int n = 0;
  for (int y = 0; y < bitmap.height; ++y)
  {
    for (int x = 0; x < bitmap.width; ++x)
    {
      bitmap.data[n] = img.pixel(x, y);
      n++;
    }
  }
  std::function<void(int)> callback = [this](int progress) { emit progressUpdated(progress, fileName); };
  std::size_t dataSize = barch::convertGs2Barch(bitmap, callback);

  //add width, height and size to barch to restore it right later
  std::ofstream outFile;
  outFile.open(newFilePath.toStdString(), std::ios::binary | std::ios::out);
  outFile.write((char*)&bitmap.width, sizeof(int));
  outFile.write((char*)&bitmap.height, sizeof(int));
  outFile.write((char*)&dataSize, sizeof(std::size_t));
  outFile.write((char*)(bitmap.data), dataSize);
  outFile.flush();
  outFile.close();
  free(bitmap.data);
  emit taskCompleted(fileName);
}

void WorkerThread::unpackFile(QString path)
{
  try
  {
    barch::Bitmap bitmap;
    size_t dataSize;
    std::ifstream inFile;
    inFile.open(path.toStdString(), std::ios::binary | std::ios::in);
    inFile.read(reinterpret_cast<char*>(&bitmap.width), sizeof(int));
    inFile.read(reinterpret_cast<char*>(&bitmap.height), sizeof(int));
    inFile.read(reinterpret_cast<char*>(&dataSize), sizeof(std::size_t));
    bitmap.data = (unsigned char*)malloc(dataSize);
    for (int i = 0; i < dataSize; ++i)
    {
      char b;
      inFile.read(&b, 1);
      bitmap.data[i] = b;
    }
    inFile.close();
    std::function<void(int)> callback = [this](int progress) { emit progressUpdated(progress, fileName); };
    barch::convertBarch2Gs(bitmap, dataSize, callback);
    size_t bufSize = bitmap.width * bitmap.height;
    std::ofstream raw;

    QImage img(bitmap.width, bitmap.height, QImage::Format_Grayscale8);
    int n = 0;
    for (int y = 0; y < bitmap.height; ++y)
      for (int x = 0; x < bitmap.width; ++x)
      {
        QColor color(bitmap.data[n], bitmap.data[n], bitmap.data[n]);
        img.setPixelColor(x, y, color);
        n++;
      }
    img.save(newFilePath);
    free(bitmap.data);
    emit taskCompleted(fileName);
  }
  catch (...)
  {
    qWarning() << "Something crashed";
    emit taskCompleted(fileName);
    emit errorMessage("we chashed during packing");
  }
}
