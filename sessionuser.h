#ifndef SESSIONUSER_H
#define SESSIONUSER_H


#include "employe.h"
class sessionuser {
private:
    employe user;  // Instance of type User
    static sessionuser* instance;  // Singleton instance

    // Private constructor to prevent direct instantiation
    sessionuser();

public:
    // Static method to get the singleton instance
    static sessionuser* getInstance();

    // Public method to access the User instance
    employe& getUser();

    // Public method to set the User instance
    void setUser(const employe& newUser);

    // Delete copy constructor and assignment operator
 //   sessionuser(const sessionuser&) = delete;
//    sessionuser& operator=(const sessionuser&) = delete;
};

#endif // SESSIONUSER_H
