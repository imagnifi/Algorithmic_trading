#include "interpolation.h"

#include <iomanip>

namespace s21 {

Interpolation::Interpolation()
    : AbstractGraph(), main_data_(), diff_(), degree_(3) {}
Interpolation::~Interpolation() {}

auto Interpolation::GetData() -> const GraphVector& { return main_data_; }

auto Interpolation::SetData(const std::vector<GraphPoint>& src) -> void {
  main_data_ = src;
}

auto Interpolation::ClearData() -> void {
  main_data_.clear();
  diff_.clear();
  divided_dif_.clear();
}

auto Interpolation::Proceed([[maybe_unused]] const GraphConfig& config)
    -> GraphVector {
  GraphVector result(config.points_num_);
  degree_ = config.polinom_degree_;
  double min = main_data_[0].first;
  double max = main_data_[main_data_.size() - 1].first;
  double step = (max - min) / (config.points_num_ - 1);
  for (uint i = 0; i < config.points_num_; i++) {
    result[i].first = (step * i) + min;
    result[i].second =
        CalculateFormula(result[i].first, config.polinom_degree_);
  }
  result = AddMainData(result);
  result.back().second = main_data_.back().second;
  return result;
}

double Interpolation::Proceed(double x) {
  return CalculateFormula(x, degree_);
};

double Interpolation::CalculateFormula(double x, int degree) {
  std::pair<int, double> tmp(FindStart(x, degree));
  int start = tmp.first;
  double min = main_data_[start].first;
  double h = tmp.second;
  long double result = main_data_[start].second;
  ClearDiff();
  ComputeDiff(start);
  int end = std::min(degree + 1, (int)main_data_.size());
  for (int i = 1; i < end; i++) {
    long double buffer = 1.0;
    for (int n = 0; n < i; n++) {
      buffer *= ((x - min) / h) - ((main_data_[start + n].first - min) / h);
    }
    buffer *= divided_dif_[i][i];
    result += buffer;
  }
  return result;
}

GraphVector Interpolation::AddMainData(GraphVector& src) {
  GraphVector res;
  for (size_t i = 0; i < src.size(); i++) {
    if (i > 0) {
      for (size_t j = 0; j < main_data_.size(); j++) {
        if (main_data_[j].first < src[i].first and
            main_data_[j].first > src[i - 1].first) {
          res.push_back({main_data_[j].first, main_data_[j].second});
        }
      }
    }
    res.push_back({src[i].first, src[i].second});
  }
  return res;
}

auto Interpolation::Factorial(int n) -> int {
  return (n == 1 || n == 0) ? 1 : Factorial(n - 1) * n;
}

auto Interpolation::Compute_h(int number_of_points) -> double {
  double res = main_data_[main_data_.size() - 1].first - main_data_[0].first;
  return res / number_of_points;
}

void Interpolation::ComputeDiffTable(std::vector<double> vec, int start) {
  double min = main_data_[start].first;
  double step = std::abs(main_data_[1].first - main_data_[0].first);
  divided_dif_.emplace_back(vec);
  for (size_t i = 1; i < vec.size(); i++) {
    auto& cur = divided_dif_.emplace_back();
    auto& prev = divided_dif_[i - 1];
    for (size_t n = 0; n < i; n++) {
      cur.push_back(0.0);
    }
    for (size_t j = i; j < prev.size(); j++) {
      cur.push_back((prev[j] - prev[j - 1]) /
                    ((main_data_[j + start].first - min) / step -
                     (main_data_[start].first - min) / step));
    }
  }
}

void Interpolation::ComputeDiff(int start) {
  std::vector<double> y_;

  for (size_t i = start; i < main_data_.size(); i++) {
    y_.push_back(main_data_[i].second);
  }
  ComputeDiffTable(y_, start);
}

std::pair<int, double> Interpolation::FindStart(time_t x, int degree) {
  std::pair<int, double> res;
  int size = main_data_.size();

  int nearestIndex = FindNearest(x);
  if (main_data_[nearestIndex].first == x) {
    res.first = nearestIndex;
  }
  int start = std::max(nearestIndex, 0);
  if (start + degree > size) start = size - degree;
  res.first = start;
  res.second = std::abs(main_data_[1].first - main_data_[0].first);
  if (degree >= size) {
    res.first = 0.0;
  }
  return res;
}

int Interpolation::FindNearest(time_t x) {
  int nearestIndex = 0;
  for (int i = 1; i < static_cast<int>(main_data_.size()); i++) {
    if (main_data_[i].first > x) {
      nearestIndex = i - 1;
      break;
    }
  }
  return nearestIndex;
}

}  //   namespace s21
