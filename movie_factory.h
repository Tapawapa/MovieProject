#ifndef MOVIE_FACTORY_H
#define MOVIE_FACTORY_H

#include <functional>
#include <map>
#include <string>

class Movie;

// Factory for creating movie objects from strings.
class MovieFactory {
public:
  using CreateFunction = std::function<Movie *(
      int, const std::string &, const std::string &, const std::string &)>;

  // Gets the singleton instance of the factory.
  static MovieFactory &getInstance();

  // Registers a movie type with a creation function.
  bool registerMovie(char genre, CreateFunction func);
  // Creates a movie object from data.
  Movie *createMovie(char genre, int stock, const std::string &director,
                     const std::string &title, const std::string &extra);

private:
  std::map<char, CreateFunction> creators;
  MovieFactory() = default;
};

// A simple hash table implementation for customer lookups.
template <typename K, typename V> class HashTable {
private:
  struct Node {
    K key;
    V value;
    Node *next;
    Node(const K &k, const V &v) : key(k), value(v), next(nullptr) {}
  };

  static const int TABLE_SIZE = 101;
  Node *table[TABLE_SIZE];

  // Hashes a key to an index in the table.
  int hash(const K &key) const {
    if constexpr (std::is_same_v<K, int>) {
      return key % TABLE_SIZE;
    } else if constexpr (std::is_same_v<K, std::string>) {
      int hash = 0;
      for (char c : key) {
        hash = (hash * 31 + c) % TABLE_SIZE;
      }
      return hash;
    }
    return 0;
  }

public:
  // Constructs a new HashTable.
  HashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
      table[i] = nullptr;
    }
  }

  // Destroys the HashTable and frees memory.
  ~HashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
      Node *current = table[i];
      while (current) {
        Node *next = current->next;
        delete current;
        current = next;
      }
    }
  }

  // Inserts a key-value pair into the hash table.
  void insert(const K &key, const V &value) {
    int index = hash(key);
    Node *newNode = new Node(key, value);

    if (table[index] == nullptr) {
      table[index] = newNode;
    } else {
      Node *current = table[index];
      while (current->next) {
        if (current->key == key) {
          current->value = value;
          delete newNode;
          return;
        }
        current = current->next;
      }
      if (current->key == key) {
        current->value = value;
        delete newNode;
      } else {
        current->next = newNode;
      }
    }
  }

  // Finds a value by its key.
  bool find(const K &key, V &value) const {
    int index = hash(key);
    Node *current = table[index];

    while (current) {
      if (current->key == key) {
        value = current->value;
        return true;
      }
      current = current->next;
    }
    return false;
  }

  // Checks if a key exists in the hash table.
  bool exists(const K &key) const {
    V dummy;
    return find(key, dummy);
  }
};

#endif // MOVIE_FACTORY_H