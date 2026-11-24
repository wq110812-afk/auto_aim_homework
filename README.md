# auto_aim_homework

## 结构

```text
auto_aim_homework/
├── configs/auto_aim.yaml   # 调参参数文件（单一 yaml，分节）
├── tasks/auto_aim/         # armor / classifier / detector 已提供；solver 为作业任务
├── io/                     # 海康相机（HikRobot）
├── tools/                  # exiter, img_tools, logger, plotter, thread_safe_queue, yaml
├── src/main.cpp            # 作业：主程序（需自己实现，可执行文件 standard）
├── tests/smoke_test.cpp    # 冒烟测试（离线视频全帧统计检出/解算帧数）
└── assets/
    ├── tiny_resnet.onnx    # 编号识别模型
    └── demo/demo.avi       # 离线调试视频（687 帧，约 23 秒）
```

## 编译

```bash
cmake -B build
make -C build -j`nproc`
```

依赖：OpenCV、Eigen3、fmt、spdlog、yaml-cpp、nlohmann-json。

## 运行（完成作业后）

```bash
# 海康相机（真机，默认，曝光/增益由 yaml 控制）
./build/standard

# 离线视频调试（yaml 中 source_type: "video"）
./build/standard configs/auto_aim.yaml
```

按 `Esc` 或 `q` 退出。识别结果（四块装甲板矩形）只在窗口显示。

## 冒烟测试

```bash
./build/smoke_test            # 处理视频所有帧
```

`smoke_test` 会处理视频中的所有帧并统计检出帧数与 PnP 解算帧数
（注：solver 未实现前，`pnp solved` 统计无意义，需先完成 solver）。

## 调参

所有可调参数集中在 `configs/auto_aim.yaml`，分节说明：

- `enemy_color`：敌方颜色（我方红色步兵 -> 敌方蓝色）。
- `source` 相关：海康相机（`hikrobot`）/ 离线视频（`video`）切换。
- `detector` 相关：二值化阈值、灯条几何、装甲板几何、编号识别置信度等传统识别参数。
- `solver` 相关：相机内参、畸变系数；装甲板 3D 尺寸（宽 133mm / 高 57mm）与车心距
  （210mm）为代码内固定值，不参数化。
- 线程：主程序暂为单线程直读帧（无线程并行），离线视频按原始帧率限速播放。
- 绘制：识别到的装甲板绿色、其余三块黄色，线宽2，不参数化。

## 排查与调参指南

### 1. 识别不到装甲板，优先检查这些

- **`enemy_color` 是否正确**（最常见原因）。我方是红色步兵、敌方蓝色步兵时，
  `enemy_color` 必须为 `"blue"`，否则检测器会把蓝色灯条全部过滤掉。
- **二值化阈值 `threshold`**。灯条过曝（很亮）时阈值要调高（实测 demo 视频阈值
  150~180 检出最多，250 也能检出）；画面整体偏暗时调低（如 60~100）。
- **`min_lightbar_length`**。低分辨率/远距离时灯条像素长度小，阈值 8 可能偏大，
  可调小（如 4~6）。
- **曝光/增益**。海康相机曝光/增益不合适时画面会偏暗或过曝，导致阈值内无亮像素，见下节。

### 2. 海康相机曝光 / 增益调节

参数沿用原工程海康相机参数，在 `configs/auto_aim.yaml` 的“海康相机参数”节，重启 `standard` 生效：

```yaml
camera_name: "hikrobot"
exposure_ms: 2     # 曝光时间(ms)，越大画面越亮
gain: 16           # 增益，越大画面越亮
vid_pid: "2bdf:0001"
```

海康 SDK 在 `io/hikrobot/hikrobot.cpp` 中会关闭自动曝光/自动增益
（`ExposureAuto=OFF`、`GainAuto=OFF`），直接写入 `ExposureTime`（µs）与 `Gain`。
调好后如果画面还是太暗/太亮，再配合调整 `threshold`。

> 注意：`camera_matrix` / `distort_coeffs` 目前沿用原工程海康相机的标定值，
> 与 `io/hikrobot` 配套使用；如果换相机需要重新标定。

### 3. 测试视频“几秒就播完”

demo 视频共 687 帧（约 23 秒）。
主程序默认按视频原始帧率播放，`configs/auto_aim.yaml` 中可调：

```yaml
video_playback_fps: 0   # 0 = 按视频自带帧率；也可写具体数值
video_loop: true        # true = 播完循环
```

### 4. 海康相机打不开

`io/hikrobot` 用海康 SDK 枚举 USB 设备（`MV_CC_EnumDevices`），打不开时日志会出现
`Not found camera!` / `MV_CC_OpenDevice failed`，按顺序排查：

- **设备没识别**：用海康官方 MVS 软件确认相机能被枚举到；检查 USB 线和接口。
- **`vid_pid` 不对**：`configs/auto_aim.yaml` 中 `vid_pid` 需匹配相机实际标识
  （原工程为 `"2bdf:0001"`）。
- **权限不足**：`sudo usermod -aG video $USER` 后重新登录，或检查 `/dev/bus/usb` 权限。
- **相机被占用**：先关闭海康 MVS 等其他占用相机的程序。
- **没有海康相机时**：用离线视频调试（yaml 中 `source_type: "video"`），不影响 detector/solver 链路验证。

### 5. 快速验证是否检出

```bash
./build/smoke_test        # 默认处理前 100 帧，日志输出检出帧数
```

用 demo 视频 + `enemy_color: "blue"` + `threshold: 150` 实测：687 帧中约 300 帧能检出
蓝色装甲板（编号 one~five / sentry / base），PnP 全部解算成功。
