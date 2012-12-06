#include "matchClient.h"

bg_namespace_BEGIN

matchClient::matchClient(bg::client & cl, matchKeeper & m, bool active):
matchPlayer(),
client_(cl),
match_(m),
connected_(true),
active_(active) {
}

bg::view & matchClient::view() {
    return view_;
}

const bg::view & matchClient::view() const {
    return view_;
}

bool matchClient::spectator() const {
    return !active_;
}

bool matchClient::connected() const {
    return connected_;
}

bg::client & matchClient::client() {
    return client_;
}

const bg::client & matchClient::client() const {
    return client_;
}

bg::match & matchClient::match() {
    return match_;
}

const bg::match & matchClient::match() const {
    return match_;
}

const std::string & matchClient::name() const {
    return client_.name();
}

void matchClient::view(bg::view & v) {
}

bg_namespace_END
