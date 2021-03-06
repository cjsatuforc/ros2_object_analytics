/*
 * Copyright (c) 2018 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <string>
#include "object_analytics_node/tracker/tracking.hpp"

namespace object_analytics_node
{
namespace tracker
{
const int32_t Tracking::kAgeingThreshold = 16;

Tracking::Tracking(int32_t tracking_id, const std::string& name, const float& probability, const cv::Rect2d& rect)
  : tracker_(cv::Ptr<cv::Tracker>()), tracked_rect_(rect), obj_name_(name), probability_(probability),
    tracking_id_(tracking_id), detected_(false)
{
}

Tracking::~Tracking()
{
  if (tracker_.get())
  {
    tracker_.release();
  }
}

void Tracking::rectifyTracker(const cv::Mat& mat, const cv::Rect2d& t_rect, const cv::Rect2d& d_rect)
{
  if (tracker_.get())
  {
    tracker_.release();
  }
#if CV_VERSION_MINOR == 2
  tracker_ = cv::Tracker::create("MIL");
#else
  tracker_ = cv::TrackerMIL::create();
#endif
  tracker_->init(mat, t_rect);
  tracked_rect_ = t_rect;
  detected_rect_ = d_rect;
}

bool Tracking::updateTracker(const cv::Mat& mat)
{
  bool ret = tracker_->update(mat, tracked_rect_);
  ageing_++;
  return ret;
}

cv::Rect2d Tracking::getTrackedRect()
{
  return tracked_rect_;
}

std::string Tracking::getObjName()
{
  return obj_name_;
}

float Tracking::getObjProbability()
{
  return probability_;
}

cv::Rect2d Tracking::getDetectedRect()
{
  return detected_rect_;
}

int32_t Tracking::getTrackingId()
{
  return tracking_id_;
}

bool Tracking::isActive()
{
  return ageing_ < kAgeingThreshold;
}

bool Tracking::isDetected()
{
  return detected_;
}

void Tracking::clearDetected()
{
  detected_ = false;
}

void Tracking::setDetected()
{
  ageing_ = 0;
  detected_ = true;
}

}  // namespace tracker
}  // namespace object_analytics_node
