// 大作业：solver 实现（装甲板 3D 建模 + PnP 解算）
// 你需要按 solver.hpp 中的接口完成：
//   1. 构造函数：从 config_path 的 yaml 读取相机内参 camera_matrix 与畸变系数
//      distort_coeffs，并建立装甲板 3D 模型 object_points_
//      （局部系：x=法线、y=水平、z=竖直向上；尺寸：宽 0.133m、高 0.057m）
//   2. solve()：对传入的 Armor 用 solvePnP 解算，把结果写入
//      armor.xyz_in_camera（位置，m）与 armor.ypr_in_camera（旋转矩阵）
//   3. project_armor()：把装甲板 3D 模型投影回图像，得到四个角点的像素坐标

#include "solver.hpp"

#include <yaml-cpp/yaml.h>

#include <vector>

namespace auto_aim
{
Solver::Solver(const std::string & config_path)
{
  // TODO(作业): 读取 yaml 中的 camera_matrix / distort_coeffs，构建 object_points_
}

void Solver::solve(Armor & armor) const
{
  // TODO(作业): solvePnP，写入 armor.xyz_in_camera / armor.ypr_in_camera
}

std::vector<cv::Point2f> Solver::project_armor(const Armor & armor) const
{
  // TODO(作业): 可调用下方重载
  return {};
}

std::vector<cv::Point2f> Solver::project_armor(
  const Eigen::Vector3d & xyz_in_camera, const Eigen::Matrix3d & ypr_in_camera) const
{
  // TODO(作业): projectPoints 投影，返回四个角点像素坐标
  return {};
}

}  // namespace auto_aim
