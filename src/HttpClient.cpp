#include <chrono>

#include "HttpClient.hpp"

using namespace dune::daq::ccm;
using namespace Pistache;

void HttpClient::init(size_t threads)
{
  options_ = Http::Client::options().threads(static_cast<int>(threads));
  client_.init(options_);
  
}

void HttpClient::start() 
{
  // set task

}

std::pair<size_t, size_t> HttpClient::getRequestCount()
{
  return std::make_pair<size_t, size_t>( completed_requests_.load(), failed_requests_.load() ); 
}

void HttpClient::handleCommand(const::std::string& content)
{
  //std::cout << "Attempt to send command content: " << content << '\n';
  auto response = client_.post(server_uri_).body(content).send();
  response.then(
    [&](Http::Response response) {
      ++completed_requests_;
      //std::cout << "Response code = " << response.code() << std::endl;
      auto body = response.body();
      //if (!body.empty())
        //std::cout << "Response body = " << body << std::endl;
    },
    [&](std::exception_ptr exc) {
      ++failed_requests_;
      PrintException excPrinter;
      excPrinter(exc);
    }
  );
  //responses_.push_back(std::move(response));
}

void HttpClient::shutdown()
{
  client_.shutdown();
}
