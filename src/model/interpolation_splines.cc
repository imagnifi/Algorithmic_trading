#include "interpolation_splines.h"

namespace s21 {
InterpolationSplines::InterpolationSplines()
    : AbstractGraph(), x_min_(0), x_max_(0){};
InterpolationSplines::~InterpolationSplines(){};

auto InterpolationSplines::Proceed(const GraphConfig &config) -> GraphVector {
  unsigned points_num = config.points_num_;
  unsigned source_num = main_data_.size();
  ClearVector();
  result_ = GraphVector(points_num);

  x_min_ = main_data_[0].first;
  x_max_ = main_data_[0].first;
  std::for_each(main_data_.begin(), main_data_.end(), [this](GraphPoint &el) {
    x_max_ = x_max_ < el.first ? el.first : x_max_;
    x_min_ = x_min_ > el.first ? el.first : x_min_;
  });
  double interval = (x_max_ - x_min_) / points_num;
  std::for_each(result_.begin(), result_.end(),
                [interval, this, i = 0](GraphPoint &el) mutable {
                  el.first = x_min_ + (i++) * interval;
                });
  auto y = [this](unsigned i) { return main_data_[i].second; };
  a_ = std::vector<double>(source_num + 1);
  b_ = std::vector<double>(source_num + 1);
  c_ = std::vector<double>(source_num + 1);
  d_ = std::vector<double>(source_num + 1);
  h_ = std::vector<double>(source_num);
  F_ = std::vector<double>(source_num);
  V_ = std::vector<double>(source_num);
  K_ = std::vector<double>(source_num);
  L_ = std::vector<double>(source_num);

  c_[0] = c_[source_num] = K_[0] = L_[0] = 0;

  for (unsigned i = 1; i < source_num; i++)
    h_[i] = Convert(main_data_[i].first) - Convert(main_data_[i - 1].first);

  for (unsigned i = 2; i < source_num; i++)
    F_[i] = 3 * ((y(i) - y(i - 1)) / h_[i] - (y(i - 1) - y(i - 2)) / h_[i - 1]);

  for (unsigned i = 1; i < source_num; i++) V_[i] = 2 * (h_[i] + h_[i - 1]);

  for (unsigned i = 1; i < source_num; i++) {
    K_[i] = (F_[i] - h_[i - 1] * K_[i - 1]) / (V_[i] - h_[i - 1] * L_[i - 1]);
    L_[i] = h_[i] / (V_[i] - h_[i - 1] * L_[i - 1]);
  }
  for (unsigned i = source_num - 1; i > 0; i--) {
    c_[i] = K_[i] - L_[i] * c_[i + 1];
    d_[i] = (c_[i + 1] - c_[i]) / (3 * h_[i]);
    b_[i] = (y(i) - y(i - 1)) / h_[i] - c_[i] * h_[i] - d_[i] * pow(h_[i], 2);
    a_[i] = y(i - 1);
  }

  for (unsigned i = 0; i < points_num - 1; i++) {
    result_[i].second = Proceed(result_[i].first);
  };
  result_[points_num - 1].first = main_data_[source_num - 1].first;
  result_[points_num - 1].second = main_data_[source_num - 1].second;
  return result_;
};

auto InterpolationSplines::GetData() -> const GraphVector & {
  return main_data_;
};

auto InterpolationSplines::SetData(const std::vector<GraphPoint> &source_data)
    -> void {
  main_data_ = GraphVector(source_data);
};

auto InterpolationSplines::ClearData() -> void { main_data_.clear(); };

auto InterpolationSplines::Proceed(double x) -> double {
  if (main_data_.empty() || result_.size() < 2)
    throw std::invalid_argument("Please make calculations first");
  if (x < x_min_ || x > x_max_)
    throw std::invalid_argument("Date is out of range");

  unsigned i = 0;
  double result;
  unsigned source_num = main_data_.size();

  while (i < source_num &&
         !(main_data_[i].first <= x && main_data_[i + 1].first >= x)) {
    i++;
  }
  result = a_[i + 1] + b_[i + 1] * (Convert(x) - Convert(main_data_[i].first)) +
           c_[i + 1] * pow(Convert(x) - Convert(main_data_[i].first), 2) +
           d_[i + 1] * pow(Convert(x) - Convert(main_data_[i].first), 3);
  return result;
};

auto InterpolationSplines::Convert(double x) -> double {
  return (2 * x - x_max_ - x_min_) / (x_max_ - x_min_);
};

auto InterpolationSplines::ClearVector() -> void {
  a_.clear();
  b_.clear();
  c_.clear();
  d_.clear();
  h_.clear();
  F_.clear();
  V_.clear();
  K_.clear();
  L_.clear();
  result_.clear();
}

}  //   namespace s21
