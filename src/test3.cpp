#include "all_in_one.h"

static atomic<int> Y(0), X(0);
static atomic<int> ReorderDetected(0);

// https://gcc.gnu.org/wiki/Atomic/GCCMM/AtomicSync
// relaxed 테스트
TEST(test3, memory_order) {
   // GTEST_SKIP();
  ready_thread_collection& pool = g_threads_3;
  
  pool[0]->set_action([]
  {
    rd();
    Y.store(20, mo_relaxed);
    X.store(10, mo_relaxed);
  });

  pool[1]->set_action([]
  {
    rd();

    int x = X.load(mo_relaxed);
    int y = Y.load(mo_relaxed);

    if (x == 10)
    {
      if (y != 20)
        ReorderDetected.fetch_add(1, mo_relaxed);
      Y.store(10, mo_relaxed);
    }
  });

  pool[2]->set_action([]
  {
    rd();

    if (Y.load(mo_relaxed) == 10)
    {
      if (X.load(mo_relaxed) != 10)
        ReorderDetected.fetch_add(1, mo_relaxed);
    }
  });

  for (int k = 0; k < g_iteration; k++)
  {
    X = 0;
    Y = 0;

    pool.run();
  }

  print_line("[relaxed] %d회 테스트 중 %d회 재배치 검출", (pool[0]->launch_count() + pool[1]->launch_count() + pool[2]->launch_count()) / 3, ReorderDetected.load(memory_order_relaxed));
  EXPECT_EQ(ReorderDetected.load(mo_relaxed), 0);
  ReorderDetected = 0;
  pool.reset_statistics();
}

