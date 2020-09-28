#include <iostream>
#include <chrono>
#include <future>
#include <thread>

#include "CCMValid.hpp"
#include "CtrlNode.hpp"

using namespace std::chrono_literals;
using namespace dune::daq;

int main(int argc, char** argv)
{

  dune::daq::ccm::CtrlNode node;

  //std::string s10m(10485760, 'x');
  //std::string s1m(1048576, 'x');
  //std::string s1k(1024, 'x');
  //std::string s2k(2048, 'x');

  std::string sk(std::stoi(argv[2]), 'x'); 
  std::cout << "Size of sk: " << sk.length() << '\n';

  try {
    const uint16_t port1 = ccm::Valid::portNumber(std::stoi(argv[1]));
    node.addClient("client1", "localhost", port1);

  } catch (std::exception e) {
    std::cout << "Could not add controlled/controller entity: " << e.what() << '\n';
  } 

  std::atomic<bool> should_run{true};;
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  auto td = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin);
  std::thread timer([&](){
    std::cout << "Timer starts...\n";
    begin = std::chrono::steady_clock::now();
    while(true) {
      std::this_thread::sleep_for(50us);
      td = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin);
      if (td.count() > 1000) {
        should_run = false;
        break;
      }
    }
    std::cout << "Timer joins...\n";
  });
  timer.detach();
  
  size_t ops = 0;

  while(should_run) {
    try {
      node.sendCommand("client1", sk);
      ++ops;
    } catch (std::exception e) {
      std::cout << "Couldn't send command to  controller client: " << e.what() << '\n';
    }

  }
      //std::chrono::steady_clock::time_point step = std::chrono::steady_clock::now();
      //auto td = std::chrono::duration_cast<std::chrono::milliseconds>(step - begin).count() << "[ms]" << std::endl;
  //std::cout << "Before cleanup let's see if there were incoming commands: \n";
  //std::cout << "\nfut3->" << fut3.get() << '\n';

  //std::cout << "main thread\n";
  //timer.join();

  std::cout << "Should run: " << should_run << " runTime: " << td.count() << "[ms] Ops:" << ops << '\n';
  auto req_count = node.getRequestCount("client1");
  std::cout << " Request count from client: +" << req_count.first << " failed:" << req_count.second << '\n';

  std::this_thread::sleep_for(5s);

  node.teardown();

}
