#include <napi.h>
#include <string>
#include <iostream>
#include <ratio>

#include "WeightWrapper.hpp"
#include "helloworld.hpp"

// Main Export Function
Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
    WeightWrapper<std::micro, double>::Init(env, exports, "microgram");
    return helloworld::Init(env, exports);
}
NODE_API_MODULE(addon, InitAll)
