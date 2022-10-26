#include "approximation.h"

namespace s21 {

Approximation::Approximation()
    : AbstractGraph(), A{1, 1}, B{1, 1}, max_date_(std::nullopt) {}
Approximation::~Approximation(){};

auto Approximation::SetData(const GraphVector &source_data) -> void {
  main_data_ = GraphVector(source_data);
};

auto Approximation::Proceed(const GraphConfig &config) -> GraphVector {
  unsigned points_num = config.points_num_;
  unsigned source_num = main_data_.size();
  unsigned days_num = config.days_num_;
  short polinom_degree = config.polinom_degree_ + 1;
  GraphVector result = GraphVector(points_num);
  if (polinom_degree < 2)
    throw std::invalid_argument("Please raise polynom degree");
  x_min_ = main_data_[0].first;
  x_max_ = main_data_[0].first;

  //  int i=0;
  std::for_each(main_data_.begin(), main_data_.end(),
                [this](GraphPoint const &el) {
                  x_max_ = x_max_ < el.first ? el.first : x_max_;
                  x_min_ = x_min_ > el.first ? el.first : x_min_;
                });

  double interval = (x_max_ - x_min_ + days_num * 3600 * 24) / points_num;
  std::for_each(result.begin(), result.end(),
                [interval, this, i = 0](GraphPoint &el) mutable {
                  el.first = x_min_ + (i++) * interval;
                });
  max_date_ = result.back().first;
  A.set_rows(source_num);
  A.set_columns(polinom_degree);
  B.set_rows(source_num);
  SetMatrices(A, B, polinom_degree);
  MatrixD A_transposed = A.Transpose();
  polinom_coefs = A_transposed * A;
  polinom_coefs = polinom_coefs.InverseMatrix();
  polinom_coefs *= A_transposed;
  polinom_coefs *= B;
  std::for_each(result.begin(), result.end(),
                [this](GraphPoint &el) { el.second = Proceed(el.first); });
  return result;
}

auto Approximation::convert(double x) -> double {
  return (2 * x - x_max_ - x_min_) / (x_max_ - x_min_);
};

auto Approximation::GetData() -> const GraphVector & { return main_data_; };

auto Approximation::ClearData() -> void { main_data_.clear(); };

auto Approximation::SetMatrices(MatrixD &a, MatrixD &b, double polinom_degree)
    -> void {
  for (unsigned i = 0; i < main_data_.size(); i++) {
    double x = 1;
    for (unsigned j = 0; j < polinom_degree; j++) {
      a(i, j) = x;
      x *= convert(main_data_[i].first);
    }
    b(i, 0) = main_data_[i].second;
  }
  std::cout << "b from set " << b(1, 0) << std::endl;
}

auto Approximation::Proceed(double x) -> double {
  double res = 0;
  double x_val = convert(x);
  if (main_data_.empty() || A.get_rows() == 1)
    throw std::invalid_argument("Please make calculations first");
  if (!max_date_ || x_val < -1 || x_val > max_date_.value())
    throw std::invalid_argument("Date is out of range");
  if (main_data_.size() == 0 || B.get_rows() == 1) {
    throw std::invalid_argument("No data loaded");
  } else {
    double acc = 1;
    int rows = polinom_coefs.get_rows();
    for (int i = 0; i < rows; i++) {
      res += polinom_coefs(i, 0) * acc;
      acc *= x_val;
    };
  };
  return res;
}
}  // namespace s21
