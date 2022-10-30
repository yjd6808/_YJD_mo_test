#include "all_in_one.h"

static atomic<bool> Y(false), X(false);
static atomic<int> Z(0);
static atomic<int> ReorderDetected(0);

// https://stackoverflow.com/questions/12340773/how-do-memory-order-seq-cst-and-memory-order-acq-rel-differ
TEST(test2, memory_order) {
  // GTEST_SKIP();
  ready_thread_collection& pool = g_threads_4;

  pool[0]->set_action([] { rd(); X = true; });
  pool[1]->set_action([] { rd(); Y = true; });
  pool[2]->set_action([] { rd(); while (!X) {} if (Y) ++Z; });
  pool[3]->set_action([] { rd(); while (!Y) {} if (X) ++Z; });

  for (int k = 0; k < g_iteration; k++)
  {
    X = false;
    Y = false;
    Z = 0;

    pool.run();

    if (Z == 0)
      ++ReorderDetected;
  }

  print_line("[seq_cst] %d회 테스트 중 %d회 재배치 검출", pool.launch_count(), ReorderDetected.load(memory_order_relaxed));
  ReorderDetected = 0;
  pool.reset_statistics();

  // ===============================================================================

  pool[0]->set_action([] { rd(); X.store(true, mo_relaxed); });
  pool[1]->set_action([] { rd(); Y.store(true, mo_relaxed); });
  pool[2]->set_action([] { rd(); while (!X.load(mo_relaxed)) {} if (Y.load(mo_relaxed)) ++Z; });
  pool[3]->set_action([] { rd(); while (!Y.load(mo_relaxed)) {} if (X.load(mo_relaxed)) ++Z; });

  for (int k = 0; k < g_iteration; k++)
  {
    X = false;
    Y = false;
    Z = 0;

    pool.run();

    if (Z == 0)
      ++ReorderDetected;
  }

  print_line("[relaxed] %d회 테스트 중 %d회 재배치 검출", pool.launch_count(), ReorderDetected.load(memory_order_relaxed));
  EXPECT_EQ(ReorderDetected.load(mo_relaxed), 0);
  pool.reset_statistics();
}

