#pragma once

#include <algorithm>
#include <string>

#include <pistache/client.h>
#include <pistache/http.h>
#include <pistache/net.h>
#include <pistache/async.h>

namespace dune {
namespace daq {
namespace ccm {

class HttpClient {

public: 
  explicit HttpClient(const std::string& uri, uint16_t port) noexcept 
    : port_{ port }, address_{ uri }
  {
    server_uri_ = address_ + ':' + std::to_string(port) + "/command";
    std::cout << "HttpClient created to communicate with: " << server_uri_ << '\n'; 
  }

  void init(size_t threads); 
  void start();
  void shutdown();

  void handleCommand(const::std::string& content);
  std::pair<size_t, size_t> getRequestCount();

private:
  uint16_t port_;
  std::string address_;
  std::string server_uri_;

  Pistache::Http::Client client_;
  Pistache::Http::Client::Options options_;
  std::vector<Pistache::Async::Promise<Pistache::Http::Response>> responses_;

  std::atomic<size_t> completed_requests_{0};
  std::atomic<size_t> failed_requests_{0};

};

}
}
}
