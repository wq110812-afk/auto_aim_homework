#ifndef AUTO_AIM__DETECTOR_HPP
#define AUTO_AIM__DETECTOR_HPP

#include <list>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

#include "tasks/auto_aim/armor.hpp"
#include "tasks/auto_aim/classifier.hpp"

namespace auto_aim
{
// 传统方法装甲板识别：二值化 -> 灯条 -> 灯条配对 -> 装甲板 -> 编号识别过滤
class Detector
{
public:
  explicit Detector(const std::string & config_path);

  std::list<Armor> detect(const cv::Mat & bgr_img);

private:
  Classifier classifier_;

  Color enemy_color_;
  double threshold_;
  double max_angle_error_;
  double min_lightbar_ratio_, max_lightbar_ratio_;
  double min_lightbar_length_;
  double min_armor_ratio_, max_armor_ratio_;
  double max_side_ratio_;
  double min_confidence_;
  double max_rectangular_error_;

  std::string save_path_;

  bool check_geometry(const Lightbar & lightbar) const;
  bool check_geometry(const Armor & armor) const;
  bool check_name(const Armor & armor) const;

  Color get_color(const cv::Mat & bgr_img, const std::vector<cv::Point> & contour) const;
  cv::Mat get_pattern(const cv::Mat & bgr_img, const Armor & armor) const;
  cv::Point2f get_center_norm(const cv::Mat & bgr_img, const cv::Point2f & center) const;

  void save(const Armor & armor) const;
};

}  // namespace auto_aim

#endif  // AUTO_AIM__DETECTOR_HPP
