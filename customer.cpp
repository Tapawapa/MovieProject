#include "customer.h"
#include "movie.h"
#include <iomanip>
#include <iostream>

// Constructs a Transaction object.
Transaction::Transaction(Type type, Movie *movie) : type(type), movie(movie) {}

// Returns a string representation of the transaction.
std::string Transaction::toString() const {
  std::string action = (type == BORROW) ? "Borrowed" : "Returned";
  if (movie != nullptr) {
    return action + " " + movie->getTitle();
  }
  return action + " [Unknown Movie]";
}

// Constructs a Customer object.
Customer::Customer(int id, const std::string &lastName,
                   const std::string &firstName)
    : id(id), lastName(lastName), firstName(firstName) {}

// Adds a transaction to the customer's history.
void Customer::addTransaction(Transaction::Type type, Movie *movie) {
  if (movie != nullptr) {
    history.emplace_back(type, movie);
  }
}

// Displays the customer's transaction history.
void Customer::displayHistory() const {
  std::cout << "History for " << id << " " << getFullName() << ":" << std::endl;

  if (history.empty()) {
    std::cout << "No history for " << getFullName() << std::endl;
    return;
  }

  for (const auto &txn : history) {
    std::string action = (txn.getType() == Transaction::BORROW) ? "Borrow" : "Return";
    const Movie* movie = txn.getMovie();
    if(movie != nullptr) {
        std::cout << action << " " << getFullName() << " " << movie->getTitle() << std::endl;
    }
  }
  std::cout << std::endl;
}