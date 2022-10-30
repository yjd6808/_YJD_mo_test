#include "all_in_one.h"


static int X, Y;
static int r1, r2;
static int ReorderDetected = 0;

TEST(sample_01, memory_order) {
  random_int(); 

  ready_thread_collection& pool = g_threads_2;
  pool[0]->set_action([]
  {
    rd();

    X = 1;
    BARRIER;
    r1 = Y;
  });

  pool[1]->set_action([]
  {
    rd();

    Y = 1;
    BARRIER;
    r2 = X;
  });


  for (int k = 0; k < g_iteration; k++)
  {
    X = 0;
    Y = 0;

    pool.run();

    if (r1 == 0 && r2 == 0)
      ReorderDetected++;
  }

  print_line("%d회 테스트 중 %d회 재배치 검출", pool.launch_count(), ReorderDetected);
  EXPECT_EQ(ReorderDetected, 0);
  pool.reset_statistics();
}

