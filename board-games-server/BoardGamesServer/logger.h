#ifndef BG_LOGGER
#define BG_LOGGER

#include <string>

#include <boost/utility.hpp>

#define bg_namespace_BEGIN namespace bg{
#define bg_namespace_END };

bg_namespace_BEGIN

/**
 * Class used to logging
 */
class logger: boost::noncopyable {
public:
    enum message_type {
        error,
        information,
        bug
    };
private:
    /**
     * @brief logs message
     * @param type type of the message
     * @param what message
     * @tparam T type of the message
     */
    template<typename T>
    void log_(message_type type, const T & what) {
        std::string mtype;
        std::cerr << std::endl;
        switch(type) {
            case error:
            mtype = "error";
            break;
            case information:
            mtype = "information";
            break;
            case bug:
            mtype = "bug";
            break;
            default:
            mtype = "unknown";
            break;
        }
        std::cerr << mtype << ": " << what;
    }

    template<typename T>
    void detail_(const T & what) {
        std::cerr << ": " << what;
    }
public:
    /**
     * @brief Creates logger
     * Uses current timestamp to name the file into which is to be logged
     * @param folder folder to which is to be logged
     */
    logger(const std::string & folder) {
    }
    /**
     * @brief logs string
     * @param what what is to be logged
     */
    void log(message_type type, const char * what) {
        log_(type, what);
    }

    /**
     * @brief adds details
     * @param what what is to be logged
     */
    void addDetail(const char * what) {
        detail_(what);
    }

    void addDetail(const std::string & what) {
        detail_(what);
    }
    /**
     * @brief logs string
     * @param what what is to be logged
     */
    void log(message_type type, const std::string & what) {
        log_(type, what);
    }
    ~logger() {
        std::cerr << std::endl;
    }
};

bg_namespace_END

#endif
