#ifndef BOOK_H
#define BOOK_H

#include <string>

/*
 * File: book.h
 * Description: Defines the Book structure for the library system.
 * Creation Date: 2025-04-17
 * Last Modified: 2025-04-17
 * Necessary Files: none
 */

struct Book {
    std::string title;    // Title of the book
    std::string author;   // Author of the book
    int id;               // Unique identifier for the book
    bool isAvailable;     // Availability status of the book

    // Constructor to initialize a Book object
    Book(const std::string& title, const std::string& author, int id, bool isAvailable = true);
};

#endif // BOOK_H
