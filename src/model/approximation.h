#ifndef APPROXIMATION_H
#define APPROXIMATION_H

#include "../shared/matrix/matrix.hpp"
#include "abstractgraph.h"
#include "types.h"

namespace s21 {

class Approximation : public AbstractGraph {
 public:
  Approximation();
  ~Approximation();
  auto GetData() -> const GraphVector & override;
  auto Proceed(const GraphConfig &config) -> GraphVector override;
  auto Proceed(double x) -> double override;
  auto SetData(const GraphVector &) -> void override;
  auto ClearData() -> void override;

 private:
  MatrixD A, B, polinom_coefs;
  std::vector<GraphPoint> main_data_;
  double x_min_, x_max_;
  double_N max_date_;

  auto SetMatrices(MatrixD &a, MatrixD &b, double polinom_degree) -> void;

  auto convert(double x) -> double;
};
}  // namespace s21
#endif  // APPROXIMATION_H
