#include <chrono>
#include <future>

#include "RestEndpoint.hpp"
#include "CCMGeneric.hpp"

using namespace dune::daq::ccm;
using namespace Pistache;

void RestEndpoint::init(size_t threads)
{
  auto opts = Http::Endpoint::options()
    .threads(static_cast<int>(threads))
    .maxRequestSize(15728640) // 15MB
    .maxResponseSize(1048576) // 1MB 
    .flags(Pistache::Tcp::Options::ReuseAddr)
    .flags(Pistache::Tcp::Options::ReusePort);
    
  http_endpoint_->init(opts);
  //createDescription();
  createRouting();
}

void RestEndpoint::start() 
{
  // set task
  server_thread_ = std::thread(&RestEndpoint::serveTask, this);
}

void RestEndpoint::serveTask() 
{ 
  // wait for external signal to start
  //ready_future_.wait();
    
  http_endpoint_->setHandler(router_.handler());
  http_endpoint_->serve();
}

void RestEndpoint::shutdown()
{
  http_endpoint_->shutdown();
  server_thread_.join();
}

void RestEndpoint::createRouting()
{
  using namespace Rest;
  Routes::Post(router_, "/command", Routes::bind(&RestEndpoint::handleCommand, this));
  Routes::Get(router_, "/ping", Routes::bind(&dune::daq::ccm::generic::handlePing));
  //Routes::Get(router, "/auth", Routes::bind(&StatsEndpoint::doAuth, this));
}

inline void extendHeader(Http::Header::Collection& headers) 
{
  headers.add<Http::Header::AccessControlAllowOrigin>("*");
  headers.add<Http::Header::AccessControlAllowMethods>("POST,GET");
  headers.add<Http::Header::ContentType>(MIME(Text, Plain));
}

//void RestEndpoint::registerCallback(std::function<void(const std::string&)> functor)
//{
  
//}

void RestEndpoint::handleCommand(const Rest::Request& request, Http::ResponseWriter response)
{
  //auto cmdName = request.param(":cmdName").as<std::string>();
  //std::cout << " cmdName: " << cmdName << '\n';
  //if (request.hasParam(":file")) {
  //  auto fileParam = request.param(":file").as<std::string>();
  //  std::cout << " Filename as str: " << fileParam << '\n';
  //}

  //auto content = request.body();
  //std::cout << "Content: " << content << '\n';

  //command_future_ = std::async([](std::string recvdData) {
  //    return recvdData;
  //},content);

  //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  command_callback_( std::move(request.body()) );  
  //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  //std::cout << "Served difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

  //extendHeader(response.headers());
  //response.timeoutAfter(std::chrono::seconds(1));
  //begin = std::chrono::steady_clock::now();
  auto res = response.send(Http::Code::Ok, "Command received\n");
  //end = std::chrono::steady_clock::now();
  //std::cout << "Response difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

  //res.then(
  //  [](ssize_t bytes) { std::cout << bytes << " bytes have been sent" << std::endl; },
  //  Async::NoExcept
  //); 
}
