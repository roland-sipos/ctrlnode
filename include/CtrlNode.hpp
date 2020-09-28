#pragma once

#include <iostream>

#include "HttpClient.hpp"
#include "RestEndpoint.hpp"

namespace dune {
namespace daq {
namespace ccm {

class CtrlNode {
public:
  void addClient(const std::string& id, const std::string& uri, uint16_t port);

  void addControlledObj(const std::string& id, const std::string& uri, uint16_t port, 
                        std::future<std::string>& fut, 
                        std::function<void(const std::string&)>);

  void removeClient(const std::string& id); // removes controller client
  void removeControlledObj(const std::string& id); // removes controlled endpoint
  void teardown(); // remove all endpoints and clients

  std::string info() const; // returns summary of current elements

  void sendCommand(const std::string& id, const std::string& content);
  std::pair<size_t, size_t> getRequestCount(const std::string& id);

private:
  std::map<std::string, std::unique_ptr<HttpClient>> clients_; // controllers are HTTP REST Clients
  std::map<std::string, std::unique_ptr<RestEndpoint>> controlled_objs_; // Controlled objects are HTTP REST Servers

};
    
} // ccm
} // daq
} // dune
