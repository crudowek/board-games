#ifndef BG_MODULELOADER
#define BG_MODULELOADER

#include <vector>
#include <string>
#include <map>

#include <boost/filesystem.hpp>

#include "module.h"
#include "gameinfo.h"
#include "exception.h"

#ifdef _WIN32
#include <windows.h>
#endif

#define bg_namespace_BEGIN namespace bg{
#define bg_namespace_END };

bg_namespace_BEGIN

class moduleLoader {
public:
    typedef std::vector<boost::shared_ptr<gameInfo> > container;

private:
    boost::filesystem::path path_;

    void loadLib(boost::filesystem::path p);
    void loadLibS(const std::string & p);

#ifdef _WIN32
    typedef HMODULE handle_t;
#else
    typedef void * handle_t;
#endif

    typedef std::map<std::string, handle_t> handle_container;

    handle_container handles_;

    container info_;
public:
    /**
     * @brief gets modules
     * @return reference to vector of pointers to modules
     */
    container & infos();
    moduleLoader();
    const container & infos() const;
    void loadLibrary(const std::string & name);

    const gameInfo & info(const std::string & name) const;
    gameInfo & info(const std::string & name);

    ~moduleLoader();

};

bg_namespace_END

#endif
