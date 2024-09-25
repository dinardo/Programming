// ################################
// # Variable template for traits #
// ################################

#include <concepts>

template<typename T>
struct isPointer
{
    inline static const bool value{true};
};

template<typename T>
struct isPointer<T*>
{
    inline static const bool value{true};
};

template<>
struct isPointer<void*>
{
    inline static const bool value{false};
};

template<typename T>
concept IsObject = requires
{
    std::is_object_v<std::remove_reference_t<T>>;
};

template<typename T>
constexpr bool isDereferenceable
{
    requires(T t) {{ *t } -> IsObject;}
};

int main()
{
    static_assert(isPointer<int*>::value,"Int");
    //static_assert(isPointer<void*>::value,"Void");
    static_assert(isDereferenceable<int*>,"Int");
}
