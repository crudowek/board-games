#ifndef VP_EXCEPTION_NUMBERPARAMS
#define VP_EXCEPTION_NUMBERPARAMS

namespace vp {
/**
 *@brief  unknown command exception
 * should be thrown when the number of parameters is wrong
 */
class invalidNumberParamsException: public exception {
private:
    /**
     * @brief invalid number of parameters message
     */
    static const char * const msg_;
public:
    /**
     * @brief default constructor
     */
    invalidNumberParamsException();

};

/**
 * @brief exception instance which is to be thrown
 */
extern invalidNumberParamsException invalidNumberParams;

}
;
//end of vp namespace

#endif
