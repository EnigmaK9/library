/*
 * File: main.cpp
 * Description: Demonstrates the Library Management System usage.
 * Creation Date: 2025-04-17
 * Last Modified: 2025-04-17
 * Necessary Files: library.h, book.h, user.h, library.cpp, book.cpp, user.cpp
 */

#include <iostream>
#include "library.h"
#include "book.h"
#include "user.h"

int main() {
    Library library; // Create library instance

    // Create and add users
    User user1("Alice Smith", 1);
    User user2("Bob Johnson", 2);
    library.addUser(user1);
    library.addUser(user2);

    // Create and add books
    Book book1("1984", "George Orwell", 101);
    Book book2("To Kill a Mockingbird", "Harper Lee", 102);
    library.addBook(book1);
    library.addBook(book2);

    // Lend book 101 to user 1
    if (library.lendBook(101, 1)) {
        std::cout << "Book 101 lent to user 1." << std::endl;
    } else {
        std::cout << "Failed to lend book 101." << std::endl;
    }

    // Attempt to lend the same book again
    if (!library.lendBook(101, 2)) {
        std::cout << "Book 101 is not available for user 2." << std::endl;
    }

    // Return book 101 from user 1
    if (library.returnBook(101, 1)) {
        std::cout << "Book 101 returned by user 1." << std::endl;
    }

    // Count available books
    std::cout << "Available books: " << library.countAvailableBooks() << std::endl;

    return 0;
}
