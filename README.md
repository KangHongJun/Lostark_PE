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
 
 
