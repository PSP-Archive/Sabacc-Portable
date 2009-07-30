#ifndef __SMF_FREETYPEEXCEPTIONS
#define __SMF_FREETYPEEXCEPTIONS

/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: August 30, 2008
 *
 * File: FreeTypeExceptions.hh
 *
 * FreeType renderer exceptions.
 */

// Standard library headers
#include <string>
using std::string;

// SMF Exception headers
#include "exceptions/base/ExceptionBase.hh"

namespace Exceptions {

    namespace FreeType {

        class GeneralError : public ExceptionBase {

        public:

            /** Constructors. See documentation for ExceptionBase. @{ */
            GeneralError(const string&, const char* = "", int = 0);	/// Short message, file (default: zero length) and line (default: -1)
            GeneralError(const string&, const string&, const char* = "", int = 0);	/// Short message, long message, file (default: zero length) and line (default: -1)
            GeneralError(const string&, const string&, const string&, const char* = "", int = 0);	/// Short message, long message, extra data, file (default: zero length) and line (default: -1)
            virtual ~GeneralError() { }
            /** @} */

            /** These functions get exception information. @{ */
            virtual string getExceptionName();

        };	// class GeneralError

        class FontOpen : public ExceptionBase {

        public:

            /** Constructors. See documentation for ExceptionBase. @{ */
            FontOpen(const string&, const char* = "", int = 0);	/// Short message, file (default: zero length) and line (default: -1)
            FontOpen(const string&, const string&, const char* = "", int = 0);	/// Short message, long message, file (default: zero length) and line (default: -1)
            FontOpen(const string&, const string&, const string&, const char* = "", int = 0);	/// Short message, long message, extra data, file (default: zero length) and line (default: -1)
            virtual ~FontOpen() { }
            /** @} */

            /** These functions get exception information. @{ */
            virtual string getExceptionName();

        };	// class FontOpen

        class Init : public ExceptionBase {

        public:

            /** Constructors. See documentation for ExceptionBase. @{ */
            Init(const string&, const char* = "", int = 0);	/// Short message, file (default: zero length) and line (default: -1)
            Init(const string&, const string&, const char* = "", int = 0);	/// Short message, long message, file (default: zero length) and line (default: -1)
            Init(const string&, const string&, const string&, const char* = "", int = 0);	/// Short message, long message, extra data, file (default: zero length) and line (default: -1)
            virtual ~Init() { }
            /** @} */

            /** These functions get exception information. @{ */
            virtual string getExceptionName();

        };	// class FaceInit

        class NotInitialized : public ExceptionBase {

        public:

            /** Constructors. See documentation for ExceptionBase. @{ */
            NotInitialized(const string&, const char* = "", int = 0);	/// Short message, file (default: zero length) and line (default: -1)
            NotInitialized(const string&, const string&, const char* = "", int = 0);	/// Short message, long message, file (default: zero length) and line (default: -1)
            NotInitialized(const string&, const string&, const string&, const char* = "", int = 0);	/// Short message, long message, extra data, file (default: zero length) and line (default: -1)
            virtual ~NotInitialized() { }
            /** @} */

            /** These functions get exception information. @{ */
            virtual string getExceptionName();

        };	// class NotInitialized

        class FaceInit : public ExceptionBase {

        public:

            /** Constructors. See documentation for ExceptionBase. @{ */
            FaceInit(const string&, const char* = "", int = 0);	/// Short message, file (default: zero length) and line (default: -1)
            FaceInit(const string&, const string&, const char* = "", int = 0);	/// Short message, long message, file (default: zero length) and line (default: -1)
            FaceInit(const string&, const string&, const string&, const char* = "", int = 0);	/// Short message, long message, extra data, file (default: zero length) and line (default: -1)
            virtual ~FaceInit() { }
            /** @} */

            /** These functions get exception information. @{ */
            virtual string getExceptionName();

        };	// class FaceInit

        class UnknownFormat : public ExceptionBase {

        public:

            /** Constructors. See documentation for ExceptionBase. @{ */
            UnknownFormat(const string&, const char* = "", int = 0);	/// Short message, file (default: zero length) and line (default: -1)
            UnknownFormat(const string&, const string&, const char* = "", int = 0);	/// Short message, long message, file (default: zero length) and line (default: -1)
            UnknownFormat(const string&, const string&, const string&, const char* = "", int = 0);	/// Short message, long message, extra data, file (default: zero length) and line (default: -1)
            virtual ~UnknownFormat() { }
            /** @} */

            /** These functions get exception information. @{ */
            virtual string getExceptionName();

        };	// class UnknownFormat

        class InvalidHandle : public ExceptionBase {

        public:

            /** Constructors. See documentation for ExceptionBase. @{ */
            InvalidHandle(const string&, const char* = "", int = 0);	/// Short message, file (default: zero length) and line (default: -1)
            InvalidHandle(const string&, const string&, const char* = "", int = 0);	/// Short message, long message, file (default: zero length) and line (default: -1)
            InvalidHandle(const string&, const string&, const string&, const char* = "", int = 0);	/// Short message, long message, extra data, file (default: zero length) and line (default: -1)
            virtual ~InvalidHandle() { }
            /** @} */

            /** These functions get exception information. @{ */
            virtual string getExceptionName();

        };	// class InvalidHandle

        class LoadGlyph : public ExceptionBase {

        public:

            /** Constructors. See documentation for ExceptionBase. @{ */
            LoadGlyph(const string&, const char* = "", int = 0);	/// Short message, file (default: zero length) and line (default: -1)
            LoadGlyph(const string&, const string&, const char* = "", int = 0);	/// Short message, long message, file (default: zero length) and line (default: -1)
            LoadGlyph(const string&, const string&, const string&, const char* = "", int = 0);	/// Short message, long message, extra data, file (default: zero length) and line (default: -1)
            virtual ~LoadGlyph() { }
            /** @} */

            /** These functions get exception information. @{ */
            virtual string getExceptionName();

        };	// class LoadGlyph

        class InsufficientMemory : public ExceptionBase {

        public:

            /** Constructors. See documentation for ExceptionBase. @{ */
            InsufficientMemory(const string&, const char* = "", int = 0);	/// Short message, file (default: zero length) and line (default: -1)
            InsufficientMemory(const string&, const string&, const char* = "", int = 0);	/// Short message, long message, file (default: zero length) and line (default: -1)
            InsufficientMemory(const string&, const string&, const string&, const char* = "", int = 0);	/// Short message, long message, extra data, file (default: zero length) and line (default: -1)
            virtual ~InsufficientMemory() { }
            /** @} */

            /** These functions get exception information. @{ */
            virtual string getExceptionName();

        };	// class InsufficientMemory

        class Render : public ExceptionBase {

        public:

            /** Constructors. See documentation for ExceptionBase. @{ */
            Render(const string&, const char* = "", int = 0);	/// Short message, file (default: zero length) and line (default: -1)
            Render(const string&, const string&, const char* = "", int = 0);	/// Short message, long message, file (default: zero length) and line (default: -1)
            Render(const string&, const string&, const string&, const char* = "", int = 0);	/// Short message, long message, extra data, file (default: zero length) and line (default: -1)
            virtual ~Render() { }
            /** @} */

            /** These functions get exception information. @{ */
            virtual string getExceptionName();

        };	// class Render

    }	// namespace FreeType

}	// namespace Exceptions

#endif // __SMF_FREETYPEEXCEPTIONS
