#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <algorithm>
#include "book.h"
#include "user.h"

/*
 * File: library.h
 * Description: Declaration of the Library class managing books and users.
 * Creation Date: 2025-04-17
 * Last Modified: 2025-04-17
 * Necessary Files: book.h, user.h, library.cpp
 */

const int MAX_BOOKS = 200; // Maximum number of books allowed in the library

class Library {
private:
    std::vector<Book> books; // Collection of books
    std::vector<User> users; // Collection of users
public:
    // Adds a user to the library
    bool addUser(const User& user);

    // Adds a book to the library. Returns false if capacity reached.
    bool addBook(const Book& book);

    // Lends a book to a user. Returns false if book unavailable or not found.
    bool lendBook(int bookId, int userId);

    // Returns a book from a user. Returns false if operation fails.
    bool returnBook(int bookId, int userId);

    // Counts and returns the number of available books.
    int countAvailableBooks() const;
};

#endif // LIBRARY_H
