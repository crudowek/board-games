#include "security.h"

bg_namespace_BEGIN

const std::vector<unsigned char> & security::generateHash(const std::string & pw, const std::string & serverSalt, const std::string & clientSalt) {
    sha_.Restart();
    sha_.Update(reinterpret_cast<const unsigned char *>(pw.data()), pw.size());
    sha_.Update(reinterpret_cast<const unsigned char *>(serverSalt.data()), serverSalt.size());
    sha_.Update(reinterpret_cast<const unsigned char *>(clientSalt.data()), clientSalt.size());
    sha_.Final(&(lastHash_[0]));
    return lastHash_;
}

security::security():lastHash_(static_cast<std::vector<unsigned char>::size_type>(CryptoPP::SHA1::DIGESTSIZE), 0) {
};

bg_namespace_END
