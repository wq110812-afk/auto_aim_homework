// ============================================================
// 大作业：主程序 standard
// 目标：detector -> solver 整条链路：识别敌方红色装甲板，并标记出
//       四块装甲板位置（识别到的主装甲板标识不得同于其他装甲板）。
//
// 你需要完成（可参考 tests/smoke_test.cpp 的模块用法）：
//   1. 构造 auto_aim::Detector 与 auto_aim::Solver（都传入 config_path）
//   2. 输入源：按 yaml 的 source_type 切换
//        - "hikrobot"：io::Camera（海康相机，封装见 io/camera.hpp）
//        - "video"    ：cv::VideoCapture（离线视频）
//      封装成统一读取函数（如 read_frame(cv::Mat &)）
//   3. 主循环：
//        - 读帧 -> detector.detect(img) 得到候选装甲板
//        - 选置信度最高的一块 -> solver.solve() 解出相机系下位姿
//        - 画出识别到的装甲板（solver.project_armor 投影回图像）
//        - 以识别到的装甲板为基准，推出车体上其余三块装甲板并画出
//          （车体中心到装甲板中心距离 0.210m，绕车体竖直轴旋转 90/180/270 度）
//        - cv::imshow 显示，Esc 或 q 退出
//   4. 在 CMakeLists.txt 中把 standard 可执行目标加回来
//
// 提示：
//   - tools::draw_points / tools::logger 可直接使用
//   - 离线视频建议按帧率限速播放（避免几秒就播完）
//   - 识别不到时先检查 configs/auto_aim.yaml 的 enemy_color 与 threshold
// ============================================================

#include <opencv2/opencv.hpp>
#include <string>

#include "io/camera.hpp"
#include "tasks/auto_aim/armor.hpp"
#include "tasks/auto_aim/detector.hpp"
#include "tasks/auto_aim/solver.hpp"
#include "tools/exiter.hpp"
#include "tools/img_tools.hpp"
#include "tools/logger.hpp"
#include "tools/yaml.hpp"

const std::string keys =
  "{help h usage ? |                        | 输出命令行参数说明}"
  "{@config-path   | configs/auto_aim.yaml | 位置参数，yaml配置文件路径 }";

int main(int argc, char * argv[])
{
  tools::Exiter exiter;

  cv::CommandLineParser cli(argc, argv, keys);
  auto config_path = cli.get<std::string>(0);
  if (cli.has("help") || config_path.empty()) {
    cli.printMessage();
    return 0;
  }

  auto yaml = tools::load(config_path);

  auto source_type = yaml["source_type"].as<std::string>();
  auto video_path = yaml["video_path"].as<std::string>();

  // TODO(作业): 从这里开始实现——
  //   1. 构造 Detector / Solver
  //   2. 实现输入源与 read_frame
  //   3. 主循环：识别 -> 解算 -> 绘制四块装甲板 -> 显示
  //   4. Esc/q 退出，释放资源

  return 0;
}
