#ifndef BG_SECURITY
#define BG_SECURITY

#include <cryptopp/sha.h>

#include <string>
#include <vector>

#define bg_namespace_BEGIN namespace bg{
#define bg_namespace_END };

bg_namespace_BEGIN

class security {
private:
    std::vector<unsigned char> lastHash_;
    CryptoPP::SHA1 sha_;

public:
    security();
    const std::vector<unsigned char> & generateHash(const std::string & pw, const std::string & serverSalt, const std::string & clientSalt);
};

bg_namespace_END

#endif
