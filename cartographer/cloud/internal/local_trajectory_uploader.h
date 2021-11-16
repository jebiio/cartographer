/*
 * Copyright 2018 The Cartographer Authors
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

#ifndef CARTOGRAPHER_CLOUD_INTERNAL_LOCAL_TRAJECTORY_UPLOADER_H
#define CARTOGRAPHER_CLOUD_INTERNAL_LOCAL_TRAJECTORY_UPLOADER_H

#include <memory>
#include <set>
#include <string>

#include "cartographer/cloud/proto/map_builder_service.pb.h"
#include "cartographer/mapping/proto/trajectory_builder_options.pb.h"
#include "cartographer/mapping/trajectory_builder_interface.h"
#include "grpc++/support/status.h"

namespace cartographer {
namespace cloud {

// Uploads sensor data batches to uplink server.
// Gracefully handles interruptions of the connection.
class LocalTrajectoryUploaderInterface {
 public:
  using SensorId = mapping::TrajectoryBuilderInterface::SensorId;

  virtual ~LocalTrajectoryUploaderInterface() = default;

  // upload thread를 시작   
  // Starts the upload thread.
  virtual void Start() = 0;

  // upload thread를 셧다운. 이 method는 셧다운이 완료될때까지 block된다.
  // Shuts down the upload thread. This method blocks until the shutdown is
  // complete.
  virtual void Shutdown() = 0;

  // upload 시킬 Add*DataRequest 메시지를 큐에 넣기
  // Enqueue an Add*DataRequest message to be uploaded.
  virtual void EnqueueSensorData(
      std::unique_ptr<proto::SensorData> sensor_data) = 0;

  // uplink에서 지정한 설정으로 새로운 trajectory를 생성. 리턴 'value'가 '!value.ok()'라는 것은 생성에 실패했다는 뜻이다.
  // Creates a new trajectory with the specified settings in the uplink. A
  // return 'value' with '!value.ok()' indicates that the creation failed.
  virtual grpc::Status AddTrajectory(
      const std::string& client_id, int local_trajectory_id,
      const std::set<SensorId>& expected_sensor_ids,
      const mapping::proto::TrajectoryBuilderOptions& trajectory_options) = 0;

  virtual grpc::Status FinishTrajectory(const std::string& client_id,
                                        int local_trajectory_id) = 0;
  virtual SensorId GetLocalSlamResultSensorId(
      int local_trajectory_id) const = 0;
};

// LocalTrajectoryUploader의 실제 구현을 반환
// Returns LocalTrajectoryUploader with the actual implementation.
std::unique_ptr<LocalTrajectoryUploaderInterface> CreateLocalTrajectoryUploader(
    const std::string& uplink_server_address, int batch_size,
    bool enable_ssl_encryption, bool enable_google_auth);

}  // namespace cloud
}  // namespace cartographer

#endif  // CARTOGRAPHER_CLOUD_INTERNAL_LOCAL_TRAJECTORY_UPLOADER_H
