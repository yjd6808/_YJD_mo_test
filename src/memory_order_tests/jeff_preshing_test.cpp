/*
 * 작성자 : 윤정도
 * 예제: https://preshing.com/20120515/memory-reordering-caught-in-the-act/
 */

#include "all_in_one.h"

#if MEMORY_ORDER_TESTS == ON && JEFF_PRESHING_TEST == ON

static int X, Y;
static int r1, r2;
static int reorder_detected = 0;

TEST(jeff_preshing_test1, memory_order) {

  ready_thread_collection& pool = g_threads_2;
  pool[0]->set_action([]      // 쓰레드 1
  {
    X = 1;
    COMPILER_BARRIER;
    r1 = Y;
  });        
  pool[1]->set_action([]      // 쓰레드 2
  {
    Y = 1;
    COMPILER_BARRIER;
    r2 = X;
  });        

  for (int k = 0; k < 100'000; k++)
  {
    X = 0;
    Y = 0;

    pool.run();

    if (r1 == 0 && r2 == 0)
      reorder_detected++;
  }

  EXPECT_EQ(reorder_detected, 0);
  pool.reset_statistics();
}

#endif