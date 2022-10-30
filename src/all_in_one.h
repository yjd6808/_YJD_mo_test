#pragma once

#include <gtest/gtest.h>
#include <jdyun/ready_thread_collection.h>

#include <stack>
#include <vector>
#include <chrono>
#include <random>
#include <limits>
#include <queue>
#include <deque>
#include <map>


using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;

// 재사용할 스레드풀들
inline ready_thread_collection g_threads_2(2);
inline ready_thread_collection g_threads_3(3);
inline ready_thread_collection g_threads_4(4);
inline ready_thread_collection g_threads_5(5);
inline ready_thread_collection g_threads_6(6);

// 테스트 수행 횟수
inline int g_iteration = 100'000;
// inline int g_iteration = 1'000'000;
// inline int g_iteration = 10'000'000;

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

#define mo_seq_cst   memory_order_seq_cst
#define mo_acquire   memory_order_acquire
#define mo_release   memory_order_release
#define mo_acq_rel   memory_order_acq_rel
#define mo_consume   memory_order_consume
#define mo_relaxed   memory_order_relaxed

// https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
inline int random_int()
{
  static std::random_device rd;  //Will be used to obtain a seed for the random number engine
  static std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  static std::uniform_int_distribution<int> distrib(0, numeric_limits<int>::max());
  return distrib(gen);
}

inline void rd()
{
  while (random_int() % 4 != 0) {}
}