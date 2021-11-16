# 용어
## Frames
* global map frame
  * global SLAM 결과를 표현하는 frame
  * 모든 loop closure와 optimization 결과를 포함하는 fixed map frame이다.
  * 이 프레임에서 다른 프레임으로 변환은 새로운 최적화 결과가 유효할때 점프가 가능하다. z축은 위, 따라서 중력가속도는 -z 방향이 된다. 가속도로 측정된 중력관련 컴포넌트는 +z 방향이다. 
* local map frame
  * local SLAM 결과로 표현되는 frame
  * loop closure와 pose graph 최적화를 제외한 fixed map frame이다.
  * 시간내에 주어진 point에 대해서 이 local map frame과 global map frame은 변경될 수 있지만 이 frame과 다른 frame들 사이에서 변환은 변경되지 않는다.
* submap frame
  * 각 submap은 독립적인 fixed frame이다.
* tracking frame
  * sensor data가 표현되는 frame
  * fixed 되어 있지 않고 시간에 따라 변경된다.
  * 다른 trajectory에 따라서 달라진다.
* gravity-aligned frame
  * 2D에서만 사용
  * 

## Transforms
* local_pose
  * tracking frame의 data -> local map frame으로 transform
* globla_pose
  * tracking frame의 data -> global map frame으로 transform
* local_submap_pose
  * submap frame의 data -> local map frame으로 transform
* global_submap_pose
  * submap frame의 data -> global map frame으로 transform

----
# 이외
* pbstream
  * 파일 포맷
  * 일종의 압축된 protobuf 파일
  * 내부 state를 직렬화
  * Cartographer가 내부적으로 사용하는 자료구조의 snapshot
* 