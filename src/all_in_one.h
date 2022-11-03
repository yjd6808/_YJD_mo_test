/*
 * 작성자 : 윤정도
 * 1. 테스트를 어떻게 수행할지에 대한 설정 파일
 * 2. 각종 편의기능 추가
 */

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
// inline int g_iteration = 10'000;
 inline int g_iteration = 100'000;
// inline int g_iteration = 1'000'000;
// inline int g_iteration = 10'000'000;


#define ON    1
#define OFF   0

// 테스트 실행 여부 결정
// 블로그 글쓰면서 작성한 코드
#define BLOG_TESTS          ON        
// 메모리 오더 관련 코드
#define MEMORY_ORDER_TESTS  ON       


#if BLOG_TESTS == ON
  #define BLOG_MY_TEST1 ON
  #define BLOG_MY_TEST2 ON
  #define BLOG_MY_TEST3 ON
  #define BLOG_MY_TEST4 ON
  #define BLOG_MY_TEST5 ON
  #define BLOG_MY_TEST6 ON
  #define BLOG_MY_TEST7 ON
#endif

#if MEMORY_ORDER_TESTS == ON

  // 제프 프레싱 코드
  #define JEFF_PRESHING_TEST    ON

  // 스택 오버플로우 코드
  #define PETERSON_TEST         ON

  #define RELAXED_TEST1         ON
  #define RELAXED_TEST2         ON
  #define RELAXED_TEST3         ON
  #define RELAXED_TEST4         ON

  
#endif


// 윈도우 테스트 용
#if defined(_WIN32) && defined(_MSC_VER)
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>

  #define COMPILER_BARRIER  _ReadWriteBarrier()
  #define CPU_BARRIER       MemoryBarrier()
#endif

// MemoryBarrier() 함수랑 동일
/*
#define BARRIER do {                                \
                  int a = 0;                        \
                  __asm { lock or dword ptr[a], 0 } \
                } while (false)
*/
// #define BARRIER __asm { mfence }

#if defined(__GNUC__) && (defined(__aarch64__) || defined(__ARM_ARCH))
  #define COMPILER_BARRIER  asm volatile("" ::: "memory")
  #define CPU_BARRIER       asm volatile("dmb ish" ::: "memory")
#endif


// 넘 길어서 축약
#define mo_seq_cst   memory_order_seq_cst
#define mo_acquire   memory_order_acquire
#define mo_release   memory_order_release
#define mo_acq_rel   memory_order_acq_rel
#define mo_consume   memory_order_consume
#define mo_relaxed   memory_order_relaxed


template <class T>
inline auto doNotOptimize(T const& datum) {
  return reinterpret_cast<char const volatile&>(datum);
}

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


// https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
inline int random_int()
{
  static std::random_device rd;  //Will be used to obtain a seed for the random number engine
  static std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  static std::uniform_int_distribution distrib(0, 0x7fffffff);
  return distrib(gen);
}

// 쓸모없는 짓을 조금해줌 (CPU 텀을 두기위함)
inline void rd(int t)
{
  static volatile int g_dummy1 = 0;
  static volatile int g_dummy2 = 0;
  static volatile int g_dummy3 = 0;
  static volatile int g_dummy4 = 0;
  static volatile int* dummy = nullptr;
  dummy = t == 0 ? &g_dummy1 : &g_dummy2;
  dummy = t == 2 ? &g_dummy3 : &g_dummy4;
  while (random_int() % 4 != 0) {}
  dummy = t == 3 ? &g_dummy4 : &g_dummy3;
  dummy = t == 1 ? &g_dummy2 : &g_dummy1;
  *dummy = random_int();
}

// 스코프 벗어날 때 호출해주기 위함
template <typename t_call>
struct destructor final
{
  destructor(t_call&& call): m_call(forward<t_call>(call)) {}
  ~destructor() { m_call(); }

  t_call m_call;
};


