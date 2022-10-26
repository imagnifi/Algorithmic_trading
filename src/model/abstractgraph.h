#ifndef ABSTRACTGRAPH_H
#define ABSTRACTGRAPH_H

#include <algorithm>
#include <ctime>
#include <vector>

#include "types.h"

namespace s21 {

class AbstractGraph {
 public:
  virtual ~AbstractGraph(){};
  virtual auto Proceed(const GraphConfig &config) -> GraphVector = 0;
  virtual auto GetData() -> const GraphVector & = 0;
  virtual auto SetData(const GraphVector &) -> void = 0;
  virtual auto ClearData() -> void = 0;
  virtual auto Proceed(double x) -> double = 0;

 private:
};
}  //  namespace s21
#endif  // ABSTRACTGRAPH_H
