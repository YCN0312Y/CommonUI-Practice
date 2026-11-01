
#pragma once

#define MAKE_SET_GET(Type, Value)\
void Set##Value(const Type& In##Value) { Value = In##Value; }\
const Type& Get##Value()const { return Value; }
