#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <cstring>

#include "../model/approximation.h"
#include "../model/interpolation.h"
#include "../model/interpolation_splines.h"
#include "../shared/parser/parser.h"

namespace s21 {

class Controller {
  /**
   * @brief The AlTraiding class - main class of model
   */

 public:
  Controller();
  ~Controller();
  void Init(GraphConfig& config);
  auto LoadFromFile(const std::string& path) -> void;
  void DeleteWeekends(s21::GraphVector& vec);
  void FillWeekends(s21::GraphVector& vec);
  auto GetData() -> const GraphVector&;
  auto SetData(const DataVector&) -> void;
  auto Proceed(const GraphConfig& config) -> GraphVector;
  auto Proceed(double) -> double;

  auto ClearData() -> void;
  auto ConvertDataToGraph(DataVector data_vector) -> GraphVector;
  auto IsData() -> bool;
  /**
   * @brief SetInterpol - connect view with model interpolation
   * @return data for represent to graph interpolation
   */
  //  auto SetInterpol() -> std::vector<std::pair<double, double>>;
  /**
   * @brief SetAproxim - connect view with model aproxima
   * @return data for represent to graph aproxima
   */
  //  auto SetAproxima() -> std::vector<std::pair<double, double>>;

  //  auto GetData() -> std::vector<std::pair<tm, double>> & { return
  //  main_data_; } auto Getseconds() -> std::vector<double>;

  //  auto IsData() -> bool;

 private:
  std::unique_ptr<AbstractGraph> model_;
  auto FromDateToSeconds(std::pair<tm, double>) -> double;
};

}  // namespace s21

#endif  // CONTROLLER_H_
