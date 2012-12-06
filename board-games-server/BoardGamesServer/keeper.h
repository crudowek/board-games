#ifndef BG_KEEPER
#define BG_KEEPER

#include <string>
#include <vp/utils.h>

#include "crossdependence.h"
//#include "message.h"

#define bg_namespace_BEGIN namespace bg{
#define bg_namespace_END };

bg_namespace_BEGIN

/**
 *	@brief abstract class that periodically informs clients inside about changes, chat messages and etc..
 */
class keeper {
public:
    typedef size_t size_type;
protected:
    vp::utils::escaper escaper_;
private:
    std::string name_;
    /**
     * @brief copy constructor
     * should not be used
     * @param original keeper to be copied
     */
    keeper(const keeper & original);
    /**
     * @brief assignment operator
     * should not be used
     * @param r keeper to be assigned from
     */
    keeper & operator=(const keeper & r);

    /**
     * @brief gets chat parameters
     * @return string of chat parameters separated by ' '
     */
    virtual std::string chatParameters() const = 0;

    /**
     * @brief returns size of the array of parameters
     * @return number of chat parameters
     */
    //	virtual size_type chatParametersSize() const = 0;
    /**
     * @brief gets value for the parameter
     * @param name name of the parameter
     * @return value of the parameter
     */
//		virtual const std::string & parameter(const std::string & name) const = 0;
    size_t count(const std::string & where, char what) const;

public:
    keeper();

    void msg(const std::string & author, const std::string & message);
    /**
     *	@brief adds client inside
     *	@param c client to be added
     */
    //	virtual void addClient(client & c);
    /**
     * @brief removes client from inside
     * @param c client to be removed
     */
    //	virtual void removeClient(client & c);
    /**
     * @brief sends message to the chat
     * @param what message to be sent
     */
    //void chat(message & what);
    /**
     * @brief name of the keeper object
     * @param name name of the object
     */
    const std::string & name() const;

    virtual void sendToAllClients(const std::string & msg) = 0;

    /**
     * @brief how often info about other clients is sent..
     * @return value in seconds
     */
    //unsigned int period();
    /**
     * @brief constructs new keeper object, creates thread for sending periodical updates
     * @param name name of the keeper object
     */
    keeper(const std::string & name);

    /**
     * @brief destructor terminates the thread sending periodical updates
     */
    virtual ~keeper();
};

bg_namespace_END

#endif
