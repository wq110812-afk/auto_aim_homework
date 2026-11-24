#ifndef AUTO_AIM__SOLVER_HPP
#define AUTO_AIM__SOLVER_HPP

#include <Eigen/Dense>  // 必须在opencv2/core/eigen.hpp上面
#include <opencv2/core/eigen.hpp>

#include <string>
#include <vector>

#include "tasks/auto_aim/armor.hpp"

namespace auto_aim
{
// 装甲板3D建模 + PnP解算：输出装甲板在相机系下的位置与姿态
class Solver
{
public:
  explicit Solver(const std::string & config_path);

  void solve(Armor & armor) const;

  // 将装甲板3D矩形投影回图像，得到四个角点的像素坐标
  std::vector<cv::Point2f> project_armor(const Armor & armor) const;
  std::vector<cv::Point2f> project_armor(
    const Eigen::Vector3d & xyz_in_camera, const Eigen::Matrix3d & ypr_in_camera) const;

private:
  cv::Mat camera_matrix_;
  cv::Mat distort_coeffs_;
  std::vector<cv::Point3f> object_points_;  // 装甲板3D模型（局部系：x=法线 y=水平 z=竖直向上）
};

}  // namespace auto_aim

#endif  // AUTO_AIM__SOLVER_HPP
