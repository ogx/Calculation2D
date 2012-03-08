#include <types/mmID.h>

#include <limits>

#undef min
#undef max

mmID const mmID::invalid = mmID(std::numeric_limits<mmUInt>::max());