/*
 * 작성자: 윤정도
 * 예제: https://stackoverflow.com/questions/12340773/how-do-memory-order-seq-cst-and-memory-order-acq-rel-differ
 * 둘 모두 무조건 성공함.. 흠..
 */

#include "all_in_one.h"

#if MEMORY_ORDER_TESTS == ON && RELAXED_TEST1 == ON

#define mo_order mo_relaxed     // mo_seq_cst

static atomic<bool> Y(false), X(false);
static atomic Z(0);
static atomic reorder_detected(0);

void thread1()
{
  
}

TEST(relaxed_test1, memory_order) {
  ready_thread_collection& pool = g_threads_4;

  pool[0]->set_action([] { rd(0); X.store(true, mo_order); });
  pool[1]->set_action([] { rd(1); Y.store(true, mo_order); });
  pool[2]->set_action([] { rd(2); while (!X.load(mo_order)) {} if (Y.load(mo_order)) ++Z; });
  pool[3]->set_action([] { rd(3); while (!Y.load(mo_order)) {} if (X.load(mo_order)) ++Z; });

  for (int k = 0; k < g_iteration; k++)
  {
    X = false;
    Y = false;
    Z = 0;

    pool.run();

    if (Z == 0)
      ++reorder_detected;
  }

  EXPECT_EQ(reorder_detected.load(), 0);
  pool.reset_statistics();
}


#endif
