#include "Store.h"
#include "command.h"
#include "customer.h"
#include "movie.h"
#include "movie_factory.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// Constructs a new Store object.
Store::Store() {}

// Loads movies from a specified file into the store's inventory.
bool Store::loadMovies(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Cannot open " << filename << std::endl;
    return false;
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    std::string tempLine = line;
    trimString(tempLine);
    std::vector<std::string> parts = split(tempLine, ',');

    if (parts.size() < 5) {
      std::cerr << "Error: Invalid movie format: " << line << std::endl;
      continue;
    }

    for (auto &part : parts) {
      trimString(part);
    }

    char genre = parts[0][0];
    int stock;

    try {
      stock = std::stoi(parts[1]);
    } catch (...) {
      std::cerr << "Error: Invalid stock number in: " << line << std::endl;
      continue;
    }

    std::string director = parts[2];
    std::string title = parts[3];
    std::string extra = parts[4];

    for (size_t i = 5; i < parts.size(); i++) {
      extra += "," + parts[i];
    }

    Movie *movie = MovieFactory::getInstance().createMovie(
        genre, stock, director, title, extra);
    if (movie != nullptr) {
      movies.insert(std::unique_ptr<Movie>(movie));
    } else {
      std::cout << "Unknown movie type: " << genre
                << ", discarding line: " << line << std::endl;
    }
  }

  return true;
}

// Loads customer data from a specified file.
bool Store::loadCustomers(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Cannot open " << filename << std::endl;
    return false;
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    std::istringstream iss(line);
    int id;
    std::string lastName;
    std::string firstName;

    if (iss >> id >> lastName >> firstName) {
      auto customer = std::make_unique<Customer>(id, lastName, firstName);
      customers.insert(id, customer.get());
      customerStorage.push_back(std::move(customer));
    } else {
      std::cerr << "Error parsing customer line: " << line << std::endl;
    }
  }
  return true;
}

// Processes a file of commands.
bool Store::processCommands(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Cannot open " << filename << std::endl;
    return false;
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    Command *cmd = CommandFactory::getInstance().createCommand(line);
    if (cmd != nullptr) {
      cmd->execute(*this);
      delete cmd;
    }
  }
  return true;
}

// Finds a movie in the inventory based on its genre and search criteria.
Movie *Store::findMovie(char genre, const std::string &searchCriteria) {
  std::string parsedCriteria = parseMovieSearchCriteria(genre, searchCriteria);

  for (const auto &moviePtr : movies) {
    if (moviePtr->getGenre() == genre) {
      if (genre == 'F') { // Comedy: Title, Year
        std::vector<std::string> parts = split(parsedCriteria, ',');
        if (parts.size() >= 2) {
          std::string searchTitle = parts[0];
          trimString(searchTitle);
          int searchYear = 0;
          try {
            searchYear = std::stoi(parts[1]);
          } catch (...) {
            continue;
          }

          const Comedy *comedy = dynamic_cast<const Comedy *>(moviePtr.get());
          if ((comedy != nullptr) && comedy->getTitle() == searchTitle &&
              comedy->getYear() == searchYear) {
            return moviePtr.get();
          }
        }
      } else if (genre == 'D') { // Drama: Director, Title
        std::vector<std::string> parts = split(parsedCriteria, ',');
        if (parts.size() >= 2) {
          std::string searchDirector = parts[0];
          std::string searchTitle = parts[1];
          trimString(searchDirector);
          trimString(searchTitle);

          const Drama *drama = dynamic_cast<const Drama *>(moviePtr.get());
          if ((drama != nullptr) && drama->getDirector() == searchDirector &&
              drama->getTitle() == searchTitle) {
            return moviePtr.get();
          }
        }
      } else if (genre == 'C') { // Classic: Month Year Actor
        std::istringstream iss(parsedCriteria);
        int searchMonth = 0;
        int searchYear = 0;
        std::string actorFirstName;
        std::string actorLastName;

        iss >> searchMonth >> searchYear >> actorFirstName >> actorLastName;
        std::string searchActor = actorFirstName + " " + actorLastName;
        trimString(searchActor);

        const Classic *classic = dynamic_cast<const Classic *>(moviePtr.get());
        if ((classic != nullptr) && classic->getMonth() == searchMonth &&
            classic->getYear() == searchYear &&
            classic->getActor() == searchActor) {
          return moviePtr.get();
        }
      }
    }
  }
  return nullptr;
}

// Finds a customer in the store by their ID.
Customer *Store::findCustomer(int customerId) {
  Customer *customer = nullptr;
  if (customers.find(customerId, customer)) {
    return customer;
  }
  return nullptr;
}

// Processes a movie borrow transaction.
bool Store::borrowMovie(int customerId, char mediaType, char movieType,
                        const std::string &movieInfo) {
  if (mediaType != 'D') {
    std::cout << "Invalid media type " << mediaType
              << ", discarding line: " << movieType << " " << movieInfo
              << std::endl;
    return false;
  }

  Customer *customer = findCustomer(customerId);
  if (customer == nullptr) {
    std::cout << "Invalid customer ID " << customerId
              << ", discarding line: " << mediaType << " " << movieType << " "
              << movieInfo << std::endl;
    return false;
  }

  Movie *movie = findMovie(movieType, movieInfo);
  if (movie == nullptr) {
    std::cout << "Invalid movie for customer " << customer->getFullName()
              << ", discarding line: " << movieInfo << std::endl;
    return false;
  }

  if (!movie->borrowMovie()) {
    std::cout << "==========================" << std::endl;
    std::cout << customer->getFullName() << " could NOT borrow "
              << movie->getTitle() << ", out of stock: " << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << "Failed to execute command: Borrow " << customer->getFullName()
              << " " << movie->getTitle() << std::endl;
    return false;
  }

  customer->addTransaction(Transaction::BORROW, movie);
  return true;
}

// Processes a movie return transaction.
bool Store::returnMovie(int customerId, char mediaType, char movieType,
                        const std::string &movieInfo) {
  if (mediaType != 'D') {
    std::cout << "Invalid media type " << mediaType
              << ", discarding line: " << movieType << " " << movieInfo
              << std::endl;
    return false;
  }

  Customer *customer = findCustomer(customerId);
  if (customer == nullptr) {
    std::cout << "Invalid customer ID " << customerId
              << ", discarding line: " << mediaType << " " << movieType << " "
              << movieInfo << std::endl;
    return false;
  }

  Movie *movie = findMovie(movieType, movieInfo);
  if (movie == nullptr) {
    std::cout << "Invalid movie for customer " << customer->getFullName()
              << ", discarding line: " << movieInfo << std::endl;
    return false;
  }

  movie->returnMovie();
  customer->addTransaction(Transaction::RETURN, movie);
  return true;
}

// Displays the current inventory of all movies.
void Store::displayInventory() {
  std::cout << "INVENTORY:" << std::endl;
  for (const auto &movie : movies) {
    std::cout << movie->toString() << std::endl;
  }
  std::cout << std::endl;
}

// Displays the transaction history for a given customer.
void Store::displayCustomerHistory(int customerId) {
  Customer *customer = findCustomer(customerId);
  if (customer == nullptr) {
    std::cerr << "Error: Customer " << customerId << " not found" << std::endl;
    return;
  }
  customer->displayHistory();
}

// Parses movie search criteria from a raw string.
std::string Store::parseMovieSearchCriteria(char /*unused*/,
                                            const std::string &info) {
  std::string result = info;
  trimString(result);
  return result;
}

// Removes leading and trailing whitespace from a string.
void Store::trimString(std::string &str) {
  str.erase(str.begin(),
            std::find_if(str.begin(), str.end(),
                         [](unsigned char ch) { return !std::isspace(ch); }));
  str.erase(std::find_if(str.rbegin(), str.rend(),
                         [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
            str.end());
}

// Splits a string by a delimiter into a vector of strings.
std::vector<std::string> Store::split(const std::string &str, char delimiter) {
  std::vector<std::string> tokens;
  std::stringstream ss(str);
  std::string token;
  while (std::getline(ss, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}