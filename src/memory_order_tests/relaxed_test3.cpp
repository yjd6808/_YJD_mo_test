/*
 * 작성자: 윤정도
 * 예제: https://stackoverflow.com/questions/48139399/memory-order-relaxed-not-work-as-expected-in-code-from-c-concurrency-in-action
 */

#include "all_in_one.h"

#if MEMORY_ORDER_TESTS == ON && RELAXED_TEST3 == ON

static atomic<bool> X, Y;
static atomic<int> Z;
static int reorder_detected;

TEST(relaxed_test3, memory_order) {
  ready_thread_collection& pool = g_threads_2;
  
  pool[0]->set_action([]
  {
    rd(0);

    X.store(true, mo_relaxed);  // 1
    Y.store(true, mo_relaxed);  // 2
  });

  pool[1]->set_action([]
  {
    rd(1);

    while (!Y.load(std::mo_relaxed)) {}  // 3
    if (X.load(std::mo_relaxed))         // 4
      ++Z;
  });


  for (int k = 0; k < g_iteration; k++)
  {
    X = false;
    Y = false;
    Z = 0;

    pool.run();

    if (Z == 0)
      reorder_detected++;
  }

  EXPECT_EQ(reorder_detected, 0);
  pool.reset_statistics();
}


#endif