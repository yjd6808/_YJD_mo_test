#include "all_in_one.h"

static int X, Y;
static atomic<int> ReorderDetected;

TEST(ww_reorder_test, memory_order) {
  // GTEST_SKIP();
  ready_thread_collection& pool = g_threads_2;

  pool[0]->set_action([] { rd(); X = 10; BARRIER; Y = 20; });
  pool[1]->set_action([] {
    rd(); 
    if (Y == 20 && X != 10)
    {
      ReorderDetected.fetch_add(1, mo_relaxed);
      print_line("%d %d\n", X, Y);
    }
  });

  for (int k = 0; k < g_iteration; k++)
  {
    X = 0;
    Y = 0;

    pool.run();
  }

  print_line("[ww_reorder] %d회 테스트 중 %d회 재배치 검출", pool.launch_count(), ReorderDetected.load(mo_relaxed));
  EXPECT_EQ(ReorderDetected.load(mo_relaxed), 0);
  pool.reset_statistics();

}

