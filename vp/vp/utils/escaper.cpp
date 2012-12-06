#include "escaper.h"

vp_namespace_BEGIN

utils_namespace_BEGIN escaper::escaper(char quote, char backslash, char separator, char replacement):
quote_(quote), backslash_(backslash), separator_(separator), replacement_(replacement) {
}

std::string & escaper::escape(std::string & what) const {
    for(size_type i = 0; i < what.size(); ++i) {
        if(what[i] == quote_ || what[i] == backslash_ || what[i] == separator_ || what[i] == '\n') {
            //change separator for replacement
            if(what[i] == separator_) {
                what[i] = replacement_;
            } else if(what[i] == '\n') {
                what[i] = 'n';
            }
            what.insert(i, 1, backslash_);
            //we need to move one position behind the encountered character
            ++i;
        }
    }
    //enclose into quotes
    what.insert(0, 1, quote_);
    what += quote_;
    return what;
}

std::string escaper::escape(const std::string & what) const
{
    std::string ret(what);
    return escape(ret);
}

void escaper::strip(std::string & what) const {
    if(what.size() < 2)
    return;

    //delete quotes
    what.erase(what.size() - 1, 1);
    what.erase(0, 1);

    for(size_type i = 0; i < what.size(); ++i) {
        if(what[i] == backslash_) {
            // after erasing backslash we will be behind escaping character
            what.erase(i, 1);
            //check size to avoid touching out of bounds error in case of invalid format
            if(i < what.size() && what[i] == replacement_) {
                what[i] = separator_;
            } else if(i < what.size() && what[i] == 'n') {
                what[i] = '\n';
            }
        }
    }
}

utils_namespace_END

vp_namespace_END
