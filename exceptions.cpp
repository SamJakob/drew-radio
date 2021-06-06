#include "exceptions.hpp"

const char* DrewRadio::InterfaceNotFoundException::what() const noexcept {
    return "Failed to locate local bluetooth interface or adaptor.";
}

const char* DrewRadio::InterfaceFailureException::what() const noexcept {
    return "Failed to bind or use bluetooth interface or adaptor.";
}

const char* DrewRadio::FileFormatException::what() const noexcept {
    return "Failed to read file because invalid data was identified and/or read.";
}
