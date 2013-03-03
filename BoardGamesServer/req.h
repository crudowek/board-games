#ifndef BG_REQ
#define BG_REQ

#include <string>
#include <vector>

#define bg_namespace_BEGIN namespace bg{
#define bg_namespace_END };

bg_namespace_BEGIN

struct req {
    std::string url;
    std::string file;
};

bg_namespace_END

#endif
