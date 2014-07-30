#ifndef _WORKER_H_
#define _WORKER_H_

#include <hiredis/hiredis.h>

class Worker {
private:
  redisContext *ctx;

public:
  Worker();
  ~Worker();
  long long work();
};

#endif /* _WORKER_H_ */
