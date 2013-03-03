#ifndef BG_MATCHCLIENT
#define BG_MATCHCLIENT

#include "crossdependence.h"
#include "match.h"
#include "client.h"
#include "matchKeeper.h"

#define bg_namespace_BEGIN namespace bg{
#define bg_namespace_END };

bg_namespace_BEGIN

class matchClient: public matchPlayer {
private:
    bg::client & client_;
    bg::view view_;
    matchKeeper & match_;
    bool connected_;
    bool active_;
public:
    matchClient(bg::client & c, matchKeeper & m, bool active);
    bg::match & match();
    const bg::match & match() const;
    bg::view & view();
    const bg::view & view() const;
    bool spectator() const;
    bool connected() const;
    bg::client & client();
    const bg::client & client() const;
    const std::string & name() const;
    void view(bg::view & v);
};

bg_namespace_END
#endif
