#ifndef GAMEIMPL_HH_INCLUDED
#define GAMEIMPL_HH_INCLUDED

// Standard library headers
#include <string>

// Exception headers
#include "exceptions/base/ExceptionBase.hh"

namespace Exceptions
  {

  namespace Game
    {

    class Game : public ExceptionBase
      {

      public:
        Game(std::string);
        Game(std::string, std::string);
        Game(std::string, std::string, std::string);
        virtual ~Game();

      };  // class Game

    class CouldNotCreateGameObject : public ExceptionBase
      {

      public:
        CouldNotCreateGameObject(std::string);
        CouldNotCreateGameObject(std::string, std::string);
        CouldNotCreateGameObject(std::string, std::string, std::string);
        virtual ~CouldNotCreateGameObject();

      };  // class CouldNotCreateGameObject

  } // namespace GamePlay

}   // namespace Exceptions

#endif // GAMEIMPL_HH_INCLUDED
