/*
 * 작성자: 윤정도
 * 예제: https://gcc.gnu.org/wiki/Atomic/GCCMM/AtomicSync
 */

#include "all_in_one.h"

#if MEMORY_ORDER_TESTS == ON && RELAXED_TEST2 == ON

static atomic<int> Y(0), X(0);
static atomic<int> reorder_detected;

TEST(relaxed_test2, memory_order) {

  ready_thread_collection& pool = g_threads_2;

  // release acquire로 변경시:
  // X = 2 ---> Y = 1 (보장됨) Happens-Before 관계
  // relaxed로 변경시:
  // X = 2 -x-> Y = 1 (보장안됨)
  pool[0]->set_action([]
  {
    rd(0);

    Y.store(1, mo_relaxed);   // mo_release
    X.store(2, mo_relaxed);   // mo_release
  });

  pool[1]->set_action([]
  {
    rd(1);

    if (X.load(mo_relaxed) == 2 && Y.load(mo_relaxed) != 1)   // mo_acquire
      reorder_detected.fetch_add(1, memory_order_relaxed);
  });


  for (int k = 0; k < g_iteration; k++)
  {
    X = 0;
    Y = 0;

    pool.run();
  }


  EXPECT_EQ(reorder_detected.load(), 0);
  pool.reset_statistics();
}

#endif