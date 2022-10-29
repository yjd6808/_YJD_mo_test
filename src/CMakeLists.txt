cmake_minimum_required(VERSION 3.12)
project(mo_test LANGUAGES CXX)

# 3.12부터 configure defends가 추가됨.
# 빌드시마다 자동으로 체크해서 변경된 파일 추가/삭제해줌
file(GLOB sources CONFIGURE_DEPENDS
     "*.h"
     "*.cpp"
     "*/*.h"
     "*/*.cpp"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

if (CMAKE_GENERATOR MATCHES "Visual Studio")
     # googletest에서 윈도우 런타임 라이브러리(MSVCRT.lib or dll)링크 방식을 
     # static 방식으로 링크하기 땜에 이 프로젝트도 MT 방식으로 링크하도록 함
     set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
     set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

     # 기본적으로 비주얼스튜디오는 소스파일에서 BOM을 이용해서 유니코드 형식인지 판단한다고한다.
     # 만약 없으면 파일 인코딩이 current user code page를 사용한다고 가정해서 컴파일한다고함.
     # https://stackoverflow.com/a/47704050/9488089
     # 따라서 컴파일 옵션으로 코드 페이지를 직접 지정해줘야한다.
     # 그리고 add_executable() 앞에서 적용해줘야함
     add_compile_options("$<$<C_COMPILER_ID:MSVC>:/source-charset:utf-8>")
     add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")

endif()

add_executable(mo_test ${sources})


if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
     # arm 리눅스에서는 std::thread 사용을 위해 pthread 링크가 필요함
     target_link_libraries(mo_test PRIVATE pthread)
endif()

target_include_directories(mo_test PRIVATE 
     ${CMAKE_SOURCE_DIR}/third_party/ready_thread/include 
     ${CMAKE_SOURCE_DIR}/third_party/googletest/googletest/include
)

target_link_libraries(mo_test PRIVATE 
     gtest 
     gtest_main
)

