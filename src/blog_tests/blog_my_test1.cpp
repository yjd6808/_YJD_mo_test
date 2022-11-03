/*
 * 작성자: 윤정도
 * 내가 상상해서 작성
 */

#include "all_in_one.h"

#if BLOG_TESTS == ON && BLOG_MY_TEST1 == ON

static int X = 0;

TEST(blog_my_test1, blog_tests) {
  ready_thread_collection pool(2);    // 2개의 쓰레드를 생성

  pool[0]->set_action([] { ++X; });   // thread 1
  pool[1]->set_action([] { ++X; });   // thread 2

  for (int k = 0; k < 100'000; k++)
    pool.run();

  EXPECT_EQ(X, 200'000);
  pool.reset_statistics();

  /*

  Thread A                 Thread B
  ------------------------ ------------------------
  read  eax, X 
                            read  eax, X
                            add   eax, 1
                            write X,   eax 
  add   eax, 1
  write X,   eax
  
  */
}

#endif


