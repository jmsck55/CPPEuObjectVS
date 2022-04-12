// Copyright (c) 2022 James Cook
//

#pragma once

// Total of 4 Modules:

// Default Modules:

#include "eu/alltypes.hpp"

// Additional Modules:

// Set these macros in "alldefs.h" or on the command-line.

#ifdef USE_STANDARD_LIBRARY
#include "modules/eufuncs.hpp"
#endif
#ifdef USE_MATH_H
#include "modules/eumath.hpp"
#endif
#ifdef USE_QUADMATH_H
#include "modules/euquad.hpp"
#endif

//#include "modules/eugmp.hpp"
//#include "modules/eumpfr.hpp"
