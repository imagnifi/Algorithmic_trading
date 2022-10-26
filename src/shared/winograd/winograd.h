#ifndef WINOGRAD_H
#define WINOGRAD_H
#include "../matrix/matrix.hpp"

namespace s21 {

class Winograd {
 public:
  Winograd();
  ~Winograd();

 private:
  Matrix<double> first_;
  Matrix<double> second_;
  Matrix<double> result_;
  std::vector<int> row_factor_;
  std::vector<int> col_factor_;
  int half_size_;

 public:
  void CreateMatrices(int a, int b, int c);
  void RandomFillMatrices();
  void ManualFillMatrices();
  void SimpleMultiplication();
  void ClassicalParallelism(int num_cycles);
  void PipelinedParallelism(int num_cycles);
  void PrintMatrices();

 private:
  void InitData();
  void CalcRowFactor();
  void CalcColFactor();
  void CalcResult();
  void CorrectionResult();
  int RandNum();
  Matrix<double> GetManualFillMatrix(int n, int m);
};
}  // namespace s21
#endif  // WINOGRAD_H
