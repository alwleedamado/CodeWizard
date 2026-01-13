#include "MainWindow.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
  const QApplication app(argc, argv);
  QApplication::setStyle(QStyleFactory::create("Fusion")); // dark-friendly
  MainWindow window;
  window.show();
  return app.exec();
}
