/** @file
 * @brief Cast a pointer we know is suitably aligned
 */
/* Copyright (C) 2016 Olly Betts
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 */
#ifndef XAPIAN_INCLUDED_ALIGNMENT_CAST_H
#define XAPIAN_INCLUDED_ALIGNMENT_CAST_H

#include <type_traits>

/** Cast a pointer we know is suitably aligned.
 *
 *  Has the same effect as reinterpret_cast<T> but avoids warnings about
 *  alignment issues.
 *
 *  Version for const pointers.
 */
template<typename T, typename U>
typename std::enable_if<std::is_const<typename std::remove_pointer<U>::type>::value, T>::type
alignment_cast(U ptr)
{
    return static_cast<T>(static_cast<const void*>(ptr));
}

/** Cast a pointer we know is suitably aligned.
 *
 *  Has the same effect as reinterpret_cast<T> but avoids warnings about
 *  alignment issues.
 *
 *  Version for non-const pointers.
 */
template<typename T, typename U>
typename std::enable_if<!std::is_const<typename std::remove_pointer<U>::type>::value, T>::type
alignment_cast(U ptr)
{
    return static_cast<T>(static_cast<void*>(ptr));
}

#endif /* XAPIAN_INCLUDED_ALIGNMENT_CAST_H */
