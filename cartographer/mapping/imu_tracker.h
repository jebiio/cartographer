/*
 * Copyright 2016 The Cartographer Authors
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

#ifndef CARTOGRAPHER_MAPPING_IMU_TRACKER_H_
#define CARTOGRAPHER_MAPPING_IMU_TRACKER_H_

#include "Eigen/Geometry"
#include "cartographer/common/time.h"

namespace cartographer {
namespace mapping {

// IMU로부터 각속도와 선형가속을 사용하여 orientation을 추적.
// 평균 선형 가속(천천히 움직인다는 가정)은 중력을 직접 측정하고 yaw는 drift가 있지만 roll/pitch는 drift 없음.
// Keeps track of the orientation using angular velocities and linear
// accelerations from an IMU. Because averaged linear acceleration (assuming
// slow movement) is a direct measurement of gravity, roll/pitch does not drift,
// though yaw does.
class ImuTracker {
 public:
  ImuTracker(double imu_gravity_time_constant, common::Time time);

  // 주어진 시간에 앞서 이를 반영하기 위해서 orientation을 업데이트
  // Advances to the given 'time' and updates the orientation to reflect this.
  void Advance(common::Time time);

  // 읽는 IMU로부터 업데이트(IMU 프레임에서)
  // Updates from an IMU reading (in the IMU frame).
  void AddImuLinearAccelerationObservation(
      const Eigen::Vector3d& imu_linear_acceleration);
  void AddImuAngularVelocityObservation(
      const Eigen::Vector3d& imu_angular_velocity);

  // 현재 시간 얻기
  // Query the current time.
  common::Time time() const { return time_; }

  // 현재 orientation estimate 얻기
  // Query the current orientation estimate.
  Eigen::Quaterniond orientation() const { return orientation_; }

 private:
  const double imu_gravity_time_constant_;
  common::Time time_;
  common::Time last_linear_acceleration_time_;
  Eigen::Quaterniond orientation_;
  Eigen::Vector3d gravity_vector_;
  Eigen::Vector3d imu_angular_velocity_;
};

}  // namespace mapping
}  // namespace cartographer

#endif  // CARTOGRAPHER_MAPPING_IMU_TRACKER_H_
