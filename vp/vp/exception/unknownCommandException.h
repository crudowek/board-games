#ifndef VP_EXCEPTION_UNKNOWNCOMMAND
#define VP_EXCEPTION_UNKNOWNCOMMAND

namespace vp {
/**
 *@brief  unknown command exception
 * should be thrown when unknown command was parsed
 */
class unknownCommandException: public exception {
private:
    /**
     * @brief unknown command message
     */
    static const char * const msg_;
public:
    /**
     * @brief default constructor
     */
    unknownCommandException();
};

/**
 * @brief exception instance which is to be thrown
 */
extern unknownCommandException unknownCommand;

}
;

#endif
