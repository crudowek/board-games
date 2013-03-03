#include "parameter.h"

bg_namespace_BEGIN

const std::string & parameter::name() const {
    return name_;
}

const std::string & parameter::description() const {
    return description_;
}

const std::string & parameter::values() const {
    return values_;
}

const std::string & parameter::defaultValue() const {
    return defaultValue_;
}

std::string parameter::listInfo() const {
    return name_ + ',' + defaultValue_;
}

bg_namespace_END
