#include <cmath>
#include <limits>
#include <thread>

#include "matrix.hpp"

template <class Type>
Matrix<Type>::Matrix(int rows, int columns)
    : m_rows(rows), m_columns(columns), m_matrix(nullptr) {
  if (rows < 1 || columns < 1) {
    throw std::invalid_argument("Matrix:");
  }
  m_matrix = new Type*[m_rows];
  for (int i = 0; i < m_rows; i++) {
    m_matrix[i] = new Type[m_columns]{};
  }
}

template <class Type>
Matrix<Type>::Matrix() : m_rows(1), m_columns(1) {
  m_matrix = new Type*[1];
  m_matrix[0] = new Type[1]{};
}

template <class Type>
Matrix<Type>::Matrix(Matrix const& other)
    : m_rows(0), m_columns(0), m_matrix(nullptr) {
  *this = other;
}

template <class Type>
Matrix<Type>::Matrix(Matrix&& other)
    : m_rows(0), m_columns(0), m_matrix(nullptr) {
  *this = std::move(other);
}

template <class Type>
Matrix<Type>::~Matrix() {
  clear();
}

template <class Type>
void Matrix<Type>::clear() {
  if (m_matrix != nullptr) {
    for (int i = 0; i < m_rows; i++) {
      delete[] m_matrix[i];
    }
    delete[] m_matrix;
    m_matrix = nullptr;
    m_rows = 0;
    m_columns = 0;
  }
}

template <class Type>
Matrix<Type>& Matrix<Type>::operator=(Matrix const& other) {
  if (this != &other) {
    clear();
    m_rows = other.m_rows;
    m_columns = other.m_columns;

    m_matrix = new Type*[m_rows];
    for (int i = 0; i < m_rows; i++) {
      m_matrix[i] = new Type[m_columns]{};
      for (int j = 0; j < m_columns; j++) {
        m_matrix[i][j] = other.m_matrix[i][j];
      }
    }
  }
  return *this;
}

template <class Type>
Matrix<Type>& Matrix<Type>::operator=(Matrix<Type>&& other) {
  if (this == &other) {
    throw std::invalid_argument("operator=:");
  }
  clear();
  Swap(other);
  return *this;
}

template <class Type>
int Matrix<Type>::get_rows() {
  return m_rows;
}

template <class Type>
int Matrix<Type>::get_columns() {
  return m_columns;
}

template <class Type>
void Matrix<Type>::set_rows(int rows) {
  Matrix current(rows, m_columns);
  for (int i = 0; i < current.m_rows && i < m_rows; i++) {
    for (int j = 0; j < current.m_columns; j++) {
      current.m_matrix[i][j] = m_matrix[i][j];
    }
  }
  *this = std::move(current);
}

template <class Type>
void Matrix<Type>::set_columns(int columns) {
  Matrix current(m_rows, columns);
  for (int i = 0; i < current.m_rows; i++) {
    for (int j = 0; j < current.m_columns && j < m_columns; j++) {
      current.m_matrix[i][j] = m_matrix[i][j];
    }
  }
  *this = std::move(current);
}

template <class Type>
bool Matrix<Type>::eq_matrix(Matrix const& other) {
  if (m_rows != other.m_rows || m_columns != other.m_columns) {
    return false;
  }
  for (int i = 0; i < m_rows; i++) {
    if (memcmp(m_matrix[i], other.m_matrix[i], m_columns * sizeof(double)) !=
        0) {
      return false;
    }
  }
  return true;
}

template <class Type>
void Matrix<Type>::sum_matrix(Matrix const& other) {
  if (m_rows != other.m_rows || m_columns != other.m_columns) {
    throw std::invalid_argument("sum_matrix:");
  }
  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_columns; j++) {
      m_matrix[i][j] += other.m_matrix[i][j];
    }
  }
}

template <class Type>
void Matrix<Type>::sub_matrix(Matrix const& other) {
  if (m_rows != other.m_rows || m_columns != other.m_columns) {
    throw std::invalid_argument("sub_matrix:");
  }
  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_columns; j++) {
      m_matrix[i][j] -= other.m_matrix[i][j];
    }
  }
}

template <class Type>
void Matrix<Type>::mul_number(Type num) {
  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_columns; j++) {
      m_matrix[i][j] *= num;
    }
  }
}

template <class Type>
void Matrix<Type>::mul_matrix(Matrix const& other) {
  if (m_columns != other.m_rows) {
    throw std::invalid_argument("mul_matrix:");
  }

  Matrix result(m_rows, other.m_columns);
  for (int i = 0; i < result.m_rows; i++) {
    for (int j = 0; j < result.m_columns; j++) {
      for (int k = 0; k < m_columns; k++) {
        result.m_matrix[i][j] += m_matrix[i][k] * other.m_matrix[k][j];
      }
    }
  }

  *this = std::move(result);
}

template <class Type>
Matrix<Type> Matrix<Type>::operator+(Matrix const& other) {
  Matrix new_matrix(*this);
  new_matrix.sum_matrix(other);
  return new_matrix;
}

template <class Type>
Matrix<Type> Matrix<Type>::operator-(Matrix const& other) {
  Matrix new_matrix(*this);
  new_matrix.sub_matrix(other);
  return new_matrix;
}

template <class Type>
Matrix<Type> Matrix<Type>::operator*(Matrix const& other) {
  Matrix new_matrix(*this);
  new_matrix.mul_matrix(other);
  return new_matrix;
}

template <class Type>
Matrix<Type> Matrix<Type>::operator*(Type num) {
  Matrix<Type> new_matrix(*this);
  new_matrix.mul_number(num);
  return new_matrix;
}

template <class Type>
Matrix<Type>& Matrix<Type>::operator+=(Matrix const& other) {
  sum_matrix(other);
  return *this;
}

template <class Type>
Matrix<Type>& Matrix<Type>::operator-=(Matrix const& other) {
  sub_matrix(other);
  return *this;
}

template <class Type>
Matrix<Type>& Matrix<Type>::operator*=(Matrix const& other) {
  mul_matrix(other);
  return *this;
}

template <class Type>
Matrix<Type>& Matrix<Type>::operator*=(Type num) {
  mul_number(num);
  return *this;
}

template <class Type>
bool Matrix<Type>::operator==(Matrix const& other) {
  return eq_matrix(other);
}

template <class Type>
Type& Matrix<Type>::operator()(int row, int column) {
  if (row < 0 || row >= m_rows || column < 0 || column >= m_columns) {
    throw std::out_of_range("operator():");
  }
  return m_matrix[row][column];
}

template <class Type>
Type Matrix<Type>::operator()(int row, int column) const {
  if (row < 0 || row >= m_rows || column < 0 || column >= m_columns) {
    throw std::out_of_range("operator():");
  }
  return m_matrix[row][column];
}

template <class Type>
void Matrix<Type>::Swap(Matrix& other) {
  if (this != &other) {
    std::swap(m_rows, other.m_rows);
    std::swap(m_columns, other.m_columns);
    std::swap(m_matrix, other.m_matrix);
  }
}

template <class Type>
Matrix<Type> Matrix<Type>::Transpose() {
  Matrix new_matrix(m_columns, m_rows);
  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_columns; j++) {
      new_matrix.m_matrix[j][i] = m_matrix[i][j];
    }
  }
  return new_matrix;
}

template <class Type>
double Matrix<Type>::Determinant() {
  if (m_rows != m_columns) {
    throw std::invalid_argument("determinant:");
  }

  Matrix copy_matrix(*this);
  double determinant = ToTriangleMatrix_();
  for (int i = 0; i < m_rows; ++i) {
    determinant *= m_matrix[i][i];
  }
  *this = std::move(copy_matrix);

  return determinant;
}

template <class Type>
double Matrix<Type>::GetMinorMatrix_(int skip_row, int skip_column) {
  Matrix minor_matrix(m_rows - 1, m_columns - 1);

  for (int i = 0, row = 0; i < m_rows; ++i) {
    if (i != skip_row) {
      for (int j = 0, column = 0; j < m_columns; ++j) {
        if (j != skip_column) {
          minor_matrix(row, column) = m_matrix[i][j];
          ++column;
        }
      }
      ++row;
    }
  }
  return minor_matrix.Determinant();
}

template <class Type>
Matrix<Type> Matrix<Type>::CalcComplements() {
  if (m_rows != m_columns) {
    throw std::invalid_argument("CalcComplements:");
  }
  Matrix complements_matrix(*this);

  unsigned count_thread = std::thread::hardware_concurrency() - 1;

  auto lambda = [&](int i, int j) {
    complements_matrix(i, j) = pow(-1, i + j) * GetMinorMatrix_(i, j);
  };

  for (int i = 0; i < m_rows; ++i) {
    for (int j = 0; j < m_columns;) {
      std::vector<std::thread> multithread;
      while (multithread.size() != count_thread && j < m_columns) {
        multithread.push_back(std::thread(lambda, i, j));
        ++j;
      }
      for (size_t k = 0; k < multithread.size(); ++k) {
        multithread[k].join();
      }
    }
  }
  return complements_matrix;
}

template <class Type>
Matrix<Type> Matrix<Type>::InverseMatrix() {
  double determinant = Determinant();
  if (EqType_(determinant, 0.0)) {
    throw std::invalid_argument("InverseMatrix: determinan is zero");
  }
  Matrix inverse_matrix(std::move(CalcComplements()));
  inverse_matrix = std::move(inverse_matrix.Transpose());
  inverse_matrix.mul_number(1.0 / determinant);
  return inverse_matrix;
}

template <class Type>
void Matrix<Type>::PrintMatrix() {
  for (int row = 0; row < m_rows; ++row) {
    for (int column = 0; column < m_columns; ++column) {
      if (column == m_columns - 1) {
        std::cout << m_matrix[row][column];
      } else {
        std::cout << m_matrix[row][column] << " ";
      }
    }
    std::cout << std::endl;
  }
  std::cout << "----------------" << std::endl;
}

template <class Type>
int Matrix<Type>::ToTriangleMatrix_() {
  int result = 1;

  for (int column = 0; column < m_columns; ++column) {
    for (int row = column; row < m_rows; ++row) {
      if (row == column) {
        // если равняется 0, проверяем есть ли возможность поменять строчки
        if (EqType_(m_matrix[row][column], 0.0)) {
          bool swap = false;
          for (int k = row + 1; k < m_rows; ++k) {
            if (!EqType_(m_matrix[k][column], 0.0)) {
              result *= -1;
              SwapRowMatrix_(row, k);
              swap = true;
              break;
            }
          }
          if (swap == false) {
            break;
          }
        }
      } else {
        Type factor = m_matrix[row][column] / m_matrix[column][column] * -1.0;
        ZeroRowMatrix_(row, column, factor);
      }
    }
  }

  return result;
}

template <class Type>
void Matrix<Type>::SwapRowMatrix_(int first_row, int second_row) {
  for (int i = 0; i < m_columns; ++i) {
    Type tmp = m_matrix[first_row][i];
    m_matrix[first_row][i] = m_matrix[second_row][i];
    m_matrix[second_row][i] = tmp;
  }
}

template <class Type>
void Matrix<Type>::DivRowMatrix_(int index_row, Type del) {
  for (int column = 0; column < m_columns; ++column) {
    m_matrix[index_row][column] /= del;
  }
}

template <class Type>
void Matrix<Type>::ZeroRowMatrix_(int index_row, int index_column,
                                  Type factor) {
  for (int j = 0; j < m_columns; ++j) {
    Type value = m_matrix[index_column][j] * factor;
    m_matrix[index_row][j] += value;
  }
}

template <class Type>
bool Matrix<Type>::EqType_(Type first, Type second) {
  if (fabs(first - second) <= std::numeric_limits<double>::epsilon() *
                                  fmax(fabs(first), fabs(second))) {
    return true;
  }
  return false;
}
