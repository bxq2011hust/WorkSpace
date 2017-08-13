#include <hiredis/hiredis.h>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char **argv)
{
    struct timeval timeout = {1, 0};
    redisContext *pRedisContext = (redisContext *)redisConnectWithTimeout("127.0.0.1", 6379, timeout);
    if ((pRedisContext == NULL) || (pRedisContext->err))
    {
        if (pRedisContext)
        {
            std::cout << "connect error:" << pRedisContext->errstr << std::endl;
        }
        else
        {
            std::cout << "can't alloc redis context." << std::endl;
        }
        return -1;
    }

    redisReply *replay;
    // auth password
    replay = (redisReply *)redisCommand(pRedisContext, "auth redispassword");
    freeReplyObject(replay);

    /* PING server */
    replay = (redisReply *)redisCommand(pRedisContext, "PING");
    printf("PING: %s\n", replay->str);
    freeReplyObject(replay);

    /* Set a key */
    replay = (redisReply *)redisCommand(pRedisContext, "SET %s %s", "foo", "hello world");
    printf("SET: %s\n", replay->str);
    freeReplyObject(replay);

    /* Set a key using binary safe API */
    replay = (redisReply *)redisCommand(pRedisContext, "SET %b %b", "bar", (size_t)3, "hello", (size_t)5);
    printf("SET (binary API): %s\n", replay->str);
    freeReplyObject(replay);

    /* Try a GET and two INCR */
    replay = (redisReply *)redisCommand(pRedisContext, "GET foo");
    printf("GET foo: %s\n", replay->str);
    freeReplyObject(replay);

    replay = (redisReply *)redisCommand(pRedisContext, "INCR counter");
    printf("INCR counter: %lld\n", replay->integer);
    freeReplyObject(replay);
    /* again ... */
    replay = (redisReply *)redisCommand(pRedisContext, "INCR counter");
    printf("INCR counter: %lld\n", replay->integer);
    freeReplyObject(replay);

    /* Create a list of numbers, from 0 to 9 */
    replay = (redisReply *)redisCommand(pRedisContext, "DEL list-mylist");
    freeReplyObject(replay);
    for (int j = 0; j < 10; j++)
    {
        char buf[64];

        snprintf(buf, 64, "%u", j);
        replay = (redisReply *)redisCommand(pRedisContext, "LPUSH list-mylist element-%s", buf);
        freeReplyObject(replay);
    }

    /* Let's check what we have inside the list */
    replay = (redisReply *)redisCommand(pRedisContext, "LRANGE list-mylist 0 -1");
    if (replay->type == REDIS_REPLY_ARRAY)
    {
        for (unsigned int j = 0; j < replay->elements; j++)
        {
            printf("%u) %s\n", j, replay->element[j]->str);
        }
    }
    freeReplyObject(replay);

    //test hiredis binary safe
    cout << "test binary safe api" << endl
         << "hset \"tt key\" testkey \"tt tt\"" << endl;
    replay = (redisReply *)redisCommand(pRedisContext, "hset %b testkey %b", "tt key", 5, "tt tt", 5);
    if (replay)
    {
        if (replay->type == REDIS_REPLY_ERROR)
        {
            cout << replay->str << endl;
        }
    }
    freeReplyObject(replay);
    cout << "hget \"tt key\" testkey :";
    replay = (redisReply *)redisCommand(pRedisContext, "hget %b testkey", "tt key", 5);
    if (replay)
    {
        if (replay->type == REDIS_REPLY_STRING)
        {
            cout << replay->str << endl;
        }
    }
    freeReplyObject(replay);

    /* Disconnects and frees the context */
    if (pRedisContext)
        redisFree(pRedisContext);

    return 0;
}
