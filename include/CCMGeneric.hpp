#pragma once

#include <pistache/http.h>

namespace dune {
namespace daq {
namespace ccm { 
namespace generic {

void handlePing(const Pistache::Rest::Request&, Pistache::Http::ResponseWriter response) 
{
  response.send(Pistache::Http::Code::Ok, "pong");
}

}
}
}
}
