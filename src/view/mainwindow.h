#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <vector>

#include "controller/controller.h"
#include "shared/qcustomplot/qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent, s21::Controller *contr);
  ~MainWindow();

 private slots:
  void on_btn_load_from_file_clicked();
  void on_draw_interpol_clicked();
  void on_draw_approxima_clicked();

  void on_btn_clear_data_clicked();

  void on_int_clear_clicked();

  void on_app_clear_clicked();

  void on_ansver_clicked();

  void on_ansver_2_clicked();

 private:
  Ui::MainWindow *ui;
  s21::Controller *contr_;
  int graph_max_count_;
  s21::GraphVector vec_{};

  auto DrawGraph() -> void;
  auto ClearGraphs() -> void;
  auto DrawInterpolation(s21::GraphVector &, s21::GraphType) -> void;
  auto DrawAproximation(s21::GraphVector &) -> void;
  auto DrawFuntion(s21::GraphVector &, int) -> void;
  auto SetReasonableScale(QCustomPlot *area, const QVector<double> &coords,
                          const QVector<double> &values, int days = 0) -> void;

  const QPen kGraphColors[6]{QColor(50, 50, 50, 255), QColor(Qt::green),
                             QColor(Qt::blue),        QColor(Qt::cyan),
                             QColor(Qt::magenta),     QColor(Qt::red)};
};
#endif  // MAINWINDOW_H
