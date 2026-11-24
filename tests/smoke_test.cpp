#include <fmt/core.h>
#include <opencv2/opencv.hpp>
#include <yaml-cpp/yaml.h>

#include <algorithm>
#include <string>

#include "tasks/auto_aim/armor.hpp"
#include "tasks/auto_aim/detector.hpp"
#include "tasks/auto_aim/solver.hpp"
#include "tools/logger.hpp"
#include "tools/yaml.hpp"

const std::string keys =
  "{help h usage ? |      | 输出命令行参数说明}"
  "{@config-path   | configs/auto_aim.yaml | 位置参数，yaml配置文件路径 }";

int main(int argc, char * argv[])
{
  cv::CommandLineParser cli(argc, argv, keys);
  auto config_path = cli.get<std::string>(0);
  if (cli.has("help") || config_path.empty()) {
    cli.printMessage();
    return 0;
  }
  auto yaml = tools::load(config_path);

  auto video_path = yaml["video_path"].as<std::string>();

  auto_aim::Detector detector(config_path);
  auto_aim::Solver solver(config_path);

  cv::VideoCapture cap(video_path);
  if (!cap.isOpened()) {
    tools::logger()->error("smoke_test: failed to open video {}", video_path);
    return 1;
  }

  int total_frames = 0;
  int detected_frames = 0;
  int solved_frames = 0;

  // 处理视频中的所有帧
  cv::Mat img;
  while (cap.read(img)) {
    total_frames++;

    auto armors = detector.detect(img);
    if (armors.empty()) continue;
    detected_frames++;

    auto best = std::max_element(
      armors.begin(), armors.end(), [](const auto_aim::Armor & a, const auto_aim::Armor & b) {
        return a.confidence < b.confidence;
      });
    solver.solve(*best);
    if (best->xyz_in_camera[2] > 0) solved_frames++;
  }

  tools::logger()->info(
    "smoke_test: total {} frames, detected {} frames, pnp solved {} frames", total_frames,
    detected_frames, solved_frames);

  return detected_frames > 0 ? 0 : 1;
}
