#ifndef BG_GAMEINFO
#define BG_GAMEINFO

#include "declare.h"
#include "module.h"
#include "match.h"
#include "parameter.h"
#include "req.h"

#include <boost/shared_ptr.hpp>

#define bg_namespace_BEGIN namespace bg{
#define bg_namespace_END };

bg_namespace_BEGIN

class gameInfo {
public:
    /**
     * @brief gets name of the module
     * @return name of the module
     */
    virtual const std::string & name() const = 0;
    virtual bool verifyParameters(std::map<std::string, std::string> parameters) const = 0;
    virtual int height() const = 0;
    virtual int width() const = 0;
    virtual const std::vector<std::string> & paramNames() const = 0;
//virtual parameter & param(const std::string & name) = 0;
    virtual const parameter & param(const std::string & name) const = 0;

    virtual boost::shared_ptr<module> createModule(match & m) = 0;
    virtual const std::vector<req> & reqs() const = 0;

    virtual ~gameInfo() = 0;
};

bg_namespace_END

#endif
