#ifndef VP_CONVERT
#define VP_CONVERT

#include <sstream>
#include <string>

namespace vp {
template<typename T>
std::string convert(T what) {
    std::string res;
    std::stringstream o;
    o << what;
    o >> res;
    return res;
}
}
;

#endif
