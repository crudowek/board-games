#include <module.h>
#include <gameinfo.h>
#include <exception.h>

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>

class dummyInfo;

class dummyModule: public bg::module {
private:
    bg::match & m_;
    const bg::gameInfo & info_;

public:
    dummyModule(const dummyInfo & info_, bg::match & m);

    /**
     * player disconnected callback
     *
     * @param player disconnected player
     */
    void playerLost(bg::matchPlayer & player);
    /**
     * action performed callback
     *
     * @param action performed action
     */
    void action(bg::placementAction & action);

    /**
     * match started callback
     *
     *
     */
    void start();

    const bg::gameInfo & info() const;
    //bg::parameter & param(const std::string & name);
    /**
     * counter run out of time callback
     *
     * @param counter reference to the counter
     */
    void counterOff(bg::matchCounter & counter);
    /**
     * destructor
     */
    virtual ~dummyModule();
};

class dummyInfo: public bg::gameInfo {
private:
    std::string name_;
    std::vector<std::string> paramNames_;
    bg::parameter parameter_;
    std::vector<bg::req> reqs_;

public:
    /**
     * @brief gets name of the module
     * @return name of the module
     */
    const std::string & name() const;
    int height() const;
    int width() const;
    dummyInfo();
    const std::vector<std::string> & paramNames() const;
    bool verifyParameters(std::map<std::string, std::string> parameters) const;
    boost::shared_ptr<bg::module> createModule(bg::match & m);
    const bg::parameter & param(const std::string & name) const;
    const std::vector<bg::req> & reqs() const;
};

extern "C" {
boost::shared_ptr<bg::gameInfo> createGameInfo();
}
