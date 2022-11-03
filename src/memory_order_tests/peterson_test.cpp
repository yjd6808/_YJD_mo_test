/*
 * 작성자: 윤정도
 * 예제: https://stackoverflow.com/questions/41858540/whats-are-practical-example-where-acquire-release-memory-order-differs-from-seq
 */

#include "all_in_one.h"

#if MEMORY_ORDER_TESTS == ON && PETERSON_TEST == ON

static std::atomic flag1{ false };
static std::atomic flag2{ false };
static std::atomic counter{ 0 };
static std::atomic reorder_detected{ 0 };

/*
  acquire_release 대신 seq_cst를 써야하는 이유이다.
  아래 예제가 제대로 동작하도록 만들기 위해서는 둘 모두 mo_seq_cst로 바꿔줘야한다.

  https://en.wikipedia.org/wiki/Memory_ordering#cite_ref-11
  x86도 이 예제는 실패하는데 여기에 나와있듯이 store명령도 load 명령뒤로 재배치가 가능하기때문이다.

  다른 쓰레드에서도 관측가능하도록 만들어줘야함.
  코드가 좀 복잡해져야 예제들이 먹히는 느낌이네
*/

static constexpr auto store_order = mo_seq_cst; // mo_release
static constexpr auto load_order = mo_seq_cst;  // mo_acquire

void busy(int n)
{
  auto& me = (n == 1) ? flag1 : flag2;
  auto& him = (n == 1) ? flag2 : flag1;

  for (;;)
  {
    me.store(true, store_order);
    COMPILER_BARRIER;
    if (him.load(load_order) == false)
    {
      // 락 획득
      break;
    }

    // 락획득에 실패한 경우 재시
    me.store(false);
  }
  int tmp = counter.fetch_add(1);
  if (tmp != 0) ++reorder_detected;


  /*
   * 크리티컬 섹션
   */

  tmp = counter.fetch_sub(1);
  if (tmp != 1) ++reorder_detected;
  me.store(false);
}

TEST(so_test2, memory_order) {
  // GTEST_SKIP();
  ready_thread_collection& pool = g_threads_2;

  pool[0]->set_action([] { busy(0); });
  pool[1]->set_action([] { busy(1); });

  for (int k = 0; k < g_iteration; k++)
  {
    flag2 = false;
    flag1 = false;
    counter = 0;

    pool.run();
  }

  EXPECT_EQ(reorder_detected.load(), 0);
  pool.reset_statistics();

}


#endif
