/*
 * File: library.cpp
 * Description: Implementation of the Library class methods.
 * Creation Date: 2025-04-17
 * Last Modified: 2025-04-17
 * Necessary Files: library.h, book.h, user.h
 */

#include "library.h"

bool Library::addUser(const User& user) {
    users.push_back(user);
    return true;
}

bool Library::addBook(const Book& book) {
    if (books.size() >= MAX_BOOKS) {
        return false;
    }
    books.push_back(book);
    return true;
}

bool Library::lendBook(int bookId, int userId) {
    for (auto& book : books) {
        if (book.id == bookId && book.isAvailable) {
            for (auto& user : users) {
                if (user.id == userId) {
                    book.isAvailable = false;
                    user.borrowedBookIds.push_back(bookId);
                    return true;
                }
            }
        }
    }
    return false;
}

bool Library::returnBook(int bookId, int userId) {
    for (auto& user : users) {
        if (user.id == userId) {
            auto it = std::find(user.borrowedBookIds.begin(), user.borrowedBookIds.end(), bookId);
            if (it != user.borrowedBookIds.end()) {
                user.borrowedBookIds.erase(it);
                for (auto& book : books) {
                    if (book.id == bookId) {
                        book.isAvailable = true;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

int Library::countAvailableBooks() const {
    int count = 0;
    for (const auto& book : books) {
        if (book.isAvailable) {
            ++count;
        }
    }
    return count;
}
