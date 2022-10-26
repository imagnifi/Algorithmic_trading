#ifndef INTERPOLATION_SPLINES_H_
#define INTERPOLATION_SPLINES_H_
#include "abstractgraph.h"

namespace s21 {

class InterpolationSplines : public AbstractGraph {
 public:
  InterpolationSplines();
  ~InterpolationSplines();
  auto GetData() -> const GraphVector & override;
  auto Proceed(const GraphConfig &config) -> GraphVector override;
  auto Proceed(double x) -> double override;
  auto SetData(const std::vector<GraphPoint> &) -> void override;
  auto ClearData() -> void override;

 private:
  /**
   * @brief main_data - data for calculate in interpolation function
   */
  std::vector<GraphPoint> main_data_;
  std::vector<double> a_, b_, c_, d_;
  std::vector<double> h_, F_, V_, K_, L_;
  GraphVector result_;
  double x_min_, x_max_;
  auto Convert(double x) -> double;
  auto ClearVector() -> void;
};

}  //   namespace s21

#endif  // INTERPOLATION_SPLINES_H_
