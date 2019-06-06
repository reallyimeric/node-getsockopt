#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/netfilter_ipv4.h>

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

    struct sockaddr_in oridestaddr;
    socklen_t socklen = sizeof(oridestaddr);
    getsockopt(socketfd, SOL_IP, SO_ORIGINAL_DST, &oridestaddr, &socklen);
    Napi::String family;
    switch (oridestaddr.sin_family)
    {
    case AF_INET:
        family = Napi::String::New(env, "IPv4");
        break;
    case AF_INET6:
        family = Napi::String::New(env, "IPv6");
        break;
    default:
        family = Napi::String::New(env, "");
        break;
    }
    unsigned short int port = ntohs(oridestaddr.sin_port);
    char addr[256] = {0};
    inet_ntop(AF_INET, &oridestaddr.sin_addr, addr, 256);
    Napi::Object result = Napi::Object::New(env);
    result.Set(Napi::String::New(env, "family"), family);
    result.Set(Napi::String::New(env, "address"), Napi::String::New(env, addr));
    result.Set(Napi::String::New(env, "port"), Napi::Number::New(env, port));
    cb.Call(env.Global(), {env.Null(),
                           result});
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(
        Napi::String::New(env, "getOriginalDst"),
        Napi::Function::New(env, GetOriginalDst));
    return exports;
}

NODE_API_MODULE(getSockOpt, Init)
