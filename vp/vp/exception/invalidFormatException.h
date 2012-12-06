#ifndef VP_EXCEPTION_INVALIDFORMAT
#define VP_EXCEPTION_INVALIDFORMAT

namespace vp {

/**
 *@brief  invalid format exception
 * should be thrown when input has invalid format
 */
class invalidFormatException: public exception {
private:
    /**
     * @brief invalid format message
     */
    static const char * const msg_;
public:
    /**
     * @brief default constructor
     */
    invalidFormatException();
};

/**
 * @brief exception instance which is to be thrown
 */
extern invalidFormatException invalidFormat;

}
;

#endif
