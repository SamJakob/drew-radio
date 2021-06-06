#include <exception>

#ifndef DR_EXCEPTIONS
#define DR_EXCEPTIONS

namespace DrewRadio {

    /**
     * Thrown when the bluetooth interface or adaptor on the local device cannot be found.
     * Typically expected to be thrown if the device does not have the adaptor, or if it
     * is disabled, or faulty.
     */
    class InterfaceNotFoundException : public std::exception {
        public: virtual const char* what() const noexcept;
    };

    /**
     * Thrown when an interaction with the bluetooth interface unexpectedly fails, e.g. due
     * to the interface being unresponsive or when a handle with the interface cannot be
     * obtained.
     */
    class InterfaceFailureException : public std::exception {
        public: virtual const char* what() const noexcept;
    };

    /**
     * Thrown when an invalid character or character sequence was identified whilst reading
     * the program data from the file.
     * Typically expected to be thrown, for example, if a magic number was incorrect because
     * the file was corrupted or saved in the wrong format.
     */
    class FileFormatException : public std::exception {
        public: virtual const char* what() const noexcept;
    };


}


#endif
