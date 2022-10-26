#include <QApplication>

#include "controller/controller.h"
#include "view/mainwindow.h"

int main(int argc, char *argv[]) {
  s21::Controller controller;
  QApplication a(argc, argv);
  MainWindow w(nullptr, &controller);
  w.show();
  return a.exec();
}
