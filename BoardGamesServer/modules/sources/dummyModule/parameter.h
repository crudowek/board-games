#ifndef BG_PARAMETER
#define BG_PARAMETER

#include <string>

#define bg_namespace_BEGIN namespace bg{
#define bg_namespace_END };

bg_namespace_BEGIN

class parameter {
private:
    std::string name_;
    std::string description_;
    std::string values_;

public:
    const std::string & name() const;
    const std::string & description() const;
    const std::string & values() const;
};

bg_namespace_END

#endif
