#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <boost/thread.hpp>
#include <boost/thread/tss.hpp>
#include <microhttpd.h>
#include "worker.hpp"

static boost::thread_specific_ptr<Worker> p_worker;

static int ahc_echo(void * _cls,
                    struct MHD_Connection * connection,
                    const char * url,
                    const char * method,
                    const char * version,
                    const char * upload_data,
                    size_t * upload_data_size,
                    void ** ptr) {
  if (p_worker.get() == NULL) {
    std::cout << "Creating new worker: " << syscall(SYS_gettid) << std::endl;
    p_worker.reset(new Worker());
  }

  struct MHD_Response * response;
  int ret;
  long long result = p_worker->work();
  std::ostringstream body;
  body << "Current length: " << result << std::endl;
  std::string data = body.str();

  response = MHD_create_response_from_data(data.size(),
                                           (void*) data.c_str(),
                                           MHD_NO,
                                           MHD_YES);
  ret = MHD_queue_response(connection,
                           MHD_HTTP_OK,
                           response);
  MHD_destroy_response(response);
  return ret;
}

int main(int argc,
         char ** argv) {
  struct MHD_Daemon * d;
  d = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY,
                       8008,
                       NULL,
                       NULL,
                       &ahc_echo,
                       NULL,
                       MHD_OPTION_THREAD_POOL_SIZE, 12,
                       MHD_OPTION_CONNECTION_LIMIT, 12,
                       MHD_OPTION_END);
  if (d == NULL)
    return 1;
  (void) getchar();
  MHD_stop_daemon(d);
  return 0;
}
