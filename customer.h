#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <memory>
#include <string>
#include <vector>

class Movie;

// Represents a single customer transaction (borrow or return).
class Transaction {
public:
  enum Type { BORROW, RETURN };

  // Constructs a new Transaction.
  Transaction(Type type, Movie *movie);
  ~Transaction() = default;

  // Gets the type of the transaction.
  Type getType() const { return type; }
  // Gets the movie associated with the transaction.
  const Movie *getMovie() const { return movie; }
  // Returns a string representation of the transaction.
  std::string toString() const;

private:
  Type type;
  Movie *movie;
};

// Represents a customer of the store.
class Customer {
public:
  // Constructs a new Customer.
  Customer(int id, const std::string &lastName, const std::string &firstName);
  ~Customer() = default;

  // Adds a new transaction to the customer's record.
  void addTransaction(Transaction::Type type, Movie *movie);
  // Displays the transaction history for the customer.
  void displayHistory() const;

  // Gets the customer's ID.
  int getId() const { return id; }
  // Gets the customer's last name.
  const std::string &getLastName() const { return lastName; }
  // Gets the customer's first name.
  const std::string &getFirstName() const { return firstName; }
  // Gets the customer's full name.
  std::string getFullName() const { return firstName + " " + lastName; }

private:
  int id;
  std::string lastName;
  std::string firstName;
  std::vector<Transaction> history;
};

#endif // CUSTOMER_H