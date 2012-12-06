#include "../exception.h"

namespace vp {

exception::exception(const char * what, int code) :
        what_(what), code_(code) {
}

exception::exception() :
        what_(defaultMessage_), code_(DEFAULT) {
}

const char * exception::what() const {
    return what_;
}

int exception::code() const {
    return code_;
}

exception::~exception() {
}

const char * const exception::defaultMessage_ = "unknown exception";

invalidFormatException::invalidFormatException() :
        exception(msg_, exception::FORMAT) {
}

const char * const invalidFormatException::msg_ = "Invalid format";

invalidNumberParamsException::invalidNumberParamsException() :
        exception(msg_, exception::INVALID_NUMBER) {
}

const char * const invalidNumberParamsException::msg_ =
        "Invalid number of parameters";

noCommandException::noCommandException() :
        exception(msg_, exception::NO_COMMAND) {
}

const char * const noCommandException::msg_ = "No Command";

unknownCommandException::unknownCommandException() :
        exception(msg_, exception::UNKNOWN) {
}

const char * const unknownCommandException::msg_ = "Unidentified command";

invalidFormatException invalidFormat;
invalidNumberParamsException invalidNumberParams;
noCommandException noCommand;
unknownCommandException unknownCommand;

}
;

