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

#ifndef CARTOGRAPHER_CLOUD_MAP_BUILDER_SERVER_INTERFACE_H
#define CARTOGRAPHER_CLOUD_MAP_BUILDER_SERVER_INTERFACE_H

#include <memory>

#include "cartographer/cloud/proto/map_builder_server_options.pb.h"
#include "cartographer/mapping/map_builder_interface.h"
#include "cartographer/metrics/family_factory.h"

namespace cartographer {
namespace cloud {

class MapBuilderServerInterface {
 public:
  virtual ~MapBuilderServerInterface() {}

  // gPRC 서버, 'LocalTrajectoryUploader'와 SLAM thread 시작. 
  // Starts the gRPC server, the 'LocalTrajectoryUploader' and the SLAM thread.
  virtual void Start() = 0;

  // 'MapBuilderServer'가 shutdown 되기를 기다림. 주의 : 서버가 shutdown 되거나 다른 thread가 이 함수를 위해서 'Shutdown()' 호출해야만 함.
  // Waits for the 'MapBuilderServer' to shut down. Note: The server must be
  // either shutting down or some other thread must call 'Shutdown()' for
  // this function to ever return.
  virtual void WaitForShutdown() = 0;

  // 모든 계산이 끝날때까지 대기하기(테스팅을 위해서)
  // Waits until all computation is finished (for testing).
  virtual void WaitUntilIdle() = 0;

  // gPRC 서버, 'LocalTrajectoryUploader'와 SLAM thread를 셧다운
  // Shuts down the gRPC server, the 'LocalTrajectoryUploader' and the SLAM
  // thread.
  virtual void Shutdown() = 0;
};

// MapBuilderServer을 위한 모든 metrics를 등록
// Registers all metrics for the MapBuilderServer.
void RegisterMapBuilderServerMetrics(metrics::FamilyFactory* factory);

// 실제 구현부를 가지는 MapBuilderServer를 반환
// Returns MapBuilderServer with the actual implementation.
std::unique_ptr<MapBuilderServerInterface> CreateMapBuilderServer(
    const proto::MapBuilderServerOptions& map_builder_server_options,
    std::unique_ptr<mapping::MapBuilderInterface> map_builder);

}  // namespace cloud
}  // namespace cartographer

#endif  // CARTOGRAPHER_CLOUD_MAP_BUILDER_SERVER_INTERFACE_H
