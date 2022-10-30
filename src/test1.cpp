#include "all_in_one.h"

static atomic<int> Y(0), X(0);
static atomic<int> ReorderDetected;

TEST(test1, memory_order) {
  // GTEST_SKIP();

  ready_thread_collection& pool = g_threads_2;

  pool[0]->set_action([]
  {
    rd();
    Y.store(1, mo_seq_cst);
    X.store(2, mo_seq_cst);
  });

  pool[1]->set_action([]
  {
    rd();

    if (X.load(mo_seq_cst) == 2 && Y.load(mo_seq_cst) != 1)
      ReorderDetected.fetch_add(1, memory_order_relaxed);
  });


  for (int k = 0; k < g_iteration; k++)
  {
    X = 0;
    Y = 0;

    pool.run();
  }

  print_line("[seq_cst] %d회 테스트 중 %d회 재배치 검출", pool.launch_count(), ReorderDetected.load(memory_order_relaxed));
  ReorderDetected = 0;
  pool.reset_statistics();

  // ===============================================================================

  pool[0]->set_action([]
  {
    rd();
    Y.store(1, mo_relaxed);
    X.store(2, mo_relaxed);
  });

  pool[1]->set_action([]
  {
    rd();
    const int x = X.load(mo_relaxed);
    const int y = Y.load(mo_relaxed);

    if (x == 2 && y != 1)
      ReorderDetected.fetch_add(1, memory_order_relaxed);
  });


  for (int k = 0; k < g_iteration; k++)
  {
    X = 0;
    Y = 0;

    pool.run();
  }

  print_line("[relaxed] %d회 테스트 중 %d회 재배치 검출", pool.launch_count(), ReorderDetected.load(memory_order_relaxed));
  EXPECT_EQ(ReorderDetected.load(mo_relaxed), 0);
  pool.reset_statistics();
}

