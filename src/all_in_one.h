#pragma once

#include <gtest/gtest.h>
#include <jdyun/ready_thread_collection.h>

#include <stack>
#include <vector>
#include <chrono>
#include <queue>
#include <deque>
#include <map>


using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;

#define USE_COMPILER_BARRIER  1
#define USE_HARDWARE_BARRIER  2
#define USE_NOTHING           3

// 무슨 베리어 타입 쓸지 여기서 정하면 됨.
#define BARRIER_TYPE USE_COMPILER_BARRIER

#ifdef _WIN32
  #if BARRIER_TYPE == USE_COMPILER_BARRIER
    #define BARRIER _ReadWriteBarrier()
  #elif BARRIER_TYPE == USE_HARDWARE_BARRIER
    #define BARRIER MemoryBarrier()
  #else 
    #define BARRIER
  #endif
#endif

#if defined(__aarch64__) || defined(__ARM_ARCH)
  #if BARRIER_TYPE == USE_COMPILER_BARRIER
    #define BARRIER asm volatile("" ::: "memory")
  #elif BARRIER_TYPE == USE_HARDWARE_BARRIER
    #define BARRIER asm volatile("dmb" ::: "memory")
  #else 
    #define BARRIER
  #endif
#endif

// 간단한 스트링 포맷
template <typename... t_args>
inline string str_fmt(const char* fmt, t_args&&... args)
{
  char str[1024];
  sprintf_s(str, 1024, fmt, forward<t_args>(args)...);
  return string(str);
}


// 간단한 출력
template <typename... t_args>
inline void print_line(const char* fmt, t_args&&... args)
{
  stringstream ss;
  ss << fmt << "\n";
  printf(ss.str().c_str(), forward<t_args>(args)...);
}

// 간단한 출력
template <typename... t_args>
inline void print(const char* fmt, t_args&&... args)
{
  printf(fmt, forward<t_args>(args)...);
}




