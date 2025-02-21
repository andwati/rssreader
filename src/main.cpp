#include "main_window.hpp"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  RSSReader reader;
  reader.show();
  return app.exec();
}
