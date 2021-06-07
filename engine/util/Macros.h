//
//  Macros.h
//  MiniRPG
//
//  Created by nuttachai on 2/5/20.
//

/** A header containing collection of useful macros
 */

#ifndef Macros_h
#define Macros_h

#include <memory>

//! Counting size of the given array.
#define ARRAY_COUNT(_array) (sizeof(_array) / sizeof(_array[0]))

//! Converts the given codes into a string (stringtify).
#define STR(_code) #_code
//! Evaluates value of the given macro and turns it into a string.
#define VALUE(_macro) STR(_macro)
//! usage: #pragma message(VAR_NAME_VALUE(macro_name))
#define VAR_NAME_VALUE(_var) #_var " = " VALUE(_var)

//! Declares a unique pointer of class _type.
#define PTR(_type) std::unique_ptr<_type>
//! Declares a share pointer of class _type.
#define SHARE(_type) std::shared_ptr<_type>

#endif /* Macros_h */
