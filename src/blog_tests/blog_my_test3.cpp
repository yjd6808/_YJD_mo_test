/*
 * 작성자: 윤정도
 * 내가 상상해서 작성
 *
 * cmake -S . -B build -G "Visual Studio 17 2022" -A Win32
 * 로 빌드해야 확인가능
 *
 */

#include "all_in_one.h"

#if BLOG_TESTS == ON && BLOG_MY_TEST3 == ON

static volatile long long g_read = 0;
static int g_tearing_detected = 0;

TEST(blog_my_test3, blog_tests) {
  ready_thread_collection pool(2);
  pool[0]->set_action([]        // 쓰레드 1
  {
    rd(0);
    g_read = 0;
  });

  pool[1]->set_action([]       // 쓰레드 2
  {
    rd(1);
    g_read = 0x0000111100002222LL;
  });

  for (int i = 0; i < 100'000; i++)
  {
    g_read = 0;
    pool.run();

    if (g_read == 0x0000111100000000LL)
      g_tearing_detected++;
  }

  EXPECT_EQ(g_tearing_detected, 0);
}

#endif


