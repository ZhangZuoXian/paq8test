#include "util/redisHandler.hpp"

RedisHandler::RedisHandler()
{
    m_addr = "127.0.0.1";
    m_port = 6379;
    m_pwd = "";
    pm_rct = NULL;
    pm_rr = NULL;
    error_msg = "";
    connect(m_addr,m_port);
}

RedisHandler::~RedisHandler()
{
    disConnect();
    pm_rct = NULL;
    pm_rr = NULL;
}

/*
连接redis数据库
addr: 地址，port：端口号，pwd：密码
成功返回M_REDIS_OK，失败返回M_CONNECT_FAIL
*/
int RedisHandler::connect(const std::string &addr = "127.0.0.1", int port = 6379, const std::string &pwd) {
    m_addr = addr;
    m_port = port;
    m_pwd = pwd;

    pm_rct = redisConnect(m_addr.c_str(), m_port);

    if (pm_rct->err)
    {
        error_msg = pm_rct->errstr;
        return M_CONNECT_FAIL;
    }

    if (!m_pwd.empty())
    {
        return connectAuth(m_pwd);
    }

    return M_REDIS_OK;
}

/*
断开redis连接
*/
int RedisHandler::disConnect()
{
    redisFree(pm_rct);
    freeReplyObject(pm_rr);
}

/*
添加或插入键值对
key：键，value：值
成功返回M_REDIS_OK，失败返回<0
*/
int RedisHandler::setValue(const std::string &key, const std::string &value = "0")
{
    // std::cout<<"key: "<<key<<" "<<"value: "<<value<<std::endl;
    std::string cmd = "set " + key + " " + value;

    pm_rr = (redisReply*)redisCommand(pm_rct, cmd.c_str());
    // std::cout<<pm_rr->str<<std::endl;
    return handleReply();
}

/*
获取键对应的值
key：键，value：值引用
成功返回M_REDIS_OK，失败返回<0
*/
int RedisHandler::getValue(const std::string &key, std::string &value)
{
    std::string cmd = "get " + key;

    // std::cout<<"key: "<<key<<" "<<"value: "<<value<<std::endl;
    pm_rr = (redisReply*)redisCommand(pm_rct, cmd.c_str());
    
    // std::cout<<pm_rr->str<<std::endl;
    int ret = handleReply(&value);
    // std::cout<<"key: "<<key<<" "<<"value: "<<value<<std::endl;
}

/*
删除键
key：键
成功返回影响的行数（可能为0），失败返回<0
*/
int RedisHandler::delKey(const std::string &key)
{
    std::string cmd = "del " + key;

    pm_rr = (redisReply*)redisCommand(pm_rct, cmd.c_str());

    int rows = 0;
    int ret = handleReply(&rows);
    if (ret == M_REDIS_OK)
        return rows;
    else
        return ret;
}

/*
打印所有键值对到屏幕上
*/
int RedisHandler::printAll()
{
    std::string cmd = "keys *";

    pm_rr = (redisReply*)redisCommand(pm_rct, cmd.c_str());

    int len ;
    redisReply **array;
    int ret = handleReply(&len, &array);
    if (ret == M_REDIS_OK)
    {
        for (int i = 0; i < len; i++)
            std::cout << std::string(array[i]->str) << std::endl;
    }
    else
        return 0;
}

/*
返回错误信息
*/
std::string RedisHandler::getErrorMsg()
{
    return error_msg;
}

/*
使用密码登录
psw：登录密码
成功返回M_REDIS_OK，失败返回<0
*/
int RedisHandler::connectAuth(const std::string &psw)
{
    std::string cmd = "auth " + psw;

    pm_rr = (redisReply*)redisCommand(pm_rct, cmd.c_str());

    return handleReply();
}

/*
处理redis返回的信息
value：数据指针，用于保存redis返回的基本类型（value指针指向该数据）
array：数组指针，用于保存redis返回的数组
成功返回M_REDIS_OK，失败返回<0
*/
int RedisHandler::handleReply(void* value, redisReply*** array)
{
    if (pm_rct->err)
    {
        error_msg = pm_rct->errstr;
        // printf("Error in null : %s\n", pm_rct->errstr);
        return M_CONTEXT_ERROR;
    }

    if (pm_rr == NULL)
    {
        error_msg = "auth redisReply is NULL";
        // printf("Error: %s\n", pm_rct->errstr);
        return M_REPLY_ERROR;
    }

    switch (pm_rr->type)
    {
    case REDIS_REPLY_ERROR:
        error_msg = pm_rr->str;
        return M_EXE_COMMAND_ERROR;
    case REDIS_REPLY_STATUS:
        if (!strcmp(pm_rr->str, "OK"))
            return M_REDIS_OK;
        else
        {
            error_msg = pm_rr->str;
            return M_EXE_COMMAND_ERROR;
        }
    case REDIS_REPLY_INTEGER:
        *(int*)value = pm_rr->integer;
        return M_REDIS_OK;
    case REDIS_REPLY_STRING:
        *(std::string*)value = pm_rr->str;
        return M_REDIS_OK;
    case REDIS_REPLY_NIL:
        *(std::string*)value = "";
        return M_REDIS_OK;
    case REDIS_REPLY_ARRAY:
        *(int*)value = pm_rr->elements;
        *array = pm_rr->element;
        return M_REDIS_OK;
    default:
        error_msg = "unknow reply type";
        return M_EXE_COMMAND_ERROR;
    }
}

void RedisHandler::flushDB(){
    std::string cmd = "flushdb";
    pm_rr = (redisReply*)redisCommand(pm_rct, cmd.c_str());
}