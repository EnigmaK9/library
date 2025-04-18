#ifndef USER_H
#define USER_H

#include <vector>
#include "book.h"

/*
 * File: user.h
 * Description: Defines the User structure for the library system.
 * Creation Date: 2025-04-17
 * Last Modified: 2025-04-17
 * Necessary Files: book.h
 */

struct User {
    std::string name;              // Name of the user
    int id;                        // Unique identifier for the user
    std::vector<int> borrowedBookIds; // List of borrowed book IDs

    // Constructor to initialize a User object
    User(const std::string& name, int id);
};

#endif // USER_H
