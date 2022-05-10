// Copyright (c) 2021 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

#ifndef GESTURE_DATA_H_
#define GESTURE_DATA_H_

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "ai_msgs/msg/capture_targets.hpp"
#include "ai_msgs/msg/perception_targets.hpp"
#include "dnn_node/dnn_node.h"
#include "dnn_node/dnn_node_data.h"

using hobot::dnn_node::DNNInput;
using hobot::dnn_node::DnnNode;
using hobot::dnn_node::DnnNodeOutput;
using hobot::dnn_node::DnnNodePara;
using hobot::dnn_node::DNNResult;
using hobot::dnn_node::ModelTaskType;
using hobot::dnn_node::NV12PyramidInput;
using hobot::dnn_node::TaskId;

using hobot::dnn_node::Model;
using hobot::dnn_node::ModelInferTask;
using hobot::dnn_node::ModelManager;
using hobot::dnn_node::ModelRoiInferTask;

using hobot::dnn_node::DNNTensor;
using hobot::dnn_node::OutputParser;

using hobot::dnn_node::OutputDescription;

using ai_msgs::msg::PerceptionTargets;

namespace inference {

typedef enum {
  LAYOUT_NHWC = 0,
  LAYOUT_NCHW = 2,
  LAYOUT_NHWC_4W8C = 134,  // 适配老模型中的layout特殊处理
  LAYOUT_NONE = 255,
} TensorLayout;

typedef enum {
  IMG_TYPE_Y,
  IMG_TYPE_NV12,
  IMG_TYPE_NV12_SEPARATE,
  IMG_TYPE_YUV444,
  IMG_TYPE_RGB,
  IMG_TYPE_BGR,
  TENSOR_TYPE_S4,
  TENSOR_TYPE_U4,
  TENSOR_TYPE_S8,  // 8
  TENSOR_TYPE_U8,
  TENSOR_TYPE_F16,
  TENSOR_TYPE_S16,
  TENSOR_TYPE_U16,
  TENSOR_TYPE_F32,  // 13
  TENSOR_TYPE_S32,
  TENSOR_TYPE_U32,
  TENSOR_TYPE_F64,
  TENSOR_TYPE_S64,
  TENSOR_TYPE_U64,
  TENSOR_TYPE_MAX
} DataType;

// 浮点转换结果
typedef struct {
  TensorLayout layout;
  int dim[4];
  std::vector<float> value;  // batch * (nhwc), batch for resizer model
} FloatTensor;

/**
 * \~Chinese @brief 2D坐标点
 */
template <typename Dtype>
struct Point_ {
  inline Point_() {}
  inline Point_(Dtype x_, Dtype y_, float score_ = 1.0)
      : x(x_), y(y_), score(score_) {}

  Dtype x = 0;
  Dtype y = 0;
  float score = 1.0;
};
typedef Point_<float> HobotPoint;
typedef std::vector<HobotPoint> Landmarks;

class LandmarksResult : public DNNResult {
 public:
  std::vector<Landmarks> values;

  void Reset() override { values.clear(); }
};

using LandmarkVector = std::vector<std::shared_ptr<Landmarks>>;
using BoxVector = std::vector<std::shared_ptr<hbDNNRoi>>;

enum class gesture_type {
  Background = 0,
  ThumbUp = 2,  // 竖起大拇指
  Victory = 3,  // “V”手势
  Mute = 4,  // “嘘”手势
  Palm = 5,  // 手掌
  Okay = 11,  // OK手势
  ThumbLeft = 12,  // 大拇指向左
  ThumbRight = 13,  // 大拇指向右
  Awesome = 14  // 666手势
};

struct GestureRes {
  int value_ = 0;
  float score_ = 0;
  // 0: valid, -1: invalid
  int state_ = 0;
};

}  // namespace inference

#endif  // GESTURE_DATA_H_
