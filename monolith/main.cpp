/*
 * File: principal.cpp
 * Description: Simple Library Management System
 * Creation Date: 2025-04-18
 * Last Modified: 2025-04-18
 * Necessary Files: principal.cpp
 *
 * Compilation (Debian 12):
 *   g++ -std=c++11 -o biblioteca principal.cpp
 *
 * Execution:
 *   ./biblioteca
 */

#include <iostream>   // For input/output
#include <string>     // For std::string

// Maximum number of books the system can handle
const int MAX_BOOKS = 200;

// Structure representing a Book in the library
struct Book {
    std::string title;     // Title of the book
    std::string author;    // Author of the book
    int id;                // Unique identifier for the book
    bool isAvailable;      // Availability status: true if available
};

// Structure representing a User of the library (reserved for future use)
struct User {
    std::string name;      // Name of the user
    int id;                // Unique identifier for the user
    // In a full implementation, this vector would store IDs of borrowed books
    // std::vector<int> borrowedBooks;
};

// Array to store books and a counter for how many have been added
Book books[MAX_BOOKS];
int bookCount = 0;

/**
 * addBook
 * Adds a new book to the library collection if the limit has not been reached.
 * Reads title, author, and id from the user and initializes availability to true.
 */
void addBook() {
    if (bookCount >= MAX_BOOKS) {
        std::cout << "Capacidad máxima de libros alcanzada." << std::endl;
        return;
    }

    // Temporary variables to hold input
    std::string title, author;
    int id;

    // Prompt user for book details (interface in Spanish)
    std::cout << "Ingrese el título del libro: ";
    std::getline(std::cin, title);
    std::cout << "Ingrese el autor del libro: ";
    std::getline(std::cin, author);
    std::cout << "Ingrese el ID del libro: ";
    std::cin >> id;
    std::cin.ignore(); // Clear newline from buffer

    // Initialize and store the new book
    books[bookCount] = { title, author, id, true };
    bookCount++;

    std::cout << "Libro agregado exitosamente." << std::endl;
}

/**
 * lendBook
 * Lends out a book if it exists and is available.
 * Prompts for the book ID, searches the collection, and updates availability.
 */
void lendBook() {
    int id;
    std::cout << "Ingrese el ID del libro a prestar: ";
    std::cin >> id;
    std::cin.ignore(); // Clear newline

    // Search for the book by ID
    for (int i = 0; i < bookCount; ++i) {
        if (books[i].id == id) {
            if (books[i].isAvailable) {
                // Mark as lent
                books[i].isAvailable = false;
                std::cout << "Libro prestado exitosamente." << std::endl;
            } else {
                std::cout << "El libro no está disponible." << std::endl;
            }
            return;
        }
    }

    // If book was not found
    std::cout << "Libro con ID " << id << " no encontrado." << std::endl;
}

/**
 * countAvailableBooks
 * Counts and returns the number of books currently available in the library.
 * No input is required; it simply iterates over the stored books.
 */
int countAvailableBooks() {
    int availableCount = 0;
    for (int i = 0; i < bookCount; ++i) {
        if (books[i].isAvailable) {
            ++availableCount;
        }
    }
    return availableCount;
}

int main() {
    int choice = -1;

    // Main loop: show menu until the user chooses to exit
    while (choice != 0) {
        std::cout << "\n=== Sistema de Gestión de Biblioteca ===\n";
        std::cout << "1. Agregar libro\n";
        std::cout << "2. Prestar libro\n";
        std::cout << "3. Mostrar cantidad de libros disponibles\n";
        std::cout << "0. Salir\n";
        std::cout << "Seleccione una opción: ";
        std::cin >> choice;
        std::cin.ignore(); // Clear newline

        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                lendBook();
                break;
            case 3:
                std::cout << "Libros disponibles: "
                          << countAvailableBooks() << std::endl;
                break;
            case 0:
                std::cout << "Saliendo del sistema. ¡Hasta luego!" << std::endl;
                break;
            default:
                std::cout << "Opción inválida. Intente de nuevo." << std::endl;
        }
    }

    return 0;
}
