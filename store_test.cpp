#include "Store.h"

/**
 * Test runner for the movie store.
 * This function initializes the store, loads movie and customer data,
 * and processes a series of commands from files.
 */
void testAll() {
  Store store;
  store.loadMovies("data4movies.txt");
  store.loadCustomers("data4customers.txt");
  store.processCommands("data4commands.txt");
}