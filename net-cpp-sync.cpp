#include <string>
#include <sstream>
#include <boost/network.hpp>
#include <boost/network/protocol/http/server.hpp>
#include "worker.hpp"

namespace http = boost::network::http;

struct handler;
// typedef http::server<handler> http_server;
typedef http::server<handler> http_server;

struct handler {
private:
  Worker worker;

public:
  void operator() (http_server::request const & request,
                   http_server::response & response) {
    long long result = worker.work();

    std::ostringstream body;
    body << "Current length: " << result << std::endl;

    response = http_server::response::stock_reply(
      http_server::response::ok, body.str());
  }

  void log(http_server::string_type const &info) {
    std::cerr << "ERROR: " << info << '\n';
  }
};

int main(int arg, char * argv[]) {
  handler handler_;
  http_server::options options(handler_);
  http_server server_(
    options.address("0.0.0.0")
    .port("8001"));
  server_.run();
}
