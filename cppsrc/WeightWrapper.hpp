#include "weight.hpp"

#include <cstdint>
#include <napi.h>
#include <string>

template <class R, class S = std::uint64_t>
class WeightWrapper : public Napi::ObjectWrap<WeightWrapper<R, S>>
{
  public:
    WeightWrapper(const Napi::CallbackInfo& info, const bool shouldUseInfo = true) :
        Napi::ObjectWrap<WeightWrapper<R, S>>(info)
    {
        auto env = info.Env();
        if (info.Length() == 0 || !shouldUseInfo)
        {
            actualWeight = new usu::weight<R, S>();
        }
        else if (info[0].IsNumber())
        {
            auto count = info[0].As<Napi::Number>().DoubleValue();
            actualWeight = new usu::weight<R, S>(count);
        }
        else
        {
            Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
        }
    };
    static Napi::Object Init(Napi::Env env, Napi::Object exports, std::string keyname)
    {
        Napi::HandleScope scope(env);
        WeightWrapper<R, S>::name = keyname;
        std::string className = keyname + "|Weight";
        Napi::Function func = Napi::ObjectWrap<WeightWrapper<R, S>>::DefineClass(
            env, className.c_str(), {Napi::ObjectWrap<WeightWrapper<R, S>>::InstanceMethod("add", &WeightWrapper<R, S>::add), Napi::ObjectWrap<WeightWrapper<R, S>>::InstanceMethod("subtract", &WeightWrapper<R, S>::subtract), Napi::ObjectWrap<WeightWrapper<R, S>>::InstanceAccessor("count", &WeightWrapper<R, S>::count, nullptr, napi_enumerable), Napi::ObjectWrap<WeightWrapper<R, S>>::InstanceAccessor("name", &WeightWrapper<R, S>::getName, nullptr, napi_enumerable)});
        constructor = Napi::Persistent(func);
        constructor.SuppressDestruct();
        exports.Set(keyname, func);
        return exports;
    };

  private:
    static Napi::FunctionReference constructor; // Reference to store the constructor for JS
    static std::string name;                    // Reference to store the name for JS

    Napi::Value count(const Napi::CallbackInfo& info)
    {
        auto env = info.Env();
        Napi::HandleScope scope(env);
        return Napi::Number::New(env, actualWeight->count());
    };
    Napi::Value getName(const Napi::CallbackInfo& info)
    {
        auto env = info.Env();
        Napi::HandleScope scope(env);
        return Napi::String::New(env, WeightWrapper<R, S>::name);
    }
    Napi::Value add(const Napi::CallbackInfo& info)
    {
        auto env = info.Env();
        Napi::HandleScope scope(env);
        auto toAddWrapped = info[0].As<Napi::Object>();
        auto toAdd = Napi::ObjectWrap<WeightWrapper<R, S>>::Unwrap(toAddWrapped);
        auto weightToAdd = *(toAdd->actualWeight) + *(this->actualWeight);
        return WeightWrapper::constructor.New({Napi::Number::New(env, weightToAdd.count())});
    }

    Napi::Value subtract(const Napi::CallbackInfo& info)
    {
        auto env = info.Env();
        Napi::HandleScope scope(env);
        auto toAddWrapped = info[0].As<Napi::Object>();
        auto toAdd = Napi::ObjectWrap<WeightWrapper<R, S>>::Unwrap(toAddWrapped);
        auto weightToAdd = *(toAdd->actualWeight) - *(this->actualWeight);
        return WeightWrapper::constructor.New({Napi::Number::New(env, weightToAdd.count())});
    }

    usu::weight<R, S>* actualWeight;
};

template <class R, class S>
Napi::FunctionReference WeightWrapper<R, S>::constructor;

template <class R, class S>
std::string WeightWrapper<R, S>::name = "Weight";
