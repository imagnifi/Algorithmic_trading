#ifndef INTERPOLATION_H
#define INTERPOLATION_H
#include "abstractgraph.h"

namespace s21 {

class Interpolation : public AbstractGraph {
 public:
  Interpolation();
  ~Interpolation();

  auto Proceed(const GraphConfig &config) -> GraphVector override;
  auto Proceed(double x) -> double override;
  auto GetData() -> const GraphVector & override;
  auto SetData(const GraphVector &) -> void override;
  auto ClearData() -> void override;

 private:
  /**
   * @brief in_data_ - data for calculate in interpolation function
   */
  GraphVector main_data_;
  std::vector<double> diff_;
  std::vector<std::vector<double>> divided_dif_{};
  int degree_;

  auto CalculateFormula(double coord, int degree) -> double;
  auto AddMainData(GraphVector &) -> GraphVector;

  auto Factorial(int n) -> int;
  auto Compute_h(int number_of_points) -> double;
  auto ComputeDiffTable(std::vector<double>, int start) -> void;
  auto ComputeDiff(int start) -> void;
  auto ClearDiff() -> void { divided_dif_.clear(); }

  auto FindStart(time_t x, int degree) -> std::pair<int, double>;
  auto FindNearest(time_t x) -> int;

  double sigmaFunction(long double value);

  auto SecondPartPolinom(int degree, double h) -> double;
  auto DiffSecondOrder(size_t index, double h, double div) -> double;
};

}  //   namespace s21
#endif  // INTERPOLATION_H
