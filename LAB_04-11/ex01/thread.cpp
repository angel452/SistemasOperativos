// example for thread::join
#include <iostream>       // std::cout
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
 
void pause_thread(int n) 
{
  std::this_thread::sleep_for (std::chrono::seconds(n));
  std::cout << "pause of " << n << " seconds ended\n";
}
 
int main() 
{
  std::cout << "Spawning 3 threads...\n";
  std::thread t1 (pause_thread,100);
  std::thread t2 (pause_thread,200);
  std::thread t3 (pause_thread,300);
  std::cout << "Done spawning threads. Now waiting for them to join:\n";
  t1.join();
  t2.join();
  t3.join();
  std::cout << "All threads joined!\n";

  return 0;
}
