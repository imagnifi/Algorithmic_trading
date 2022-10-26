#ifndef TYPES_H
#define TYPES_H

#include <chrono>
#include <optional>
#include <utility>
#include <vector>

#include "../shared/matrix/matrix.hpp"

namespace s21 {

typedef std::pair<tm, double> DataPoint;
typedef std::pair<double, double> GraphPoint;
typedef std::vector<DataPoint> DataVector;
typedef std::vector<GraphPoint> GraphVector;
typedef Matrix<double> MatrixD;
typedef std::optional<double> double_N;
typedef Matrix<double_N> MatrixD_N;

enum GraphType { SPLINE, PONINOMIAL, APPROXIMATION };

struct GraphConfig {
  GraphType graph_type_;
  unsigned points_num_ = 0;
  unsigned days_num_ = 0;
  short polinom_degree_ = 0;
};
}  // namespace s21
#endif  // TYPES_H
