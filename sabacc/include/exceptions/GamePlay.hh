#ifndef __GAMEEXCEPTIONS_HH
#define __GAMEEXCEPTIONS_HH

// Standard library headers
#include <std::string>

// Exception headers
#include "exceptions/base/ExceptionBase.hh"

namespace Exceptions {

    namespace GamePlay {

        class TooFewPlayers : public ExceptionBase {

        public:
            TooFewPlayers(std::std::string);
            TooFewPlayers(std::std::string, std::string);
            TooFewPlayers(std::std::string, std::string, std::string);
            virtual ~TooFewPlayers();

        };  // class TooFewPlayers

        class TooManyPlayers : public ExceptionBase {

        public:
            TooManyPlayers(std::std::string);
            TooManyPlayers(std::std::string, std::string);
            TooManyPlayers(std::std::string, std::string, std::string);
            virtual ~TooManyPlayers();

        };  // class TooManyPlayers

        class CannotHoldMore : public ExceptionBase {

        public:
            CannotHoldMore(std::std::string);
            CannotHoldMore(std::std::string, std::string);
            CannotHoldMore(std::std::string, std::string, std::string);
            virtual ~CannotHoldMore();

        };  // class CannotHoldMore


    } // namespace GamePlay

}   // namespace Exceptions

#endif // __GAMEEXCEPTIONS_HH
