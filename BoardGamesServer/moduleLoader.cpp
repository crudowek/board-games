#include "moduleLoader.h"

#include <algorithm>
#include <string>

#include <boost/bind.hpp>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <boost/shared_ptr.hpp>

#include "module.h"
#include "constants.h"
#include "exception.h"

bg_namespace_BEGIN

typedef boost::shared_ptr<gameInfo> (*createModuleF)();

void moduleLoader::loadLib(boost::filesystem::path p)
{
    try {
        loadLibS(p.string());
    } catch(libraryNotExistException &) {
    } catch(libraryAlreadyLoadedException &) {
    }

}

void moduleLoader::loadLibS(const std::string & name) {
#ifdef _WIN32
    HMODULE handle = 0;
#else
    void * handle = 0;
#endif

    bool notFound = false;

    boost::shared_ptr<gameInfo> mod;

#ifdef _WIN32
    handle = LoadLibrary(name.c_str());
#else
    handle = dlopen(name.c_str() ,RTLD_LAZY);
#endif
    createModuleF createModule = 0;

    if(!handle)
    {
        notFound = true;
        goto end;
    }

#ifdef _WIN32
    createModule = reinterpret_cast<createModuleF>(GetProcAddress(handle, c::MODULE_FUNCTION_NAME));
#else
    createModule = reinterpret_cast<createModuleF>(dlsym(handle, c::MODULE_FUNCTION_NAME));
#endif
    if(!createModule)
    {
        notFound = true;
        goto end;
    }

    mod = (*createModule)();
    if(handles_.find(mod->name()) != handles_.end()) {
        notFound = false;
        goto end;
    }

    info_.push_back(mod);
    handles_.insert(std::make_pair(mod->name(), handle));
    return;

    end:
    if(handle) {
#ifdef _WIN32
        FreeLibrary(handle);
#else
        dlclose(handle);
#endif
    }
    notFound ? throw libraryNotExist : throw libraryAlreadyLoaded;
}

const moduleLoader::container & moduleLoader::infos() const
{
    return info_;
}

moduleLoader::container & moduleLoader::infos()
{
    return info_;
}

const gameInfo & moduleLoader::info(const std::string & name) const
{
    for(container::const_iterator it = info_.begin(); it != info_.end(); ++it)
    {
        if((*it)->name() == name)
        return *(*it);
    }

    throw gameNotFound;
}

gameInfo & moduleLoader::info(const std::string & name)
{
    for(container::iterator it = info_.begin(); it != info_.end(); ++it)
    {
        if((*it)->name() == name)
        return *(*it);
    }

    throw gameNotFound;
}

void moduleLoader::loadLibrary(const std::string & name) {
    boost::filesystem::path p = path_ / name;

    loadLibS(p.string());
}

moduleLoader::moduleLoader():path_(c::MODULE_DIR_NAME) {
    if(boost::filesystem::is_directory(path_))
    {
        std::for_each(boost::filesystem::directory_iterator(path_), boost::filesystem::directory_iterator(), boost::bind(&moduleLoader::loadLib, this, _1));
    }
}

moduleLoader::~moduleLoader() {
    info_.clear();

    for(handle_container::iterator it = handles_.begin(); it != handles_.end(); ++it) {
#ifdef _WIN32
        FreeLibrary(it->second);
#else
        dlclose(it->second);
#endif
    }
}

bg_namespace_END
