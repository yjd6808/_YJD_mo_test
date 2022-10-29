#include "all_in_one.h"


int X, Y;
int r1, r2;

TEST(sample_01, memory_order) {
  ready_thread_collection col(2);

  col[0]->set_action([]
    {
      X = 1;
      BARRIER;
      r1 = Y;
    });
  col[1]->set_action([]
    {
      Y = 1;
      BARRIER;
      r2 = X;
    });


  constexpr int iteration = 100000;
  int reorder_detected = 0;

  for (int k = 0; k < iteration; k++)
  {
    X = 0;
    Y = 0;

    col.run();

    if (r1 == 0 && r2 == 0)
      reorder_detected++;
  }

  
  std::cout << reorder_detected << "회 재배치 검출\n";
  std::cout << iteration - reorder_detected << "회 가능한\n";
}

