#ifndef BG_MODULE
#define BG_MODULE

#define bg_namespace_BEGIN namespace bg{
#define bg_namespace_END };

#include <string>
#include <vector>

#include "declare.h"
#include "match.h"
#include "gameinfo.h"
#include "parameter.h"

bg_namespace_BEGIN

/**
 * interface to be implemented by all modules
 */
class module {
public:

    /**
     * player disconnected callback
     *
     * @param player disconnected player
     */
    virtual void playerLost(matchPlayer & player) = 0;
    /**
     * action performed callback
     *
     * @param action performed action
     */
    virtual void action(placementAction & action) = 0;

    virtual const gameInfo & info() const = 0;

    /**
     * match started callback
     *
     *
     */
    virtual void start() = 0;
    /**
     * counter run out of time callback
     *
     * @param counter reference to the counter
     */
    virtual void counterOff(matchCounter & counter) = 0;
    /**
     * destructor
     */
    virtual ~module() = 0;
};

/*
 extern "C"{
 void * createGameInfo();
 };
 */

bg_namespace_END

#endif
