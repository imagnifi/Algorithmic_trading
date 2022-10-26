#include "parser.h"

namespace s21 {

void Parser::Open(const std::string &filename) {
  file = std::fstream(filename);
  if (!file.is_open())
    throw std::invalid_argument("file \"" + filename + "\" don't exist");
}

DataPoint Parser::ReadLine() {
  DataPoint res;
  std::string tmp;
  getline(file, tmp);
  res = ConvertString(tmp);
  return res;
}

DataVector Parser::ReadFileWhole() {
  std::vector<DataPoint> vec;
  if (!file.is_open()) throw std::runtime_error("file isn't open");
  std::string first_line;
  getline(file, first_line);
  if (first_line == "Date,Close") {
    while (!file.eof()) {
      DataPoint tmp = ReadLine();
      if (IsLine(tmp)) vec.push_back(tmp);
    }
  }
  return vec;
}

bool Parser::IsLine(std::pair<tm, double> src) {
  bool res = false;
  if (src.first.tm_year != 0 and src.first.tm_mday != 0) res = true;
  return res;
}

auto Parser::ConvertString(std::string str) -> DataPoint {
  DataPoint res{};
  if (str != "Date,Close" and !str.empty()) {
    char *buf = str.data();
    strptime(buf, "%Y-%m-%d", &res.first);
    if (res.first.tm_year == 0)
      throw std::invalid_argument("parser: date is invalid");

    char s2 = ',';
    size_t pos2 = str.find(s2);
    if (pos2 != std::string::npos) {
      std::string value;
      value.assign(str, pos2 + 1, str.length());
      res.second = std::stod(value.c_str());
    } else {
      throw std::invalid_argument("Parser: value is invalid");
    }
  }
  return res;
};
}  // namespace s21
