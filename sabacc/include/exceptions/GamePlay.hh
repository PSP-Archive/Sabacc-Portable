#ifndef __GAMEEXCEPTIONS_HH
#define __GAMEEXCEPTIONS_HH

// Standard library headers
#include <string>

// Exception headers
#include "exceptions/base/ExceptionBase.hh"

namespace Exceptions
  {

  namespace GamePlay
    {

    class TooFewPlayers : public ExceptionBase
      {

      public:
        TooFewPlayers(std::string);
        TooFewPlayers(std::string, std::string);
        TooFewPlayers(std::string, std::string, std::string);
        virtual ~TooFewPlayers();

      };  // class TooFewPlayers

    class TooManyPlayers : public ExceptionBase
      {

      public:
        TooManyPlayers(std::string);
        TooManyPlayers(std::string, std::string);
        TooManyPlayers(std::string, std::string, std::string);
        virtual ~TooManyPlayers();

      };  // class TooManyPlayers

    class CannotHoldMore : public ExceptionBase
      {

      public:
        CannotHoldMore(std::string);
        CannotHoldMore(std::string, std::string);
        CannotHoldMore(std::string, std::string, std::string);
        virtual ~CannotHoldMore();

      };  // class CannotHoldMore


  } // namespace GamePlay

}   // namespace Exceptions

#endif // __GAMEEXCEPTIONS_HH
