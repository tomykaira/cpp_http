#include "gwan.h" // G-WAN API
#include "worker.hpp"
#include <string>

using namespace std;
class Hello{
public:
  void whatsup(xbuf_t *reply)
  {
    string h("Hello World");
    work();
    xbuf_cat(reply, (char*)h.c_str());
  }
};

int main(int argc, char *argv[]){
  initialize();

  Hello hello;
  hello.whatsup(get_reply(argv), h);
  return HTTP_200_OK; // HTTP status code
}
