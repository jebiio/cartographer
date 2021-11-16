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

#include "cartographer/cloud/internal/handlers/add_fixed_frame_pose_data_handler.h"

#include "absl/memory/memory.h"
#include "async_grpc/rpc_handler.h"
#include "cartographer/cloud/internal/map_builder_context_interface.h"
#include "cartographer/cloud/internal/sensor/serialization.h"
#include "cartographer/cloud/proto/map_builder_service.pb.h"
#include "cartographer/sensor/fixed_frame_pose_data.h"
#include "cartographer/sensor/internal/dispatchable.h"
#include "google/protobuf/empty.pb.h"

namespace cartographer {
namespace cloud {
namespace handlers {

void AddFixedFramePoseDataHandler::OnSensorData(
    const proto::AddFixedFramePoseDataRequest& request) {
  // 'sensor_data_queue()'로 리턴되는 'BlockingQueue'는 이미 thread-safe하다.
  // 따라서 'MapBuilderContext'에 대한 동기화되지 않는 참조를 얻기에 충분하다.
  // The 'BlockingQueue' returned by 'sensor_data_queue()' is already
  // thread-safe. Therefore it suffices to get an unsynchronized reference to
  // the 'MapBuilderContext'.
  GetUnsynchronizedContext<MapBuilderContextInterface>()->EnqueueSensorData(
      request.sensor_metadata().trajectory_id(),
      sensor::MakeDispatchable(
          request.sensor_metadata().sensor_id(),
          sensor::FromProto(request.fixed_frame_pose_data())));

  // 'LocalTrajectoryUploader'에서 'BlockingQueue'는 thread-safe하다.
  // 따라서 'MapBuilderContext'에 대한 동기화되지 않은 레퍼런스를 얻기에 충분하다.
  // The 'BlockingQueue' in 'LocalTrajectoryUploader' is thread-safe.
  // Therefore it suffices to get an unsynchronized reference to the
  // 'MapBuilderContext'.
  if (GetUnsynchronizedContext<MapBuilderContextInterface>()
          ->local_trajectory_uploader()) {
    auto sensor_data = absl::make_unique<proto::SensorData>();
    *sensor_data->mutable_sensor_metadata() = request.sensor_metadata();
    *sensor_data->mutable_fixed_frame_pose_data() =
        request.fixed_frame_pose_data();
    GetUnsynchronizedContext<MapBuilderContextInterface>()
        ->local_trajectory_uploader()
        ->EnqueueSensorData(std::move(sensor_data));
  }
}

}  // namespace handlers
}  // namespace cloud
}  // namespace cartographer
