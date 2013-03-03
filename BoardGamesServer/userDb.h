#ifndef BG_USERDB
#define BG_USERDB

#include <fstream>

#include <boost/utility.hpp>

#include <vp/exception.h>

#define bg_namespace_BEGIN namespace bg{
#define bg_namespace_END };

bg_namespace_BEGIN

class dbErrorException:public vp::exception {
public:
    dbErrorException();
};

class userdb:boost::noncopyable {
public:
    typedef std::pair<std::string, std::string> value_type;
private:
    std::fstream stream_;

public:
    userdb(const std::string & fileName);

    bool find(const std::string & who, value_type & found);

    void insert(const value_type & value);
};

bg_namespace_END

#endif
