#pragma once

#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>

template <class Type>
class Matrix {
 public:
  Matrix(int rows, int columns);
  Matrix(Matrix const& other);
  Matrix(Matrix&& other);
  Matrix();
  ~Matrix();
  void clear();

  Matrix& operator=(Matrix const& other);
  Matrix& operator=(Matrix&& other);

  int get_rows();
  int get_columns();
  void set_rows(int rows);
  void set_columns(int columns);

  bool eq_matrix(Matrix const& other);
  void sum_matrix(Matrix const& other);
  void sub_matrix(Matrix const& other);
  void mul_matrix(Matrix const& other);
  void mul_number(Type num);

  Matrix operator+(Matrix const& other);
  Matrix operator-(Matrix const& other);
  Matrix operator*(Matrix const& other);
  Matrix operator*(Type num);

  Matrix& operator+=(Matrix const& other);
  Matrix& operator-=(Matrix const& other);
  Matrix& operator*=(Matrix const& other);
  Matrix& operator*=(Type num);

  bool operator==(Matrix const& other);
  Type& operator()(int row, int column);
  Type operator()(int row, int column) const;

  void Swap(Matrix& other);

  Matrix Transpose();
  double Determinant();
  Matrix CalcComplements();
  Matrix InverseMatrix();

  void PrintMatrix();

 protected:
  int ToTriangleMatrix_();
  void SwapRowMatrix_(int first_row, int second_row);
  void DivRowMatrix_(int index_row, Type del);
  void ZeroRowMatrix_(int index_row, int index_column, Type factor);
  bool EqType_(Type first, Type second);

  double GetMinorMatrix_(int skip_row, int skip_column);

 private:
  int m_rows;
  int m_columns;
  Type** m_matrix;
};

#include "matrix.inl"
