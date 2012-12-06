#ifndef BG_MATCHKEEPER
#define BG_MATCHKEEPER

#include "keeper.h"
#include "client.h"
#include "match.h"
#include "parameter.h"
#include "module.h"
#include "matchClient.h"

#include <vector>
#include <map>
#include <string>

#define bg_namespace_BEGIN namespace bg{
#define bg_namespace_END };

bg_namespace_BEGIN

/**
 * @brief Class that creates thread sending periodically information about changes inside match and
 * And also sends information about clients joining/leaving the match to the other one
 */
class matchKeeper: public keeper, public virtual match {
private:
    roomKeeper & room_;
    unsigned int parameterVersion_;
    boost::shared_ptr<module> module_;
    player_container players_;
    player_container spectators_;

    std::map<std::string, std::string> settings_;

public:
    //typedef std::map<std::string, boost::shared_ptr<client> > client_container;
    //typedef std::map<std::string, boost::shared_ptr<client> > player_container;
    typedef std::vector<bg::parameter> parameter_container;
    typedef keeper::size_type size_type;

    matchKeeper(const std::string & name, roomKeeper & room, const std::string & game,const std::string & params);
    /**
     * @brief adds client to the match
     * @param which client to be added
     */
    //void addClient(client & which);
    /**
     * @brief removes client from the match
     * @param which client to be removed
     */
    //void removeClient(client & which);
    //const std::string & name() const;
    void join(const std::string & who, bool spectator);
    void leave(const std::string & who);

    int parameterVersion() const;
    bool isFirstPlayer(const std::string & name) const;
    const parameter_container & parameters() const;
    void setParam(const std::string & who, const std::string & version, const std::string & params);
    void reqParam(const std::string & who, const std::string & params);
    void ready(const std::string & who, const std::string & version);
    void choose(const std::string & author, const std::string & who);
    void unchoose(const std::string & author, const std::string & who);
    const roomKeeper & room() const;
    roomKeeper & room();
    parameter_container & parameters();
    const module & game() const;
    const std::string & name() const;
    player_container & players();
    const player_container & players() const;
    player_container & spectators();
    const player_container & spectators() const;
    std::map<std::string, std::string> & settings();
    bool hasClient(const std::string & name) const;
    std::string parameterList() const;
    bool state() const;

    void sendToAllClients(const std::string & msg);

private:
    void round(const std::string & name);

    void end(const std::string & result);
    /**
     * @brief gets room name and match name separated by ' '
     * @return name of the room and name of the match separated by ' '
     */
    std::string chatParameters() const;
    parameter_container parameters_;

    /**
     * @brief returns size of the array of parameters
     * @return number of chat parameters
     */
    //	size_type chatParametersSize() const;
    /**
     * @brief gets value for the parameter
     * @param name name of the parameter
     * @return value of the parameter
     */
    //	const std::string & parameter(const std::string & name) const;
};

bg_namespace_END

#endif
