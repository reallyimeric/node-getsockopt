// #include "napi.h"

// Napi::Value GetOriginalDst(const Napi::CallbackInfo& info)
// {
//     Napi::Env env = info.Env();
//     int socketfd = info[0]
//                        .As<Napi::Object>()
//                        .Get("_handle")
//                        .As<Napi::Object>()
//                        .Get("fd")
//                        .As<Napi::Number>()
//                        .Int32Value();
//     return Napi::Number::New(env, socketfd);
// }

// Napi::Object Init(Napi::Env env, Napi::Object exports)
// {
//     exports.Set(
//         Napi::String::New(env, "getOriginalDst"),
//         Napi::Function::New(env, GetOriginalDst));
//     return exports;
// }

// NODE_API_MODULE(getSockOpt, Init)

#include <napi.h>

void GetOriginalDst(const Napi::CallbackInfo &info)
{
    Napi::Function cb = info[1].As<Napi::Function>();
    Napi::Env env = info.Env();
    if (!info[0].IsObject())
    {
        // Napi::TypeError::New(env, Napi::String::New(env, "socket is not a object"))
        cb.Call(env.Global(), {Napi::String::New(env, "socket is not an object")});
        return;
    }
    Napi::Object socket = info[0].As<Napi::Object>();
    if (!socket.Get("_handle").IsObject())
    {
        cb.Call(env.Global(), {Napi::String::New(env, "socket._handle is not an object")});
        return;
    }
    Napi::Object handle = socket.Get("_handle").As<Napi::Object>();
    if (!handle.Get("fd").IsNumber())
    {
        cb.Call(env.Global(), {Napi::String::New(env, "socket._handle.fd is not a number")});
        return;
    }
    Napi::Number fd = handle.Get("fd").As<Napi::Number>();
    int socketfd = fd.Int32Value();
    cb.Call(env.Global(), {env.Null(),
                           Napi::Number::New(env, socketfd)});
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(
        Napi::String::New(env, "getOriginalDst"),
        Napi::Function::New(env, GetOriginalDst));
    return exports;
}

NODE_API_MODULE(getSockOpt, Init)
