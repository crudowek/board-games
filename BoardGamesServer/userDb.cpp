#include "userDb.h"

#include "constants.h"

#include <iostream>
#include <string>

bg_namespace_BEGIN

dbErrorException::dbErrorException():vp::exception(c::REGISTER_FIRST_MESSAGE, c::REGISTER_FIRST_CODE) {
}

dbErrorException dbError;

userdb::userdb(const std::string & fileName):stream_(fileName.c_str()) {
}

bool userdb::find(const std::string & who, value_type & found) {
    std::string tmp;
    stream_.clear();
    stream_.seekg(std::ios_base::beg);
    bool err = false;
    bool odd = true;
    while(std::getline(stream_, tmp)) {
        if(odd) {
            odd = false;
            if(tmp == who) {
                if(!std::getline(stream_, tmp)) {
                    err = true;
                    break;
                }
                found = value_type(who, tmp);
                return true;
            }
        } else {
            odd = true;
        }
    }
    if(err || !stream_) {
        throw dbError;
    }

    return false;
}

void userdb::insert(const value_type & value) {
    stream_.seekp(std::ios_base::end);
    stream_ << value.first << '\n' << value.second << std::endl;
}

bg_namespace_END
