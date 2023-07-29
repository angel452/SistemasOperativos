#include <iostream>       // std::cout
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
 
void pause_thread(int idThread, int n) 
{
  std::this_thread::sleep_for (std::chrono::seconds(n));
  std::cout << "Id del Thread: " << idThread << "\n";
}
 
int main() 
{
  std::cout << "Spawning and detaching 100 threads...\n";
  
  for (int i = 0; i < 100; i++){
    std::thread (pause_thread, i, 1).detach(); // i -> id   1 -> 1 segundo
  }

  std::cout << "Done spawning threads.\n";

  std::cout << "(the main thread will now pause for 5 seconds)\n";
  // give the detached threads time to finish (but not guaranteed!):
  pause_thread(666,5);
  return 0;
}
