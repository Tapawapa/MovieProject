#ifndef STORE_H
#define STORE_H

#include "command.h"
#include "customer.h"
#include "movie.h"
#include "movie_factory.h"
#include <fstream>
#include <memory>
#include <set>
#include <string>
#include <vector>

class Movie;
class Command;

// Custom comparator for sorting movies in the inventory.
struct MovieComparator {
  bool operator()(const std::unique_ptr<Movie> &a,
                  const std::unique_ptr<Movie> &b) const {
    if (a->getGenre() != b->getGenre()) {
      if (a->getGenre() == 'F') {
        return true;
      }
      if (b->getGenre() == 'F') {
        return false;
      }
      if (a->getGenre() == 'D') {
        return true;
      }
      if (b->getGenre() == 'D') {
        return false;
      }
      return false;
    }
    return *a < *b;
  }
};

class Store {
public:
  // Constructs a new Store object.
  Store();
  ~Store() = default;

  // Loads movies from a given file.
  bool loadMovies(const std::string &filename);
  // Loads customers from a given file.
  bool loadCustomers(const std::string &filename);
  // Processes commands from a given file.
  bool processCommands(const std::string &filename);

  // Finds a movie based on its genre and specific search criteria.
  Movie *findMovie(char genre, const std::string &searchCriteria);
  // Handles the borrowing of a movie by a customer.
  bool borrowMovie(int customerId, char mediaType, char movieType,
                   const std::string &movieInfo);
  // Handles the return of a movie by a customer.
  bool returnMovie(int customerId, char mediaType, char movieType,
                   const std::string &movieInfo);

  // Finds a customer by their ID.
  Customer *findCustomer(int customerId);

  // Displays the current inventory of movies.
  void displayInventory();
  // Displays the transaction history for a specific customer.
  void displayCustomerHistory(int customerId);

private:
  std::set<std::unique_ptr<Movie>, MovieComparator> movies;
  HashTable<int, Customer *> customers;
  std::vector<std::unique_ptr<Customer>> customerStorage;

  // Parses the search criteria string for a movie.
  static std::string parseMovieSearchCriteria(char genre,
                                              const std::string &info);
  // Trims leading and trailing whitespace from a string.
  static void trimString(std::string &str);
  // Splits a string into a vector of substrings based on a delimiter.
  static std::vector<std::string> split(const std::string &str, char delimiter);
};

#endif // STORE_H