#include <iostream>
#include <string>
#include "library.h"
#include "book.h"
#include "user.h"

void showMenu() {
    std::cout << "\nLibrary Menu:\n"
              << "1. Add User\n"
              << "2. Add Book\n"
              << "3. Lend Book\n"
              << "4. Return Book\n"
              << "5. Count Available Books\n"
              << "6. Search Books by Author\n"
              << "7. Calculate Late Fine\n"
              << "0. Exit\n"
              << "Choose an option: ";
}

int main() {
    Library lib;
    int choice;
    do {
        showMenu();
        std::cin >> choice;
        switch (choice) {
            case 1: {
                std::string name;
                int uid;
                std::cout << "Enter user name: ";
                std::cin.ignore();
                std::getline(std::cin, name);
                std::cout << "Enter user ID: "; std::cin >> uid;
                lib.addUser(User(name, uid));
                std::cout << "User added.\n";
                break;
            }
            case 2: {
                std::string title, author;
                int bid;
                std::cout << "Enter book title: ";
                std::cin.ignore(); std::getline(std::cin, title);
                std::cout << "Enter author: "; std::getline(std::cin, author);
                std::cout << "Enter book ID: "; std::cin >> bid;
                if (lib.addBook(Book(title, author, bid)))
                    std::cout << "Book added.\n";
                else
                    std::cout << "Library is full.\n";
                break;
            }
            case 3: {
                int bid, uid;
                std::cout << "Book ID to lend: "; std::cin >> bid;
                std::cout << "User ID: "; std::cin >> uid;
                std::cout << (lib.lendBook(bid, uid) ? "Lend successful.\n" : "Lend failed.\n");
                break;
            }
            case 4: {
                int bid, uid;
                std::cout << "Book ID to return: "; std::cin >> bid;
                std::cout << "User ID: "; std::cin >> uid;
                std::cout << (lib.returnBook(bid, uid) ? "Return successful.\n" : "Return failed.\n");
                break;
            }
            case 5:
                std::cout << "Available books: " << lib.countAvailableBooks() << "\n";
                break;
            case 6: {
                std::string author;
                std::cout << "Enter author name: ";
                std::cin.ignore(); std::getline(std::cin, author);
                auto found = lib.searchBooksByAuthor(author);
                if (found.empty()) std::cout << "No books found.\n";
                else for (auto& b : found)
                    std::cout << b.id << ": " << b.title << "\n";
                break;
            }
            case 7: {
                int days;
                std::cout << "Days late: "; std::cin >> days;
                std::cout << "Fine: $" << lib.calculateFine(days) << "\n";
                break;
            }
            case 0:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid option.\n";
        }
    } while (choice != 0);
    return 0;
}
