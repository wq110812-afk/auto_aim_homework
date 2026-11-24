#ifndef AUTO_AIM__CLASSIFIER_HPP
#define AUTO_AIM__CLASSIFIER_HPP

#include <opencv2/opencv.hpp>
#include <string>

#include "tasks/auto_aim/armor.hpp"

namespace auto_aim
{
// 装甲板编号识别：裁剪出的装甲板图案 -> 装甲板编号（one~five/sentry/outpost/base/not_armor）
class Classifier
{
public:
  explicit Classifier(const std::string & config_path);

  void classify(Armor & armor);

private:
  cv::dnn::Net net_;
};

}  // namespace auto_aim

#endif  // AUTO_AIM__CLASSIFIER_HPP
