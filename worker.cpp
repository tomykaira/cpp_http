#include <cstdlib>
#include <iostream>
#include <hiredis/hiredis.h>
#include "worker.hpp"

static redisContext *ctx = NULL;

static void check_redis_error(redisContext *ctx)
{
  if (ctx != NULL && ctx->err) {
    std::cerr << "Redis error: " << ctx->errstr << std::endl;
    exit(1);
  }
}

Worker::Worker()
{
  ctx = redisConnect("127.0.0.1", 6379);
  check_redis_error(ctx);
}

Worker::~Worker()
{
  if (ctx != NULL)
    redisFree(ctx);
}

long long Worker::work()
{
  redisReply *reply;

  reply = (redisReply *)redisCommand(ctx, "RPUSH mylist {\"result\":\"success\",\"id\":5}");
  if (reply->type == REDIS_REPLY_ERROR) {
    std::cerr << "Redis replied error: " << reply->str << std::endl;
    freeReplyObject(reply);
    return 0LL;
  }
  freeReplyObject(reply);

  reply = (redisReply *)redisCommand(ctx, "LLEN mylist");
  if (reply->type == REDIS_REPLY_ERROR) {
    std::cerr << "Redis replied error: " << reply->str << std::endl;
    freeReplyObject(reply);
    return 0LL;
  }
  long long size = reply->integer;
  freeReplyObject(reply);
  return size;
}
