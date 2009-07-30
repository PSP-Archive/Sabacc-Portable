// Standard liobrary headers


// Exception headers
#include "exceptions/base/ExceptionBase.hh"
#include "exceptions/GameImpl.hh"

namespace Exceptions {

    namespace Game {

        Game::Game(std::string file) : ExceptionBase(file, "Generic game implementation exception.") { }
        Game::Game(std::string file, std::string dtld) : ExceptionBase(file, "Game implementation execption.", dtld) { }
        Game::Game(std::string file, std::string msg, std::string dtld) : ExceptionBase(file, msg, dtld) { }
        Game::~Game() { }

        CouldNotCreateGameObject::CouldNotCreateGameObject(std::string file) : ExceptionBase(file, "Generic game implementation exception.") { }
        CouldNotCreateGameObject::CouldNotCreateGameObject(std::string file, std::string dtld) : ExceptionBase(file, "CouldNotCreateGameObject implementation execption.", dtld) { }
        CouldNotCreateGameObject::CouldNotCreateGameObject(std::string file, std::string msg, std::string dtld) : ExceptionBase(file, msg, dtld) { }
        CouldNotCreateGameObject::~CouldNotCreateGameObject() { }

    }   // namespace GamePlay

}   // namespace Exceptions
