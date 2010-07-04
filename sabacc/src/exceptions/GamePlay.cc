// Exception headers
#include "exceptions/base/ExceptionBase.hh"
#include "exceptions/GamePlay.hh"

namespace Exceptions
  {

  namespace GamePlay
    {

    TooFewPlayers::TooFewPlayers(std::string file) : ExceptionBase(file, "Too few players joined the game.") { }
    TooFewPlayers::TooFewPlayers(std::string file, std::string dtld) : ExceptionBase(file, "Too few players joined the game.", dtld) { }
    TooFewPlayers::TooFewPlayers(std::string file, std::string msg, std::string dtld) : ExceptionBase(file, msg, dtld) { }
    TooFewPlayers::~TooFewPlayers() { }

    TooManyPlayers::TooManyPlayers(std::string file) : ExceptionBase(file, "Too many players for a game.") { }
    TooManyPlayers::TooManyPlayers(std::string file, std::string dtld) : ExceptionBase(file, "Too many players for a game.", dtld) { }
    TooManyPlayers::TooManyPlayers(std::string file, std::string msg, std::string dtld) : ExceptionBase(file, msg, dtld) { }
    TooManyPlayers::~TooManyPlayers() { }

    CannotHoldMore::CannotHoldMore(std::string file) : ExceptionBase(file, "Too many players for a game.") { }
    CannotHoldMore::CannotHoldMore(std::string file, std::string dtld) : ExceptionBase(file, "Too many players for a game.", dtld) { }
    CannotHoldMore::CannotHoldMore(std::string file, std::string msg, std::string dtld) : ExceptionBase(file, msg, dtld) { }
    CannotHoldMore::~CannotHoldMore() { }

  }   // namespace GamePlay

}   // namespace Exceptions
