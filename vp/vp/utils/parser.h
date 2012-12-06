#ifndef VP_UTILS_PARSER
#define VP_UTILS_PARSER

#include <map>
#include <string>
#include <sstream>
#include <vector>

#include "../exception.h"

#define utils_namespace_BEGIN namespace utils{
#define utils_namespace_END };

#define vp_namespace_BEGIN namespace vp{
#define vp_namespace_END };

#define VP_CALL_MEMBER(object,ptrToMember)  ((object).*(ptrToMember)) 

vp_namespace_BEGIN

utils_namespace_BEGIN/**
 * @brief class parsing lines separating commands and parameters and then calling appropriate function for the commands with parameters
 * @tparam T type of the object on which commands will be done 
 */
template<typename T>class parser {
public:
    typedef void(T::*function_type)(const std::vector<std::string> &);
    static const int NO_PARAM_CHECK = -1;
private:
    typedef std::pair<function_type, int> mapped_type;
    typedef std::map<std::string, mapped_type> map_type;
    /**
     * @brief function mapped by strings
     */
    map_type commands_;
    /**
     * @brief separator
     */
    char separator_;
    /**
     * @brief quote
     */
    char quote_;
    /**
     * @brief pointer to object on which the action are performed
     */
    T * pointer_;
    /**
     * @brief escaper used to strip characters
     */
    escaper escaper_;
    /**
     * @brief backslash char
     */
    char backslash_;
    /**
     * @brief counts number of consecutive occurences of char at the end of the string
     * @param c char
     * @param s string
     * @return @true event count, @false uneven
     */
    bool even(const std::string & s, char c) {
        size_t count = 0;
        size_t i = s.size() - 1;
        for(; i > 0; --i) {
            if(s[i] == c)
            ++count;
            else {
                //case s[1] != c
                ++i;
                break;
            }
        }
        if( i == 0 && s[0] == c)
        ++count;
        return count % 2;
    }
    /**
     * @brief obtain next string from the stream
     * @param in input stream
     * @param where string into which the content is stored
     * @return true if obtained succesfully, false if there was no next string
     * @throw exception::invalidFormat when input has invalid format
     */
    bool getNext(std::istream & in, std::string & where) {
        //there should be no other separators inside parameters
        if(!std::getline(in, where, separator_))
        {
            return false;
        }
        //check whether it is string parameter sent by client
        if(where.size() > 0 && where[0] == quote_) {
            escaper_.strip(where);
        }
        return true;
    }
public:
    /**
     * @brief constructor
     * @param separator char used as separator
     * @param quote char used as quote
     */
    parser(T * pointer = 0, char separator = ' ', char quote = '"', char backslash = '\\', char replacement = ','):
    separator_(separator),
    quote_(quote),pointer_(pointer), backslash_(backslash), escaper_(quote, backslash, separator, replacement) {
    }
    /**
     * @brief adds command to parser
     * @param name name of the command
     * @param function pointer to member to be called
     * @param params number of parameters to the command NO_PARAM_CHECK value to disable
     */
    void addCommand(const std::string & name, function_type function, int params = NO_PARAM_CHECK) {
        commands_[name] = typename map_type::mapped_type(function, params);
    }
    /**
     * @brief sets the pointer to the object on which the members will be executed
     * @param pointer pointer to object on which the members will be executed
     */
    void pointer(T * pointer) {
        pointer_ = pointer;
    }
    /**
     * @brief removes command from parser
     * @param name of the command to be removed
     */
    void eraseCommand(const std::string & name) {
        commands_.erase(name);
    }
    /**
     *@brief split line into command and parameters
     *@param line input line
     *@throw exception::unknownCommand when the unknown command is parsed
     *@throw exception::noCommand when the command is missing
     *@throw exception::invalidFormat when the line has invalid format
     *@throw exception::invalidNumberParams when there is wrong number of parameters to the command
     */
    void parse(const std::string & line) {
        std::string tmp;
        std::istringstream in(line);
        if(!getNext(in, tmp)) {
            throw vp::noCommand;
        }
        typename map_type::const_iterator it = commands_.find(tmp);
        if(it == commands_.end()) {
            throw vp::unknownCommand;
        }
        std::vector<std::string> params;
        while(getNext(in, tmp)) {
            params.push_back(tmp);
        };
        if(params.size() != it->second.second &&
                it->second.second != NO_PARAM_CHECK) {
            throw vp::invalidNumberParams;
        }
        VP_CALL_MEMBER(*pointer_, it->second.first)(params);
    }
};

utils_namespace_END

vp_namespace_END

#endif
