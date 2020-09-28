#include <iostream>
#include <chrono>
#include <future>

#include "CCMValid.hpp"
#include "CtrlNode.hpp"

using namespace std::chrono_literals;
using namespace dune::daq;

void cmdCallback(const std::string& content)
{
  std::cout << "OMG THIS IS HANDLED BY THE CALLBACK! >>>> " << content << '\n';
}

void cmdCallback2(const std::string& content)
{
  //std::cout << "OMG THIS IS HANDLED BY THE CALLBACK! >>>> " << content << '\n';
  std::string c = content;
  //std::cout << "Content with size: " << content.length() << " received\n";
}

int main(int argc, char** argv)
{

  dune::daq::ccm::CtrlNode node;

  std::future<std::string> fut1;
  std::future<std::string> fut2; 
  std::future<std::string> fut3;

  try {
    const uint16_t port1 = ccm::Valid::portNumber(12345);
    const uint16_t port2 = ccm::Valid::portNumber(12346);

    node.addControlledObj("daqobj1", "localhost", port1, fut1, &cmdCallback); 
    node.addControlledObj("daqobj2", "localhost", port2, fut2, &cmdCallback);
    node.addClient("client1", "localhost", port1); 
    node.addClient("client2", "localhost", port2);
  } catch (std::exception e) {
    std::cout << "Could not add controlled/controller entity: " << e.what() << '\n';
  } 

  std::cout << "Going to sleep 2s...\n";
  std::this_thread::sleep_for(2s);   

  try {
    node.removeClient("client1");
    node.removeControlledObj("daqobj1"); 
    node.removeControlledObj("daqobj5"); // not in map
  } catch (std::exception e) {
    std::cout << "Couldn't remove controlled/controlled entitiy: " << e.what() << '\n';
  }

  node.addControlledObj("daqobj111", "localhost", 12347, fut3, &cmdCallback2);

  std::this_thread::sleep_for(1s);

  std::string content = " CONTENT CONTENT CONTENT CONTENT\n";
  try {
    node.sendCommand("client1", ".........  CONTENT  .........\n");
  } catch (std::exception e) {
    std::cout << "Couldn't send command to  controller client: " << e.what() << '\n';
  }

  std::cout << "Before good send...\n";
  try { 
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    node.sendCommand("client2", content);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Execute time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
  } catch (std::exception e) {
    std::cout << "Couldn't send command to controller client: " << e.what() << '\n';
  }

  std::cout << "Going to sleep 10s...\n";

  std::this_thread::sleep_for(10000s);

  std::cout << "Before cleanup let's see if there were incoming commands: \n";
  std::cout << "\nfut3->" << fut3.get() << '\n';

  node.teardown();

}
