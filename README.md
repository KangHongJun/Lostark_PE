# Lostark_PE

curl 연결 = CLion(CMake)
 - 소스 아카이브 버전 다운로드
 - $projects\Windows\VC14.30에 있는 sin파일 빌드
 
 ``` cmake
set(CURL "E:/curl-7.87.0/")
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})
include_directories(${CURL_SOURCE_DIR})
target_link_libraries(pj ${CURL_LIBRARIES})
 ```
  
 libcurl 사용 주의점
  - callback 사용시 curl_easy_perform이전에 값을 담아야 한다.
  - 출력 시 한글이 깨지는 현상이 있었는데 디버깅 시에는 출력이 정상이었고, \123 형태로 출력 -> <세계지원을 위해 Unicode UTF-8 설정 체크><br>
    (다른 설정도 건들였지만 최종적으로 위 설정을 통해 적용됨)
