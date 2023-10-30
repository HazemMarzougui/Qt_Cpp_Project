#include "sessionuser.h"
sessionuser* sessionuser::instance = nullptr;

sessionuser::sessionuser()
{
    this->user.setId(0);

}

sessionuser* sessionuser::getInstance() {
    if (instance == nullptr) {
        instance = new sessionuser();
    }
    return instance;
}

employe& sessionuser::getUser() {
    return user;
}

void sessionuser::setUser(const employe& newUser) {
    user = newUser;
}
