#include "movie.h"
#include "movie_factory.h"
#include <iostream>
#include <sstream>

bool Comedy::registered = Comedy::registerSelf();
bool Drama::registered = Drama::registerSelf();
bool Classic::registered = Classic::registerSelf();

// Constructs a Movie object.
Movie::Movie(int stock, const std::string &director, const std::string &title)
    : stock(stock), borrowed(0), director(director), title(title) {}

// Decrements stock and increments borrowed count.
bool Movie::borrowMovie() {
  if (stock > borrowed) {
    borrowed++;
    return true;
  }
  return false;
}

// Increments stock and decrements borrowed count.
bool Movie::returnMovie() {
  if (borrowed > 0) {
    borrowed--;
    return true;
  }
  return false;
}

// Constructs a Comedy movie.
Comedy::Comedy(int stock, const std::string &director, const std::string &title,
               int year)
    : Movie(stock, director, title), year(year) {}

// Compares two Comedy movies for sorting.
bool Comedy::operator<(const Movie &other) const {
  const Comedy *otherComedy = dynamic_cast<const Comedy *>(&other);
  if (otherComedy == nullptr) {
    return getGenre() < other.getGenre();
  }

  if (title != otherComedy->title) {
    return title < otherComedy->title;
  }
  return year < otherComedy->year;
}

// Checks if two Comedy movies are equal.
bool Comedy::operator==(const Movie &other) const {
  const Comedy *otherComedy = dynamic_cast<const Comedy *>(&other);
  if (otherComedy == nullptr) {
    return false;
  }

  return title == otherComedy->title && year == otherComedy->year;
}

// Returns a string representation of a Comedy movie.
std::string Comedy::toString() const {
  return "Comedy: " + title + " (" + std::to_string(year) +
         ") Dir: " + director + " Stock: " + std::to_string(stock - borrowed) +
         " Out: " + std::to_string(borrowed);
}

// Creates a clone of a Comedy movie.
Movie *Comedy::clone() const {
  return new Comedy(stock, director, title, year);
}

// Factory method to create a Comedy movie.
Movie *Comedy::create(int stock, const std::string &director,
                      const std::string &title, const std::string &extra) {
  try {
    int year = std::stoi(extra);
    return new Comedy(stock, director, title, year);
  } catch (...) {
    std::cerr << "Error: Invalid year for comedy: " << extra << std::endl;
    return nullptr;
  }
}

// Registers the Comedy movie type with the factory.
bool Comedy::registerSelf() {
  return MovieFactory::getInstance().registerMovie('F', Comedy::create);
}

// Constructs a Drama movie.
Drama::Drama(int stock, const std::string &director, const std::string &title,
             int year)
    : Movie(stock, director, title), year(year) {}

// Compares two Drama movies for sorting.
bool Drama::operator<(const Movie &other) const {
  const Drama *otherDrama = dynamic_cast<const Drama *>(&other);
  if (otherDrama == nullptr) {
    return getGenre() < other.getGenre();
  }

  if (director != otherDrama->director) {
    return director < otherDrama->director;
  }
  return title < otherDrama->title;
}

// Checks if two Drama movies are equal.
bool Drama::operator==(const Movie &other) const {
  const Drama *otherDrama = dynamic_cast<const Drama *>(&other);
  if (otherDrama == nullptr) {
    return false;
  }

  return director == otherDrama->director && title == otherDrama->title;
}

// Returns a string representation of a Drama movie.
std::string Drama::toString() const {
  return "Drama: " + director + ", " + title + " (" + std::to_string(year) +
         ") Stock: " + std::to_string(stock - borrowed) +
         " Out: " + std::to_string(borrowed);
}

// Creates a clone of a Drama movie.
Movie *Drama::clone() const { return new Drama(stock, director, title, year); }

// Factory method to create a Drama movie.
Movie *Drama::create(int stock, const std::string &director,
                     const std::string &title, const std::string &extra) {
  try {
    int year = std::stoi(extra);
    return new Drama(stock, director, title, year);
  } catch (...) {
    std::cerr << "Error: Invalid year for drama: " << extra << std::endl;
    return nullptr;
  }
}

// Registers the Drama movie type with the factory.
bool Drama::registerSelf() {
  return MovieFactory::getInstance().registerMovie('D', Drama::create);
}

// Constructs a Classic movie.
Classic::Classic(int stock, const std::string &director,
                 const std::string &title, const std::string &actor, int month,
                 int year)
    : Movie(stock, director, title), actor(actor), month(month), year(year) {}

// Compares two Classic movies for sorting.
bool Classic::operator<(const Movie &other) const {
  const Classic *otherClassic = dynamic_cast<const Classic *>(&other);
  if (otherClassic == nullptr) {
    return getGenre() < other.getGenre();
  }

  if (month != otherClassic->month) {
    return month < otherClassic->month;
  }
  if (year != otherClassic->year) {
    return year < otherClassic->year;
  }
  return actor < otherClassic->actor;
}

// Checks if two Classic movies are equal.
bool Classic::operator==(const Movie &other) const {
  const Classic *otherClassic = dynamic_cast<const Classic *>(&other);
  if (otherClassic == nullptr) {
    return false;
  }

  return month == otherClassic->month && year == otherClassic->year &&
         actor == otherClassic->actor;
}

// Returns a string representation of a Classic movie.
std::string Classic::toString() const {
  return "Classic: " + std::to_string(month) + " " + std::to_string(year) +
         " " + actor + " - " + title + " Dir: " + director +
         " Stock: " + std::to_string(stock - borrowed) +
         " Out: " + std::to_string(borrowed);
}

// Creates a clone of a Classic movie.
Movie *Classic::clone() const {
  return new Classic(stock, director, title, actor, month, year);
}

// Factory method to create a Classic movie.
Movie *Classic::create(int stock, const std::string &director,
                       const std::string &title, const std::string &extra) {
  std::string parseStr = extra;

  for (char &c : parseStr) {
    if (c == ',') {
      c = ' ';
    }
  }

  std::istringstream iss(parseStr);
  std::string firstName;
  std::string lastName;
  int month;
  int year;

  if (!(iss >> firstName >> lastName >> month >> year)) {
    std::cerr << "Error: Invalid classic movie format: " << extra << std::endl;
    return nullptr;
  }

  std::string actor = firstName + " " + lastName;
  return new Classic(stock, director, title, actor, month, year);
}

// Registers the Classic movie type with the factory.
bool Classic::registerSelf() {
  return MovieFactory::getInstance().registerMovie('C', Classic::create);
}

// Returns the singleton instance of the MovieFactory.
MovieFactory &MovieFactory::getInstance() {
  static MovieFactory instance;
  return instance;
}

// Registers a new movie type with its creation function.
bool MovieFactory::registerMovie(char genre, CreateFunction func) {
  creators[genre] = func;
  return true;
}

// Creates a movie object based on its genre and data.
Movie *MovieFactory::createMovie(char genre, int stock,
                                 const std::string &director,
                                 const std::string &title,
                                 const std::string &extra) {
  auto it = creators.find(genre);
  if (it != creators.end()) {
    return it->second(stock, director, title, extra);
  }
  return nullptr;
}