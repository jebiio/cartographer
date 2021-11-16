# Cartographer
Cartographer는 시스템으로 다양한 플랫폼 및 센서 설정을 통해서 2D와 3D로 SLAM(real-time simultaneous localization and mapping)을 제공한다.
## 기술 개요
* 하이레벨 시스템 개요
![](https://github.com/cartographer-project/cartographer/blob/master/docs/source/high_level_system_overview.png)

## 시작하기
* Cartographer는 독립적인 C++ 라이브러리이다. 빠르게 시작할려면 ROS 통합된 버전을 사용할 수 있다.
### ROS로 시작하기
* ROS 통합 버전은 [Cartographer ROS 저장소](https://github.com/cartographer-project/cartographer_ros)에서 제공한다. ROS로 사용하는 방법에 대한 문서는 [여기](https://google-cartographer-ros.readthedocs.io/en/latest/)를 참고하자.

### ROS없이 시작하기
* 독립 라이브러리와 여러분의 시스템과 통합하는 방법을 익히기 위해서는 ROS 버전으로 시작하는 것을 현재 가장 최선의 방법이다.

## 시스템 요구사항
* 16bit, Intel i7
* 16GB RAM
* Ubuntu 20.04
* gcc version 6.3.0, 7.5.0, 9.3.0

## 논문
* [Real-Time Loop Closure in 2D LIDAR SLAM](https://research.google/pubs/pub45466/)
  * W. Hess, D. Kohler, H. Rapp, and D. Andor