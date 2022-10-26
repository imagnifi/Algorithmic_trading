#include "mainwindow.h"

#include <QMessageBox>
#include <vector>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, s21::Controller *contr)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      contr_(contr),
      graph_max_count_(5) {
  ui->setupUi(this);
  statusBar()->setSizeGripEnabled(false);

  ui->interpol->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
  ui->approx->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
  QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
  dateTicker->setDateTimeFormat("d. MMMM\nyyyy hh:mm ");
  ui->interpol->xAxis->setTicker(dateTicker);
  ui->approx->xAxis->setTicker(dateTicker);

  ui->interpol->setInteraction(QCP::iRangeZoom, true);
  ui->interpol->setInteraction(QCP::iRangeDrag, true);
  ui->approx->setInteraction(QCP::iRangeZoom, true);
  ui->approx->setInteraction(QCP::iRangeDrag, true);

  ui->interpol->legend->setBrush(QColor(215, 217, 168, 100));
  ui->approx->legend->setBrush(QColor(215, 217, 168, 100));
  QFont legend_font = font();
  legend_font.setPointSize(10);
  ui->interpol->legend->setFont(legend_font);
  ui->approx->legend->setFont(legend_font);
  ui->interpol->axisRect()->insetLayout()->setInsetAlignment(
      0, Qt::AlignTop | Qt::AlignLeft);
  ui->approx->axisRect()->insetLayout()->setInsetAlignment(
      0, Qt::AlignTop | Qt::AlignLeft);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_btn_load_from_file_clicked() {
  s21::GraphConfig config;
  config.graph_type_ = s21::APPROXIMATION;
  contr_->Init(config);
  QString file_path = QFileDialog::getOpenFileName(
      this, QFileDialog::tr("Open file"), QFileDialog::tr("(*.csv)"));
  if (!file_path.isEmpty()) {
    contr_->LoadFromFile(file_path.toStdString());
    vec_ = contr_->GetData();
    ui->int_points_num->setMinimum(contr_->GetData().size());
    ui->int_points_num->setValue(contr_->GetData().size());
    ui->app_points_num->setMinimum(contr_->GetData().size());
    ui->app_points_num->setValue(contr_->GetData().size());

    ui->int_dateTimeEdit->setMinimumDate(
        QDateTime::fromSecsSinceEpoch(contr_->GetData()[0].first).date());
    ui->int_dateTimeEdit->date() =
        QDateTime::fromSecsSinceEpoch(contr_->GetData()[0].first).date();
    ui->app_dateTimeEdit->setMinimumDate(
        QDateTime::fromSecsSinceEpoch(contr_->GetData()[0].first).date());
    ui->app_dateTimeEdit->date() =
        QDateTime::fromSecsSinceEpoch(contr_->GetData()[0].first).date();

    ClearGraphs();
    ui->file_name->setText(
        file_path.right(file_path.size() - file_path.lastIndexOf("/") - 1));
    DrawGraph();
  }
}

void MainWindow::DrawGraph() {
  ui->interpol->addGraph();
  ui->approx->addGraph();

  s21::GraphVector vec;
  vec = contr_->GetData();
  QVector<double> values(vec.size()), dates(vec.size());
  for (size_t i = 0; i < vec.size(); i++) {
    values[i] = vec.at(i).second;
    dates[i] = vec[i].first;
  }

  ui->interpol->graph(0)->setData(dates, values);

  ui->interpol->graph(0)->setPen(QColor(Qt::black));
  ui->approx->graph(0)->setPen(QColor(Qt::black));
  ui->interpol->graph(0)->setLineStyle(QCPGraph::lsNone);
  ui->interpol->graph(0)->setScatterStyle(
      QCPScatterStyle(QCPScatterStyle::ssDisc, 6));
  ui->interpol->graph(0)->setName("main data");
  SetReasonableScale(ui->interpol, dates, values);

  ui->approx->graph(0)->setData(dates, values);
  ui->approx->graph(0)->setLineStyle(QCPGraph::lsNone);
  ui->approx->graph(0)->setScatterStyle(
      QCPScatterStyle(QCPScatterStyle::ssDisc, 6));
  ui->approx->graph(0)->setName("main data");
  SetReasonableScale(ui->approx, dates, values);

  ui->interpol->replot();
  ui->approx->replot();
}

void MainWindow::ClearGraphs() {
  ui->interpol->clearGraphs();
  ui->approx->clearGraphs();
  ui->interpol->legend->setVisible(false);
  ui->approx->legend->setVisible(false);
  ui->draw_approxima->setEnabled(true);
  ui->draw_interpol->setEnabled(true);
  ui->ansver->setEnabled(false);
  ui->ansver_2->setEnabled(false);
  ui->file_name->setText("None");
}

void MainWindow::DrawInterpolation(s21::GraphVector &vec, s21::GraphType type) {
  int points = ui->int_points_num->value();

  int count = ui->interpol->graphCount() - 1;
  if (count < graph_max_count_) {
    ui->interpol->addGraph();
    ++count;
    ui->interpol->graph(count)->setPen(kGraphColors[count]);
    ui->interpol->graph(count)->setLineStyle(QCPGraph::lsLine);
    ui->interpol->graph(count)->setScatterStyle(
        QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
    if (type == s21::PONINOMIAL) {
      ui->interpol->graph(count)->setName(
          QString("Newton, points: ") + QString::number(points) +
          QString(" Degree: ") +
          QString::number(ui->int_polinom_degree->value()));
    } else if (type == s21::SPLINE) {
      ui->interpol->graph(count)->setName(QString("Spline, points: ") +
                                          QString::number(points));
    }
    QVector<double> values(vec.size()), dates(vec.size());
    for (size_t i = 0; i < vec.size(); i++) {
      values[i] = vec.at(i).second;
      dates[i] = vec[i].first;
    }
    ui->interpol->graph(count)->setData(dates, values);
    ui->interpol->legend->setVisible(true);
    //    SetReasonableScale(ui->interpol, dates, values);
    ui->interpol->replot();
  } else {
    ui->draw_interpol->setEnabled(false);
  }
  ui->ansver->setEnabled(true);
}

void MainWindow::DrawAproximation(s21::GraphVector &vec) {
  int points = ui->app_points_num->value();

  int count = ui->approx->graphCount() - 1;
  if (count < graph_max_count_) {
    ui->approx->addGraph();
    ++count;
    ui->approx->graph(count)->setPen(kGraphColors[count]);
    ui->approx->graph(count)->setLineStyle(QCPGraph::lsLine);
    ui->approx->graph(count)->setScatterStyle(
        QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
    ui->approx->graph(count)->setName(QString("Approxima, points: ") +
                                      QString::number(points));

    QVector<double> values(vec.size()), dates(vec.size());
    for (size_t i = 0; i < vec.size(); i++) {
      values[i] = vec.at(i).second;
      dates[i] = vec[i].first;
    }
    ui->approx->graph(count)->setData(dates, values);
    ui->approx->legend->setVisible(true);
    ui->approx->replot();
  } else {
    ui->draw_approxima->setEnabled(false);
  }
  ui->ansver_2->setEnabled(true);
}

void MainWindow::SetReasonableScale(QCustomPlot *area,
                                    const QVector<double> &coords,
                                    const QVector<double> &values, int days) {
  double left{coords[0]}, right{coords[coords.size() - 1] + days * 86400},
      top{values[0]}, bottom{values[0]};
  for (auto &one : values) {
    if (one < bottom)
      bottom = one;
    else if (one > top)
      top = one;
  }
  double x_padding{(right - left) / 20}, y_padding{(top - bottom) / 20};

  area->xAxis->setRange(left - x_padding, right + x_padding);
  area->yAxis->setRange(bottom - y_padding, top + y_padding);
}

void MainWindow::on_draw_interpol_clicked() {
  if (contr_->IsData()) {
    s21::GraphConfig config;
    config.points_num_ = ui->int_points_num->value();
    config.polinom_degree_ = ui->int_polinom_degree->value();
    if (ui->int_method_polinom->isChecked()) {
      config.graph_type_ = s21::PONINOMIAL;
    } else if (ui->int_method_spline->isChecked()) {
      config.graph_type_ = s21::SPLINE;
    }
    contr_->Init(config);
    s21::GraphVector result;
    result = contr_->Proceed(config);
    DrawInterpolation(result, config.graph_type_);
  } else {
    QMessageBox::about(this, "ERROR", "Please load MAINDATA");
  }
}

void MainWindow::on_draw_approxima_clicked() {
  if (contr_->IsData()) {
    try {
      s21::GraphConfig config;
      config.graph_type_ = s21::APPROXIMATION;
      config.points_num_ = ui->app_points_num->value();
      config.polinom_degree_ = ui->app_polinom_degree->value();
      config.days_num_ = ui->app_num_days->value();
      contr_->Init(config);
      s21::GraphVector result;
      result = contr_->Proceed(config);
      DrawAproximation(result);
    } catch (std::exception &e) {
      QMessageBox::about(this, "ERROR", e.what());
    }
  } else {
    QMessageBox::about(this, "ERROR", "Please load MAINDATA");
  }
}

void MainWindow::on_btn_clear_data_clicked() {
  ClearGraphs();
  ui->interpol->replot();
  ui->approx->replot();
  contr_->ClearData();
}

void MainWindow::on_int_clear_clicked() {
  int count = ui->interpol->graphCount() - 1;
  if (count <= graph_max_count_ and count > 0) {
    ui->interpol->removeGraph(count);
    ui->interpol->replot();
    if (count == graph_max_count_) ui->draw_interpol->setEnabled(true);
  }
}

void MainWindow::on_app_clear_clicked() {
  int count = ui->approx->graphCount() - 1;
  if (count <= graph_max_count_ and count > 0) {
    ui->approx->removeGraph(count);
    ui->approx->replot();
    if (count == graph_max_count_) ui->draw_approxima->setEnabled(true);
  }
}

void MainWindow::on_ansver_clicked() {
  ui->x_ansver->setText("");
  try {
    if (ui->int_dateTimeEdit->dateTime().toSecsSinceEpoch() <=
        vec_[vec_.size() - 1].first) {
      double ansv =
          contr_->Proceed(ui->int_dateTimeEdit->dateTime().toSecsSinceEpoch());
      ui->x_ansver->setText(QString::number(ansv, 'f', 3));
    }
  } catch (std::exception &e) {
    QMessageBox::about(this, "ERROR", e.what());
  }
}

void MainWindow::on_ansver_2_clicked() {
  ui->x_ansver_2->setText("");
  try {
    if (ui->app_dateTimeEdit->dateTime().toSecsSinceEpoch() <=
        vec_[vec_.size() - 1].first + (ui->app_num_days->value() * 86400)) {
      double ansv =
          contr_->Proceed(ui->app_dateTimeEdit->dateTime().toSecsSinceEpoch());
      ui->x_ansver_2->setText(QString::number(ansv, 'f', 3));
    }
  } catch (std::exception &e) {
    QMessageBox::about(this, "ERROR", e.what());
  }
}
