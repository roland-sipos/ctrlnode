#include <iostream>
#include <sstream>
#include <future>

#include "CCMValid.hpp"
#include "CtrlNode.hpp"

using namespace dune::daq::ccm;

template<typename T>
inline bool contains(const std::map<std::string, T>& dict,  const std::string& id)
{
  return (dict.find(id) == dict.end()) ? false : true; 
}

inline void throw_error(const std::string& s)
{
    throw std::runtime_error(s.c_str());
}

void CtrlNode::addClient(const std::string& id, const std::string& uri, uint16_t port)
{
  if ( !contains(clients_, id) ) {
    clients_[id] = std::make_unique<HttpClient>( uri, port );
    clients_[id]->init(1);
    clients_[id]->start();
  } else {
    throw_error("Client already exist: " + id);
  }
}

void CtrlNode::addControlledObj(const std::string& id, const std::string& uri, uint16_t port, 
                                std::future<std::string>& fut, std::function<void(const std::string&)> func)
{
  if ( !contains(controlled_objs_, id) ) {
    controlled_objs_[id] = std::make_unique<RestEndpoint>( uri, port, fut, func );
    controlled_objs_[id]->init(4);
    controlled_objs_[id]->start();
  } else {
    throw_error("Controlled object already exist: " + id);
  }

}

void CtrlNode::removeClient(const std::string& id)
{
  if ( contains(clients_, id) ) { 
    clients_[id]->shutdown();
    clients_.erase(id);
  } else {
    throw_error("Client doesn't exist: " + id);
  }
}

void CtrlNode::removeControlledObj(const std::string& id)
{
  if ( contains(controlled_objs_, id) ) { 
    std::cout << "Removing controlled object " << id << '\n';
    controlled_objs_[id]->shutdown();
    controlled_objs_.erase(id);
  } else {
    throw_error("Client doesn't exist: " + id);
  }
}

std::string CtrlNode::info() const 
{
  std::ostringstream oss;
  oss << "CtrlNode -->";
  return oss.str();
}

void CtrlNode::sendCommand(const std::string& id, const std::string& content)
{
  if ( contains(clients_, id) ) {
    clients_[id]->handleCommand(content);
  } else {
    throw_error("Client doesn't exist: " + id);
  }
}

std::pair<size_t, size_t> CtrlNode::getRequestCount(const std::string& id)
{
  if ( contains(clients_, id) ) {
    return clients_[id]->getRequestCount();
  } else {
    throw_error("Client doesn't exist: " + id);
  }
}

void CtrlNode::teardown()
{
  std::cout << "Teardown clients and servers...\n";
  for( auto const& [id, client] : clients_ )
  {
    client->shutdown();
  }
  for( auto const& [id, controlledObj] : controlled_objs_ )
  {
    controlledObj->shutdown();
  }
  clients_.clear();
  controlled_objs_.clear();
}
