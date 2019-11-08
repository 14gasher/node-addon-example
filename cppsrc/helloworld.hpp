// Hello World wrapper
namespace helloworld
{
    double hello(double a, double b);
    Napi::Number HelloWrapped(const Napi::CallbackInfo& info);
    Napi::Object Init(Napi::Env env, Napi::Object exports);
} // namespace helloworld

// Implementations

double helloworld::hello(double a, double b)
{
    return a + b;
}

Napi::Number helloworld::HelloWrapped(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber())
    {
        Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    }
    Napi::Number returnValue = Napi::Number::New(env, helloworld::hello(info[0].As<Napi::Number>().DoubleValue(), info[1].As<Napi::Number>().DoubleValue()));

    return returnValue;
}

Napi::Object helloworld::Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("hello", Napi::Function::New(env, helloworld::HelloWrapped));

    return exports;
}
