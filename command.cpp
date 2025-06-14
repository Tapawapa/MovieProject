#include "command.h"
#include "Store.h"
#include <iostream>
#include <sstream>

bool BorrowCommand::registered = BorrowCommand::registerSelf();
bool ReturnCommand::registered = ReturnCommand::registerSelf();
bool InventoryCommand::registered = InventoryCommand::registerSelf();
bool HistoryCommand::registered = HistoryCommand::registerSelf();

// Constructs a new BorrowCommand.
BorrowCommand::BorrowCommand(int customerId, char mediaType, char movieType,
                             const std::string &movieInfo)
    : customerId(customerId), mediaType(mediaType), movieType(movieType),
      movieInfo(movieInfo) {}

// Executes the borrow action in the store.
bool BorrowCommand::execute(Store &store) {
  return store.borrowMovie(customerId, mediaType, movieType, movieInfo);
}

// Provides a string representation of the BorrowCommand.
std::string BorrowCommand::toString() const {
  return "Borrow: Customer " + std::to_string(customerId) + " borrows " +
         movieInfo;
}

// Factory method to create a BorrowCommand from a line of text.
Command *BorrowCommand::create(const std::string &line) {
  std::istringstream iss(line);
  char cmd;
  int customerId;
  char mediaType;
  char movieType;

  if (!(iss >> cmd >> customerId >> mediaType >> movieType)) {
    return nullptr;
  }

  std::string movieInfo;
  std::getline(iss, movieInfo);
  if (!movieInfo.empty() && movieInfo[0] == ' ') {
    movieInfo = movieInfo.substr(1);
  }

  return new BorrowCommand(customerId, mediaType, movieType, movieInfo);
}

// Registers the BorrowCommand with the CommandFactory.
bool BorrowCommand::registerSelf() {
  return CommandFactory::getInstance().registerCommand('B',
                                                       BorrowCommand::create);
}

// Constructs a new ReturnCommand.
ReturnCommand::ReturnCommand(int customerId, char mediaType, char movieType,
                             const std::string &movieInfo)
    : customerId(customerId), mediaType(mediaType), movieType(movieType),
      movieInfo(movieInfo) {}

// Executes the return action in the store.
bool ReturnCommand::execute(Store &store) {
  return store.returnMovie(customerId, mediaType, movieType, movieInfo);
}

// Provides a string representation of the ReturnCommand.
std::string ReturnCommand::toString() const {
  return "Return: Customer " + std::to_string(customerId) + " returns " +
         movieInfo;
}

// Factory method to create a ReturnCommand from a line of text.
Command *ReturnCommand::create(const std::string &line) {
  std::istringstream iss(line);
  char cmd;
  int customerId;
  char mediaType;
  char movieType;

  if (!(iss >> cmd >> customerId >> mediaType >> movieType)) {
    return nullptr;
  }

  std::string movieInfo;
  std::getline(iss, movieInfo);
  if (!movieInfo.empty() && movieInfo[0] == ' ') {
    movieInfo = movieInfo.substr(1);
  }

  return new ReturnCommand(customerId, mediaType, movieType, movieInfo);
}

// Registers the ReturnCommand with the CommandFactory.
bool ReturnCommand::registerSelf() {
  return CommandFactory::getInstance().registerCommand('R',
                                                       ReturnCommand::create);
}

// Executes the inventory display action in the store.
bool InventoryCommand::execute(Store &store) {
  store.displayInventory();
  return true;
}

// Provides a string representation of the InventoryCommand.
std::string InventoryCommand::toString() const { return "Display Inventory"; }

// Factory method to create a new InventoryCommand.
Command *InventoryCommand::create(const std::string & /*unused*/) {
  return new InventoryCommand();
}

// Registers the InventoryCommand with the CommandFactory.
bool InventoryCommand::registerSelf() {
  return CommandFactory::getInstance().registerCommand(
      'I', InventoryCommand::create);
}

// Constructs a new HistoryCommand.
HistoryCommand::HistoryCommand(int customerId) : customerId(customerId) {}

// Executes the customer history display action in the store.
bool HistoryCommand::execute(Store &store) {
  store.displayCustomerHistory(customerId);
  return true;
}

// Provides a string representation of the HistoryCommand.
std::string HistoryCommand::toString() const {
  return "Display History for Customer " + std::to_string(customerId);
}

// Factory method to create a HistoryCommand from a line of text.
Command *HistoryCommand::create(const std::string &line) {
  std::istringstream iss(line);
  char cmd;
  int customerId;

  if (!(iss >> cmd >> customerId)) {
    return nullptr;
  }

  return new HistoryCommand(customerId);
}

// Registers the HistoryCommand with the CommandFactory.
bool HistoryCommand::registerSelf() {
  return CommandFactory::getInstance().registerCommand('H',
                                                       HistoryCommand::create);
}

// Returns the singleton instance of the CommandFactory.
CommandFactory &CommandFactory::getInstance() {
  static CommandFactory instance;
  return instance;
}

// Registers a new command type with its creation function.
bool CommandFactory::registerCommand(char cmdType, CreateFunction func) {
  creators[cmdType] = func;
  return true;
}

// Creates a command object based on a line of text.
Command *CommandFactory::createCommand(const std::string &line) {
  if (line.empty()) {
    return nullptr;
  }

  char cmdType = line[0];
  auto it = creators.find(cmdType);

  if (it != creators.end()) {
    Command *cmd = it->second(line);
    if (cmd == nullptr) {
    }
    return cmd;
  }

  std::cout << "Unknown command type: " << cmdType
            << ", discarding line: " << line << std::endl;
  return nullptr;
}