/*
 * File: book.cpp
 * Description: Implementation of the Book constructor.
 * Creation Date: 2025-04-17
 * Last Modified: 2025-04-17
 * Necessary Files: book.h
 */

#include "book.h"

Book::Book(const std::string& title, const std::string& author, int id, bool isAvailable)
    : title(title), author(author), id(id), isAvailable(isAvailable) {}
