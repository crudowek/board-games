#ifndef VP_UTILS_ESCAPER
#define VP_UTILS_ESCAPER

#include <string>

#define vp_namespace_BEGIN namespace vp{
#define vp_namespace_END };
#define utils_namespace_BEGIN namespace utils{
#define utils_namespace_END };

vp_namespace_BEGIN

utils_namespace_BEGIN/**
 * @brief class escaping and stripping by use of the slash equivalent
 * also separators are not only escaped but also replaced by replacement to allow simpler parsing
 *
 */
class escaper {
private:
    char quote_;
    char backslash_;
    char separator_;
    char replacement_;
    typedef std::string::size_type size_type;
public:
    escaper(char quote, char backslash, char separator, char replacement);

    std::string & escape(std::string & what) const;

    std::string escape(const std::string & what) const;

    void strip(std::string & what) const;

};

utils_namespace_END

vp_namespace_END

#endif
