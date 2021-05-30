#pragma once

// Standard library
#include <iostream>
#include <cstdint>
#include <cassert>
#include <stdexcept>
#include <initializer_list>
#include <functional>
#include <glm/gtx/string_cast.hpp>

#include "SizeLiteral.h"
#include "DefaultEquality.h"
#include "DefaultIncrement.h"
#include "SList.h"
#include "Vector.h"
#include "HashMap.h"
#include "RTTI.h"
#include "Datum.h"
#include "Scope.h"

#ifndef UNREFERENCED_LOCAL
#define UNREFERENCED_LOCAL(P) (P)
#endif