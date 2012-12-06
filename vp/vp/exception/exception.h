#ifndef VP_EXCEPTION_EXCEPTION
#define VP_EXCEPTION_EXCEPTION

namespace vp {

/**
 * @brief exception from which every other thrown by anything in vp namespace is derived
 *
 */
class exception {
private:
    /**
     * @brief  message
     */
    const char * const what_;
    /**
     * @brief error code
     */
    const int code_;
    /**
     * @brief default message
     */
    static const char * const defaultMessage_;
protected:
    /**
     * @brief constructor
     * @param what message
     */
    exception(const char * what, int code);
public:
    enum {
        DEFAULT = 0,
        NO_COMMAND = 10,
        FORMAT = 30,
        INVALID_NUMBER = 40,
        UNKNOWN = 20
    };
    /**
     * @brief default constructor
     * constructs exception with default message and code
     */
    exception();
    /**
     * @brief gets message
     * @return message
     */
    const char * what() const;
    /**
     * @brief gets error code
     * @return error code
     */
    int code() const;
    /**
     * @brief destructor
     */
    virtual ~exception();

};

}
;

#endif
