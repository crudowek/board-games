#include "keeper.h"

#include "constants.h"
#include "client.h"
#include "roomKeeper.h"

bg_namespace_BEGIN

keeper::keeper(const keeper & original): escaper_(c::QUOTE, c::BACKSLASH, c::SEPARATOR, c::REPLACEMENT) {
}

keeper & keeper::operator=(const keeper & r) {
}

//const std::string * keeper::chatParameters()=0;

/**
 * @brief returns size of the array of parameters
 * @return number of chat parameters
 */
//keeper::size_type keeper::chatParametersSize()=0;
/**
 * @brief gets value for the parameter
 * @param name name of the parameter
 * @return value of the parameter
 */
//const std::string & keeper::parameter(const std::string & name) const=0;
keeper::keeper():name_(""), escaper_(c::QUOTE, c::BACKSLASH, c::SEPARATOR, c::REPLACEMENT) {
}
/**
 *@brief adds client inside
 *@param c client to be added
 */
//virtual void addClient(client & c);
/**
 * @brief removes client from inside
 * @param c client to be removed
 */
//virtual void removeClient(client & c);
/**
 * @brief sends message to the chat
 * @param what message to be sent
 */
/*void keeper::chat(message & what){
 chatParameters
 }
 */

size_t keeper::count(const std::string & where, char what) const {
    size_t res = 0;
    for(size_t i = 0; i < where.size(); ++i)
    {
        if(where[i] == what)
        ++res;
    }

    return res;
}

void keeper::msg(const std::string & author, const std::string & message) {
    std::string command;
    bool r = dynamic_cast<roomKeeper *>(this);
    if(r)
    {
        command = c::RMSG_COMMAND;
    }
    else
    {
        command = c::MMSG_COMMAND;
    }

    command += ' ';
    command += chatParameters();

    command += ' ';
    command += escaper_.escape(author);
    command += ' ';
    command += escaper_.escape(message);

    sendToAllClients(command);

}

/**
 * @brief name of the keeper object
 * @param name name of the object
 */
const std::string & keeper::name() const {
    return name_;
}

/**
 * @brief how often info about other clients is sent..
 * @return value in seconds
 */
//unsigned int period();
/**
 * @brief constructs new keeper object, creates thread for sending periodical updates
 * @param name name of the keeper object
 */
keeper::keeper(const std::string & name):
name_(name),
escaper_(c::QUOTE, c::BACKSLASH, c::SEPARATOR, c::REPLACEMENT) {
}

/**
 * @brief destructor terminates the thread sending periodical updates
 */
keeper::~keeper() {
}
bg_namespace_END

