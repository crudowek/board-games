#include "dummyModule.h"

void dummyModule::playerLost(bg::matchPlayer & player) {
}

void dummyModule::action(bg::placementAction & action) {
}

void dummyModule::start() {
}

/*bg::parameter & dummyModule::param(const std::string & name){
 return parameter_;
 }*/

const bg::parameter & dummyInfo::param(const std::string & name) const {
    throw bg::parameterNotFound;
    return parameter_;
}

void dummyModule::counterOff(bg::matchCounter & counter) {
}

dummyModule::~dummyModule() {
}

dummyModule::dummyModule(const dummyInfo & info, bg::match & m) :
        info_(info), m_(m) {
}

const std::string & dummyInfo::name() const {
    return name_;
}

bool dummyInfo::verifyParameters(
        std::map<std::string, std::string> parameters) const {
    return true;
}

int dummyInfo::height() const {
    return 0;
}

int dummyInfo::width() const {
    return 0;
}

const bg::gameInfo & dummyModule::info() const {
    return info_;
}

const std::vector<std::string> & dummyInfo::paramNames() const {
    return paramNames_;
}

const std::vector<bg::req> & dummyInfo::reqs() const {
    return reqs_;
}

dummyInfo::dummyInfo() :
        name_("dummyModule") {
}

boost::shared_ptr<bg::module> dummyInfo::createModule(bg::match & m) {
    return boost::shared_ptr<bg::module>(new dummyModule(*this, m));
}

extern "C" {
boost::shared_ptr<bg::gameInfo> createGameInfo() {
    return boost::shared_ptr<bg::gameInfo>(new dummyInfo());
}
}
