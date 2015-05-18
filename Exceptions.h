#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

struct FileNotFoundException : public std::exception { const char* what() const throw() { return "FileNotFound"; } };
struct InvalidAddressRangeFile : public std::exception { const char* what() const throw() { return "InvalidAddressRangeFile"; }};
struct NoRangeAvailable : public std::exception { const char* what() const throw() { return "NoRangeAvailable"; } };
struct InvalidRangeIncrement : public std::exception { const char* what() const throw() { return "InvalidRangeIncrementmak"; } };

#endif