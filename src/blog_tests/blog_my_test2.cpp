/*
 * 작성자: 윤정도
 * 내가 상상해서 작성
 */

#include "all_in_one.h"

#if BLOG_TESTS == ON && BLOG_MY_TEST2 == ON

static volatile bool g_running;

TEST(blog_my_test2, blog_tests) {
  ready_thread_collection pool(2);
  pool[0]->set_action([]        // 쓰레드 1
  {
    g_running = false;
  });

  pool[1]->set_action([]       // 쓰레드 2
  {
    if (g_running)
    {
      // 작업...
    }
  });

  for (int i = 0; i < 100'000; i++)
  {
    g_running = true;
    pool.run();
  }
}

#endif


