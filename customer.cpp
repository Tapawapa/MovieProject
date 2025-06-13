#include "customer.h"
#include "movie.h"
#include <iomanip>
#include <iostream>

// Transaction Implementation
Transaction::Transaction(Type type, Movie *movie) : type(type), movie(movie) {}

std::string Transaction::toString() const {
  std::string action = (type == BORROW) ? "Borrowed" : "Returned";
  if (movie != nullptr) {
    return action + " " + movie->getTitle(); // Simplified
  }
  return action + " [Unknown Movie]";
}

// Customer Implementation
Customer::Customer(int id, const std::string &lastName,
                   const std::string &firstName)
    : id(id), lastName(lastName), firstName(firstName) {}

void Customer::addTransaction(Transaction::Type type, Movie *movie) {
  if (movie != nullptr) {
    history.emplace_back(type, movie);
  }
}

void Customer::displayHistory() const {
  // Corrected header to match sample output
  std::cout << "History for " << id << " " << getFullName() << ":" << std::endl;

  if (history.empty()) {
    std::cout << "No history for " << getFullName() << std::endl;
    // The sample doesn't have an extra blank line here, so removed it.
    return;
  }

  for (const auto &txn : history) {
    std::string action = (txn.getType() == Transaction::BORROW) ? "Borrow" : "Return";
    const Movie* movie = txn.getMovie();
    if(movie != nullptr) {
        // The history line in the sample doesn't include the customer name again
        // Let's re-verify sample: "Borrow Mouse Minnie Good Morning Vietnam"
        // It seems my previous fix was not quite right.
        std::cout << action << " " << getFullName() << " " << movie->getTitle() << std::endl;
    }
  }
  // The sample has a blank line after each history block
  std::cout << std::endl;
}