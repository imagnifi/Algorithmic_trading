#include "winograd.h"
namespace s21 {

Winograd::Winograd() {}

Winograd::~Winograd() {}

void Winograd::CreateMatrices(int a, int b, int c) {
  first_ = Matrix<double>(a, b);
  second_ = Matrix<double>(b, c);
  result_ = Matrix<double>(a, c);
  half_size_ = second_.get_rows() / 2;
}

void Winograd::InitData() {
  row_factor_.clear();
  col_factor_.clear();
}

void Winograd::CalcRowFactor() {
  for (int i = 0; i < first_.get_rows(); i++) {
    row_factor_.push_back(0);
    for (int j = 0; j < half_size_; j++) {
      row_factor_.at(i) += first_(i, 2 * j + 1) * first_(i, 2 * j);
    }
  }
}

void Winograd::CalcColFactor() {
  for (int i = 0; i < second_.get_columns(); i++) {
    col_factor_.push_back(0);
    for (int j = 0; j < half_size_; j++) {
      col_factor_.at(i) += second_(2 * j + 1, i) * second_(2 * j, i);
    }
  }
}

void Winograd::CalcResult() {
  for (int i = 0; i < first_.get_rows(); i++) {
    for (int j = 0; j < second_.get_columns(); j++) {
      result_(i, j) = -row_factor_.at(i) - col_factor_.at(j);
      for (int k = 0; k < half_size_; k++) {
        result_(i, j) += (first_(i, 2 * k) + second_(2 * k + 1, j)) *
                         (first_(i, 2 * k + 1) + second_(2 * k, j));
      }
    }
  }
}

void Winograd::CorrectionResult() {
  if (second_.get_rows() % 2 == 1) {
    for (int i = 0; i < first_.get_rows(); i++) {
      for (int j = 0; j < second_.get_columns(); j++) {
        result_(i, j) += first_(i, second_.get_rows() - 1) *
                         second_(second_.get_rows() - 1, j);
      }
    }
  }
}

void Winograd::SimpleMultiplication() {
  if (first_.get_rows() == 1 && second_.get_rows() == 1 &&
      second_.get_columns() == 1) {
    result_(0, 0) = first_(0, 0) * second_(0, 0);
    return;
  }
  InitData();
  CalcRowFactor();
  CalcColFactor();
  CalcResult();
  CorrectionResult();
}
}  // namespace s21
