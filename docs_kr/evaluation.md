# Evaluation
* 성능 평가는 SLAM 개발에서 중요한 부분
* 이런 목적을 위해서 Cartographer는 내장 도구를 제공하여 튜닝 프로세스나 품질 보장하는데 사용 가능
* 이 도구를 이용하면 ground truth가 유효하지 않은 환경에서도 SLAM 결과를 평가할 수 있다.
* KITTI dataset이나 TUM RGB-D dataset 같은 공용 SLAM benchmark 도구와는 대비된다. 정밀 ground truth 상태(GPS-RTK, motion capture)가 참조로 유효하다.

## 개념
* 2가지 단계로 구성되어 있다.
  1. "ground truth" 관계의 자동 생성
  2. 생성된 ground truth에 대한 test data 평가

* 평가는 pose 관계를 기반으로 아래 논문의 기준을 따른다. trajectory의 pose를 직접 관련 ground truth pose와 비교하는 대신에 ground truth trajectory 내에 2개 trajectory node들 사이의 관계에 대해서 probe data에서 2개 trajectory node 사이의 상대 pose를 비교
* ground truth 생성 방법 : Cartographer에서 loop closure로 trajectory로부터 ground truth relation을 생성
* loop closure와 optimized trajectory를 ground truth 생성에 대한 입력으로
* 다음 기준을 만족시키는 loop closure constraints로부터 relation을 선택
  * min_covered_distance : loop closure가 자동 생성된 ground truth를 위한 후보로 고려하기 전에 미터 단위로 최소 covered distance
  * outlier_threshold_meters : outlier라고 판단하는 거리
  * outlier_threshold_radians : outlier라고 판단하는 각도

* 이러한 요구사항을 충족하는 이웃하는 trajectory node의 pose relation은 완전히 optimized trajectory내에서 지역적으로 보정이 되었다고 가정한다. 비록 다른 소스로부터 독립적인 입력의 관점에서는 ground truth가 아니라고 보지만 loop closure 최적화 없이 생성된 local SLAM 결과의 품질을 평가하는데 사용할 수 있다.
* 다음 그림은 개념을 설명한다. 왼쪽은 ground truth 관계를 완전히 최적화된 trajectory의 trajectory node들 사이의 연결을 녹색으로 표시하였다. 오른쪽은 최적화 하지 않은 trajectory에서 관련 관계를 빨간색으로 표시하였다.
* 실제 측정 기준은 녹색(ground truth)과 붉은색(probe) 사잉의 관계의 차이를 계산하는 것이다.

## Advantage & Limitation
* 첫번째 명확한 장점은 ground truth 셋업에 비교해서 더 쉬워진 데이터 수집 프로세스이다. 
* 다른 장점은 SLAM 시스템이 커스텀으로 센서 설정 가능
* 전체 SLAM 시스템의 정확성을 측정하는데는 적합하지 않음. 

## 방법
* 완전 최적화 trajectory의 직렬화된 state가 주어지면, ground truth relation은 다음 명령으로 생성할 수 있다.
```
cd <build>
./cartographer_autogenerate_ground_truth -pose_graph_filename optimized.pbstream -output_filename relations.pbstream -min_covered_distance 100 -outlier_threshold_meters 0.15 -outlier_threshold_radians 0.02
```
* 비최적화 trajectory test.pbstream은 생성된 relation에 대해서 평가할 수 있다.
```
./cartographer_compute_relations_metrics -relations_filename relations.pbstream -pose_graph_filename test.pbstream
```
* 이는 다음과 같은 결과를 출력한다.
```
Abs translational error 0.01944 +/- 0.01819 m
Sqr translational error 0.00071 +/- 0.00189 m^2
Abs rotational error 0.11197 +/- 0.12432 deg
Sqr rotational error 0.02799 +/- 0.07604 deg^2
```
