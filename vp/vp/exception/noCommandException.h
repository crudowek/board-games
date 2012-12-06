#ifndef VP_EXCEPTION_NOCOMMAND
#define VP_EXCEPTION_NOCOMMAND

namespace vp {

/**
 *@brief noCommand exception
 * should be thrown when command was expected and none was given
 */
class noCommandException: public exception {
private:
    /**
     * @brief no command message
     */
    static const char * const msg_;
public:
    /**
     * @brief default constructor
     */
    noCommandException();
};

/**
 * @brief exception instance which is to be thrown
 */
extern noCommandException noCommand;

}
;

#endif
