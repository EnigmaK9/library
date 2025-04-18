#include "library.h"

bool Library::addUser(const User& user) {
    users.push_back(user);
    return true;
}

bool Library::addBook(const Book& book) {
    if (books.size() >= MAX_BOOKS) return false;
    books.push_back(book);
    return true;
}

bool Library::lendBook(int bookId, int userId) {
    for (auto& b : books) {
        if (b.id == bookId && b.isAvailable) {
            for (auto& u : users) {
                if (u.id == userId) {
                    b.isAvailable = false;
                    u.borrowedBookIds.push_back(bookId);
                    return true;
                }
            }
        }
    }
    return false;
}

bool Library::returnBook(int bookId, int userId) {
    for (auto& u : users) {
        if (u.id == userId) {
            auto it = std::find(u.borrowedBookIds.begin(), u.borrowedBookIds.end(), bookId);
            if (it != u.borrowedBookIds.end()) {
                u.borrowedBookIds.erase(it);
                for (auto& b : books) {
                    if (b.id == bookId) {
                        b.isAvailable = true;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

int Library::countAvailableBooks() const {
    int cnt = 0;
    for (auto& b : books) if (b.isAvailable) ++cnt;
    return cnt;
}

std::vector<Book> Library::searchBooksByAuthor(const std::string& author) const {
    std::vector<Book> res;
    for (auto& b : books) if (b.author == author) res.push_back(b);
    return res;
}

double Library::calculateFine(int daysLate) const {
    return (daysLate > 0) ? daysLate * LATE_FEE_PER_DAY : 0.0;
}
