#ifndef PASER1_H
#define PASER1_H

#include <ctime>
#include <fstream>
#include <string>
#include <vector>

#include "../../model/types.h"

namespace s21 {

class Parser {
 public:
  Parser() = default;
  ~Parser() = default;
  inline auto IsOpen() const -> bool { return file.is_open(); };
  auto Open(const std::string &filename) -> void;
  auto ReadFileWhole() -> DataVector;

 private:
  std::fstream file;
  auto ReadLine() -> DataPoint;
  /**
   * @brief IsLine - check line for null contetnt
   * @param src - return value of ReadLine function
   * @return true - false if null content
   */
  auto IsLine(DataPoint src) -> bool;
  auto ConvertString(std::string str) -> DataPoint;
};
}  //  namespace s21

#endif  // PASER1_H
