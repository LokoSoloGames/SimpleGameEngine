#include "Error.h"

namespace SimpleGameEngine {

Error::Error(const SrcLoc& loc, StrView msg) 
: _loc(loc), _msg(msg)
{} 

} // namespace
