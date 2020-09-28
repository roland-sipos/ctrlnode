#pragma once

#include <thread>
#include <chrono>
#include <future>

#include <pistache/http.h>
#include <pistache/description.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

namespace dune {
namespace daq {
namespace ccm {

class RestEndpoint {

public: 
  explicit RestEndpoint(const std::string& uri, uint16_t port, 
                        std::future<std::string>& future, 
                        std::function<void(const std::string&)> functor) noexcept 
    : port_{ port }, address_{ Pistache::Ipv4::any(), port_ }, // options_{ },
      http_endpoint_{ std::make_shared<Pistache::Http::Endpoint>( address_ ) },
      description_{"DUNE DAQ CCM CtrlNode API", "0.1"},
      command_future_{ future },
      command_callback_{ functor }
  {
    std::cout << "RestEndpoint created to serve on: " << uri << " port: " << port << '\n';
  }

  void init(size_t threads);
  void start();
  void stop();
  void shutdown();

private:

  void createRouting();
  void createDescription();

  void serveTask(); 

  // Routes
  void handleCommand(const Pistache::Rest::Request&, Pistache::Http::ResponseWriter response);

  Pistache::Port port_;
  Pistache::Address address_;
  std::shared_ptr<Pistache::Http::Endpoint> http_endpoint_;
  Pistache::Rest::Description description_;
  Pistache::Rest::Router router_;

  std::future<std::string>& command_future_;

  std::function<void(const std::string&)> command_callback_;

  std::thread server_thread_;


};

}
}
}
