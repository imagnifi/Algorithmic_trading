#include "controller.h"
namespace s21 {

Controller::Controller() {}
Controller::~Controller() {}

void Controller::Init(GraphConfig &config) {
  if (!model_ || model_->GetData().size() == 0) {
    if (config.graph_type_ == SPLINE)
      model_ = std::make_unique<InterpolationSplines>(InterpolationSplines());
    else if (config.graph_type_ == PONINOMIAL)
      model_ = std::make_unique<Interpolation>(Interpolation());
    else if (config.graph_type_ == APPROXIMATION)
      model_ = std::make_unique<Approximation>(Approximation());
  } else {
    std::vector<GraphPoint> tmp_data(model_->GetData());
    model_->ClearData();
    Init(config);
    model_->SetData(tmp_data);
  }
}

void Controller::LoadFromFile(const std::string &path) {
  s21::Parser PS;
  PS.Open(path);
  if (PS.IsOpen()) {
    ClearData();
    DataVector dv = PS.ReadFileWhole();
    GraphVector gv = ConvertDataToGraph(dv);
    FillWeekends(gv);
    model_->SetData(gv);
    //    model_->GetData();
  }
}

void Controller::FillWeekends(s21::GraphVector &vec) {
  GraphVector::iterator iter = vec.begin();
  double step = 24 * 3600;
  while ((iter) != vec.end()) {
    GraphVector::iterator iter2(iter);
    ++iter2;
    if (iter2 != vec.end() && iter2->first - iter->first > 1.5 * step) {
      GraphPoint p(iter->first + step, iter->second);
      vec.insert(iter2, p);
      iter = vec.begin();
    }
    ++iter;
  }
};

auto Controller::ConvertDataToGraph(DataVector data_vector) -> GraphVector {
  std::vector<DataPoint>::iterator it = data_vector.begin();
  std::vector<DataPoint>::iterator it_end = data_vector.end();
  GraphVector res = GraphVector(0);
  GraphPoint res_point;
  while (it != it_end) {
    res_point.first = mktime(&it->first);
    res_point.second = it->second;
    res.push_back(res_point);
    ++it;
  }
  return res;
}

auto Controller::Proceed(GraphConfig const &config) -> GraphVector {
  return model_->Proceed(config);
}

double Controller::Proceed(double x) {
  if (!model_) throw std::invalid_argument("Data is empty");
  return model_->Proceed(x);
}

auto Controller::GetData() -> const GraphVector & { return model_->GetData(); };
void Controller::ClearData() { model_->ClearData(); }
auto Controller::IsData() -> bool {
  return model_ && model_->GetData().size() > 0;
}

}  // namespace s21
