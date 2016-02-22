#ifndef BASE_TYPES_H_
#define BASE_TYPES_H_

//--------------------------------------------------
//
// Type definitions and aliases
//
//--------------------------------------------------

#include <cstdint>
#include <boost/smart_ptr.hpp>

// Typedefs and aliases
typedef std::size_t size_t;

typedef unsigned char byte_t;

template<typename T>
using ScopedPtr = boost::scoped_ptr<T>;

template<typename T>
using SharedPtr = boost::shared_ptr<T>;

template<typename T>
using WeakPtr = boost::weak_ptr<T>;

#endif