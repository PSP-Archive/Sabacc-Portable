/**
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 28, 2008
 *
 * File: FileExceptions.cc
 *
 * File IO related exceptions.
 */

// Standard library headers
#include <string>
using std::string;

// SMF Exception headers
#include "exceptions/base/ExceptionBase.hh"
#include "exceptions/font/FreeTypeExceptions.hh"

namespace Exceptions
  {

  namespace FreeType
    {

    GeneralError::GeneralError(const string& message, const char* file, int line) :
        ExceptionBase(message, file, line) { }
    GeneralError::GeneralError(const string& message, const string& details, const char* file, int line)  :
        ExceptionBase(message, details, file, line) { }
    GeneralError::GeneralError(const string& message, const string& details, const string& data, const char* file, int line)  :
        ExceptionBase(message, details, data, file, line) { }

    string GeneralError::getExceptionName()
    {
      return("FreeType::GeneralError");
    }

    FontOpen::FontOpen(const string& message, const char* file, int line) :
        ExceptionBase(message, file, line) { }
    FontOpen::FontOpen(const string& message, const string& details, const char* file, int line)  :
        ExceptionBase(message, details, file, line) { }
    FontOpen::FontOpen(const string& message, const string& details, const string& data, const char* file, int line)  :
        ExceptionBase(message, details, data, file, line) { }

    string FontOpen::getExceptionName()
    {
      return("FreeType::FontOpen");
    }

    Init::Init(const string& message, const char* file, int line) :
        ExceptionBase(message, file, line) { }
    Init::Init(const string& message, const string& details, const char* file, int line)  :
        ExceptionBase(message, details, file, line) { }
    Init::Init(const string& message, const string& details, const string& data, const char* file, int line)  :
        ExceptionBase(message, details, data, file, line) { }

    string Init::getExceptionName()
    {
      return("FreeType::Init");
    }

    NotInitialized::NotInitialized(const string& message, const char* file, int line) :
        ExceptionBase(message, file, line) { }
    NotInitialized::NotInitialized(const string& message, const string& details, const char* file, int line)  :
        ExceptionBase(message, details, file, line) { }
    NotInitialized::NotInitialized(const string& message, const string& details, const string& data, const char* file, int line)  :
        ExceptionBase(message, details, data, file, line) { }

    string NotInitialized::getExceptionName()
    {
      return("File::NotInitialized");
    }

    FaceInit::FaceInit(const string& message, const char* file, int line) :
        ExceptionBase(message, file, line) { }
    FaceInit::FaceInit(const string& message, const string& details, const char* file, int line)  :
        ExceptionBase(message, details, file, line) { }
    FaceInit::FaceInit(const string& message, const string& details, const string& data, const char* file, int line)  :
        ExceptionBase(message, details, data, file, line) { }

    string FaceInit::getExceptionName()
    {
      return("File::FaceInit");
    }

    UnknownFormat::UnknownFormat(const string& message, const char* file, int line) :
        ExceptionBase(message, file, line) { }
    UnknownFormat::UnknownFormat(const string& message, const string& details, const char* file, int line)  :
        ExceptionBase(message, details, file, line) { }
    UnknownFormat::UnknownFormat(const string& message, const string& details, const string& data, const char* file, int line)  :
        ExceptionBase(message, details, data, file, line) { }

    string UnknownFormat::getExceptionName()
    {
      return("File::UnknownFormat");
    }

    InvalidHandle::InvalidHandle(const string& message, const char* file, int line) :
        ExceptionBase(message, file, line) { }
    InvalidHandle::InvalidHandle(const string& message, const string& details, const char* file, int line)  :
        ExceptionBase(message, details, file, line) { }
    InvalidHandle::InvalidHandle(const string& message, const string& details, const string& data, const char* file, int line)  :
        ExceptionBase(message, details, data, file, line) { }

    string InvalidHandle::getExceptionName()
    {
      return("File::InvalidHandle");
    }

    LoadGlyph::LoadGlyph(const string& message, const char* file, int line) :
        ExceptionBase(message, file, line) { }
    LoadGlyph::LoadGlyph(const string& message, const string& details, const char* file, int line)  :
        ExceptionBase(message, details, file, line) { }
    LoadGlyph::LoadGlyph(const string& message, const string& details, const string& data, const char* file, int line)  :
        ExceptionBase(message, details, data, file, line) { }

    string LoadGlyph::getExceptionName()
    {
      return("File::LoadGlyph");
    }

    InsufficientMemory::InsufficientMemory(const string& message, const char* file, int line) :
        ExceptionBase(message, file, line) { }
    InsufficientMemory::InsufficientMemory(const string& message, const string& details, const char* file, int line)  :
        ExceptionBase(message, details, file, line) { }
    InsufficientMemory::InsufficientMemory(const string& message, const string& details, const string& data, const char* file, int line)  :
        ExceptionBase(message, details, data, file, line) { }

    string InsufficientMemory::getExceptionName()
    {
      return("File::InsufficientMemory");
    }

    Render::Render(const string& message, const char* file, int line) :
        ExceptionBase(message, file, line) { }
    Render::Render(const string& message, const string& details, const char* file, int line)  :
        ExceptionBase(message, details, file, line) { }
    Render::Render(const string& message, const string& details, const string& data, const char* file, int line)  :
        ExceptionBase(message, details, data, file, line) { }

    string Render::getExceptionName()
    {
      return("File::Render");
    }

  }	// namespace LoadError

}	// namespace Exceptions
