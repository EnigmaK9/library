#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <algorithm>
#include <string>
#include "book.h"
#include "user.h"

const int MAX_BOOKS = 200;                   // Max books allowed
static constexpr double LATE_FEE_PER_DAY = 0.50; // USD per day

class Library {
private:
    std::vector<Book> books;
    std::vector<User> users;

public:
    bool addUser(const User& user);
    bool addBook(const Book& book);
    bool lendBook(int bookId, int userId);
    bool returnBook(int bookId, int userId);
    int  countAvailableBooks() const;
    std::vector<Book> searchBooksByAuthor(const std::string& author) const;
    double calculateFine(int daysLate) const;
};
#endif // LIBRARY_H
