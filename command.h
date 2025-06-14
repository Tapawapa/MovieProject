#ifndef COMMAND_H
#define COMMAND_H

#include <functional>
#include <map>
#include <sstream>
#include <string>

class Store;

// Abstract base class for all command types.
class Command {
public:
  virtual ~Command() = default;
  // Executes the command on the given store.
  virtual bool execute(Store &store) = 0;
  // Returns a string representation of the command.
  virtual std::string toString() const = 0;
};

// Command to handle borrowing a movie.
class BorrowCommand : public Command {
public:
  // Constructs a BorrowCommand.
  BorrowCommand(int customerId, char mediaType, char movieType,
                const std::string &movieInfo);

  // Executes the borrow command.
  bool execute(Store &store) override;
  // Returns a string representation of the borrow command.
  std::string toString() const override;

  // Creates a BorrowCommand from a command line string.
  static Command *create(const std::string &line);
  // Registers this command type with the factory.
  static bool registerSelf();

private:
  int customerId;
  char mediaType;
  char movieType;
  std::string movieInfo;
  static bool registered;
};

// Command to handle returning a movie.
class ReturnCommand : public Command {
public:
  // Constructs a ReturnCommand.
  ReturnCommand(int customerId, char mediaType, char movieType,
                const std::string &movieInfo);

  // Executes the return command.
  bool execute(Store &store) override;
  // Returns a string representation of the return command.
  std::string toString() const override;

  // Creates a ReturnCommand from a command line string.
  static Command *create(const std::string &line);
  // Registers this command type with the factory.
  static bool registerSelf();

private:
  int customerId;
  char mediaType;
  char movieType;
  std::string movieInfo;
  static bool registered;
};

// Command to display the store's inventory.
class InventoryCommand : public Command {
public:
  InventoryCommand() = default;

  // Executes the inventory display command.
  bool execute(Store &store) override;
  // Returns a string representation of the inventory command.
  std::string toString() const override;

  // Creates an InventoryCommand from a command line string.
  static Command *create(const std::string &line);
  // Registers this command type with the factory.
  static bool registerSelf();

private:
  static bool registered;
};

// Command to display a customer's history.
class HistoryCommand : public Command {
public:
  // Constructs a HistoryCommand.
  explicit HistoryCommand(int customerId);

  // Executes the history display command.
  bool execute(Store &store) override;
  // Returns a string representation of the history command.
  std::string toString() const override;

  // Creates a HistoryCommand from a command line string.
  static Command *create(const std::string &line);
  // Registers this command type with the factory.
  static bool registerSelf();

private:
  int customerId;
  static bool registered;
};

// Factory for creating command objects from strings.
class CommandFactory {
public:
  using CreateFunction = std::function<Command *(const std::string &)>;

  // Gets the singleton instance of the factory.
  static CommandFactory &getInstance();

  // Registers a command type with a creation function.
  bool registerCommand(char cmdType, CreateFunction func);
  // Creates a command object from a command line string.
  Command *createCommand(const std::string &line);

private:
  std::map<char, CreateFunction> creators;
  CommandFactory() = default;
};

#endif // COMMAND_H