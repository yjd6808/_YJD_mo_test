

cmake_minimum_required(VERSION 3.18)
project(${CMAKE_PROJECT_NAME} LANGUAGES CXX)

# 3.12부터 configure defends가 추가됨.
# 빌드시마다 자동으로 체크해서 변경된 파일 추가/삭제해줌
# GLOB 대신 GLOB_RECURSE를 쓰면 하위 폴더까지 재귀적으로 스캔해줌
file(GLOB_RECURSE headers CONFIGURE_DEPENDS *.h)
file(GLOB_RECURSE blog_sources CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/blog_tests/*.cpp)
file(GLOB_RECURSE memory_order_sources CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/memory_order_tests/*.cpp)
file(GLOB_RECURSE cmakestext CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/CMakeLists.txt)
file(GLOB_RECURSE ready_threads CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/third_party/ready_thread/include/*.h)



# ===========================================================================
# 비주얼스튜디오 설정
# ===========================================================================

if (${MSVC})
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

     # CMake 3.6부터 시작 프로젝트 지정이가능함
     set_property(DIRECTORY ${CMAKE_BINARY_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})


     # CMake 3.18부터 빌드 제외 설정 기능이 추가됨.
     # CMakeLists.txt 파일은 추가 안해줘도 자동으로 추가해주던데 .. 제외 설정 해놓자.
     # 안해놓으면 빌드할때마다 자꾸 프로젝트/솔루션 리로딩됨
     # 안되네... 
     set_property(SOURCE ${cmakestext} PROPERTY VS_SETTINGS "ExcludedFromBuild=true")

     # 그래서 우회해서 헤더파일로 아이템 타입 변경하도록 함
     # 이것도 안되네...
     set_source_files_properties(${cmakestext} PROPERTIES HEADER_FILE_ONLY TRUE)

     # 필터 생성
     # https://stackoverflow.com/questions/33808087/cmake-how-to-create-visual-studio-filters
     source_group("ready_threads" FILES ${ready_threads})
     source_group("blog_tests" FILES ${blog_sources})
     source_group("memory_order_tests" FILES ${memory_order_sources})

     
endif()


set(sources ${memory_order_sources};${blog_sources})

# 빌드 제외시킬 소스 (src 폴더 이하 경로로 -> 파일명에 문자열 포함된 것들 모두 없애줌)
list_remove_contain(sources blog_my_test1)
list_remove_contain(sources blog_my_test2)
list_remove_contain(sources blog_my_test3)
# list_remove_contain(sources memory_order)

message("=============================================")
message(프로젝트 명 : ${CMAKE_PROJECT_NAME})
message(소스디렉토리 : ${CMAKE_SOURCE_DIR})
message(빌드디렉토리 : ${CMAKE_BINARY_DIR} \n)

message([헤더 파일목록])
foreach(file ${headers})
     get_filename_component(name_only ${file} NAME)
     message(${name_only})
endforeach()

message(\n[소스 파일목록])
foreach(file ${sources})
     get_filename_component(name_only ${file} NAME)
     message(${name_only})
endforeach()

message(\n[외부 헤더 파일목록])
foreach(file ${ready_threads})
     get_filename_component(name_only ${file} NAME)
     message(${name_only})
endforeach()
message(\n=============================================)

add_executable(${CMAKE_PROJECT_NAME} 
  ${sources} 
  ${headers} 
  ${cmakestext} 
  ${ready_threads}
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/third_party/ready_thread/include 
    ${CMAKE_SOURCE_DIR}/third_party/googletest/googletest/include
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE 
     gtest          # 유닛 테스트 수행을 위해 링크해줌
     gtest_main     # 메인함수 없이 실행 가능하도록 링크해줌
)

# target link_libraries는 add_executable 뒤에 둬야함.
# https://stackoverflow.com/questions/25909943/getting-a-cmake-error-cannot-specify-link-libraries-for-target-which-is-not-bui
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
     # arm 리눅스에서는 std::thread 사용을 위해 pthread 링크가 필요함
     target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE pthread)
     set(CMAKE_CXX_FLAGS "-Wall -Wextra")
     set(CMAKE_CXX_FLAGS_DEBUG "-g")
     set(CMAKE_CXX_FLAGS_RELEASE "-O3")      # 기본적으로 Release 타입이 적용됨
endif()
