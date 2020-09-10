#include <string>
#include <map>

#define SLOT void

class SignalObject
{
private:
    struct Func
    {
        void* ptr;
        uintptr_t func;
    };
    std::map<std::string, Func> _funcs;

public:
    SignalObject() {}
    virtual ~SignalObject() {}

    template <typename... Args>
    void connect(const std::string& name, void (*cb)(Args...))
    {
        uintptr_t func = reinterpret_cast<uintptr_t>(cb);
        _funcs[name] = {nullptr, func};
    }

    template <typename To, typename... Args>
    void connect(const std::string& name, To* obj, void (To::*cb)(Args...))
    {
        typedef void (*fptr)(To*, Args...);
        uintptr_t func = reinterpret_cast<uintptr_t>((fptr)(obj->*cb));
        _funcs[name] = {reinterpret_cast<void*>(obj), func};
    }

    template <typename... Args>
    void emit(const std::string& name, Args... args)
    {
        if (!_funcs.contains(name))
            return;

        Func func = _funcs[name];

        // Static Function
        if (func.ptr == nullptr) {
            void (*cb)(Args...) = reinterpret_cast<void (*)(Args...)>(func.func);
            cb(args...);
        }
        
        // Member Function
        else {
            auto* ptr = reinterpret_cast<SignalObject*>(func.ptr);
            typedef void (*fptr)(decltype(ptr), Args...);
            fptr cb = reinterpret_cast<fptr>(func.func);
            cb(ptr, args...);
        }
    }
};