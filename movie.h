#ifndef MOVIE_H
#define MOVIE_H

#include <iostream>
#include <string>

// Abstract base class for all movie types.
class Movie {
public:
  // Constructs a new Movie object.
  Movie(int stock, const std::string &director, const std::string &title);
  virtual ~Movie() = default;

  // Defines the less-than comparison for sorting.
  virtual bool operator<(const Movie &other) const = 0;
  // Defines the equality comparison.
  virtual bool operator==(const Movie &other) const = 0;
  // Returns a string representation of the movie.
  virtual std::string toString() const = 0;
  // Returns the genre character of the movie.
  virtual char getGenre() const = 0;
  // Creates a copy of the movie object.
  virtual Movie *clone() const = 0;

  // Processes a borrow transaction for this movie.
  bool borrowMovie();
  // Processes a return transaction for this movie.
  bool returnMovie();
  // Gets the current stock of the movie.
  int getStock() const { return stock; }
  // Gets the number of borrowed copies.
  int getBorrowed() const { return borrowed; }

  // Gets the director of the movie.
  const std::string &getDirector() const { return director; }
  // Gets the title of the movie.
  const std::string &getTitle() const { return title; }

protected:
  int stock;
  int borrowed;
  std::string director;
  std::string title;
};

// Represents a Comedy movie (genre 'F').
class Comedy : public Movie {
public:
  // Constructs a new Comedy movie.
  Comedy(int stock, const std::string &director, const std::string &title,
         int year);

  // Compares this Comedy movie with another movie for sorting.
  bool operator<(const Movie &other) const override;
  // Checks if this Comedy movie is equal to another movie.
  bool operator==(const Movie &other) const override;
  // Returns a string representation of the Comedy movie.
  std::string toString() const override;
  // Returns the genre character for Comedy movies.
  char getGenre() const override { return 'F'; }
  // Creates a clone of this Comedy movie object.
  Movie *clone() const override;

  // Gets the release year of the comedy.
  int getYear() const { return year; }

  // Factory method to create a Comedy movie from a string.
  static Movie *create(int stock, const std::string &director,
                       const std::string &title, const std::string &extra);
  // Registers the Comedy movie type with the factory.
  static bool registerSelf();

private:
  int year;
  static bool registered;
};

// Represents a Drama movie (genre 'D').
class Drama : public Movie {
public:
  // Constructs a new Drama movie.
  Drama(int stock, const std::string &director, const std::string &title,
        int year);

  // Compares this Drama movie with another movie for sorting.
  bool operator<(const Movie &other) const override;
  // Checks if this Drama movie is equal to another movie.
  bool operator==(const Movie &other) const override;
  // Returns a string representation of the Drama movie.
  std::string toString() const override;
  // Returns the genre character for Drama movies.
  char getGenre() const override { return 'D'; }
  // Creates a clone of this Drama movie object.
  Movie *clone() const override;

  // Gets the release year of the drama.
  int getYear() const { return year; }

  // Factory method to create a Drama movie from a string.
  static Movie *create(int stock, const std::string &director,
                       const std::string &title, const std::string &extra);
  // Registers the Drama movie type with the factory.
  static bool registerSelf();

private:
  int year;
  static bool registered;
};

// Represents a Classic movie (genre 'C').
class Classic : public Movie {
public:
  // Constructs a new Classic movie.
  Classic(int stock, const std::string &director, const std::string &title,
          const std::string &actor, int month, int year);

  // Compares this Classic movie with another movie for sorting.
  bool operator<(const Movie &other) const override;
  // Checks if this Classic movie is equal to another movie.
  bool operator==(const Movie &other) const override;
  // Returns a string representation of the Classic movie.
  std::string toString() const override;
  // Returns the genre character for Classic movies.
  char getGenre() const override { return 'C'; }
  // Creates a clone of this Classic movie object.
  Movie *clone() const override;

  // Gets the major actor of the classic movie.
  const std::string &getActor() const { return actor; }
  // Gets the release month of the classic movie.
  int getMonth() const { return month; }
  // Gets the release year of the classic movie.
  int getYear() const { return year; }

  // Factory method to create a Classic movie from a string.
  static Movie *create(int stock, const std::string &director,
                       const std::string &title, const std::string &extra);
  // Registers the Classic movie type with the factory.
  static bool registerSelf();

private:
  std::string actor;
  int month;
  int year;
  static bool registered;
};

#endif // MOVIE_H