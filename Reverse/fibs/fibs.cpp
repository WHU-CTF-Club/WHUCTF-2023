#include <coroutine>
#include <optional>
 
#include <unistd.h>

template<std::movable T>
class Generator
{
public:
    struct promise_type
    {
        Generator<T> get_return_object()
        {
            return Generator{Handle::from_promise(*this)};
        }
        static std::suspend_always initial_suspend() noexcept
        {
            return {};
        }
        static std::suspend_always final_suspend() noexcept
        {
            return {};
        }
        std::suspend_always yield_value(T value) noexcept
        {
            current_value = std::move(value);
            return {};
        }
        // Disallow co_await in generator coroutines.
        void await_transform() = delete;
        [[noreturn]]
        static void unhandled_exception() { throw; }
 
        std::optional<T> current_value;
    };
 
    using Handle = std::coroutine_handle<promise_type>;
 
    explicit Generator(const Handle coroutine) :
        m_coroutine{coroutine}
    {}
 
    Generator() = default;
    ~Generator()
    {
        if (m_coroutine)
            m_coroutine.destroy();
    }
 
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;
 
    Generator(Generator&& other) noexcept :
        m_coroutine{other.m_coroutine}
    {
        other.m_coroutine = {};
    }
    Generator& operator=(Generator&& other) noexcept
    {
        if (this != &other)
        {
            if (m_coroutine)
                m_coroutine.destroy();
            m_coroutine = other.m_coroutine;
            other.m_coroutine = {};
        }
        return *this;
    }
 
    // Range-based for loop support.
    class Iter
    {
    public:
        void operator++()
        {
            m_coroutine.resume();
        }
        const T& operator*() const
        {
            return *m_coroutine.promise().current_value;
        }
        bool operator==(std::default_sentinel_t) const
        {
            return !m_coroutine || m_coroutine.done();
        }
 
        explicit Iter(const Handle coroutine) :
            m_coroutine{coroutine}
        {}
 
    private:
        Handle m_coroutine;
    };
 
    Iter begin()
    {
        if (m_coroutine)
            m_coroutine.resume();
        return Iter{m_coroutine};
    }
 
    std::default_sentinel_t end() { return {}; }
 
private:
    Handle m_coroutine;
};
 
template<std::integral T>
Generator<T> range(T first, const T last)
{
    while (first < last)
        co_yield first++;
}

template<std::integral T>
Generator<T> magic(T index, T mod)
{
    if (index == 0)
        co_yield 0;
    else if(index == 1)
    {
        co_yield 0;
        co_yield 1;
    }
    else
    {
        co_yield 0;
        co_yield 1;
        T previous = 1;
        T current = 1;
        while (index > 0)
        {
            T tmp = (previous + current) % mod;
            previous = current;
            current = tmp;
            --index;
            co_yield current;
        }
    }
}

size_t get(size_t x)
{
    size_t value;
    constexpr size_t MOD = 0xFFFFFFFF;
    for (const auto i : magic(x - 2, MOD))
        value = i;
    return value;
}

template<size_t N>
void print(const char(&arr)[N])
{
    write(STDOUT_FILENO, arr, N - 1);
}

int main()
{
    print("Hold on, I'm calculating the flag for you...\n");

    static size_t idxs[32] = { 0x0f705d0a8f3f4f5f, 0x0af4f65c7f0274ec, 0x03a1dd096b65a01a, 0x5a8528422f6d09b2, 0x435bf69613c3dbc8, 0x35094fc0a11a1222, 0x46164fb35afaabab, 0x9b286f15feb95e22, 0xe52d79256cf22943, 0xf66797457072cbc1, 0x07f5b564860a6721, 0x55bc75ae208f9f46, 0x3d12fabac77ac707, 0x653fe0ac0fa48f0e, 0x8bbe5efc003b4bb4, 0xbd3e420b2cddfbc3, 0x517099a5473cfcad, 0x5497dee39a94724a, 0xb93ccb3d9eb2b076, 0x352b2ada9679a90d, 0x9b80784b9fddba0a, 0x7505573a5a07f82f, 0xa9fe0c4d81f6500c, 0x15a395d8224ea931, 0x485a0c6d43148670, 0xf733fd07c3a23ea5, 0x24ff13037a00bf2a, 0xfdd4207650095fb0, 0x368df4a095720ab6, 0x533e9586eaef1285, 0x87ef103dac5f8d3c, 0x00c3912b47abce9f };
    static size_t divs[32] = { 0x0000000001930adc, 0x00000000009194fa, 0x0000000004981de3, 0x00000000005353d3, 0x0000000000b47401, 0x00000000005a20a7, 0x000000000001222c, 0x00000000000b287e, 0x0000000000b6fe6d, 0x0000000001e7ff68, 0x00000000020e19f1, 0x00000000024e45eb, 0x0000000002687c57, 0x0000000002f36730, 0x0000000001f14c89, 0x00000000023230f1, 0x0000000000eeb5e5, 0x0000000000c524cd, 0x0000000002bd88b1, 0x0000000001821d58, 0x00000000002b65ed, 0x0000000001b9ad8d, 0x0000000000eba098, 0x000000000187f94f, 0x0000000001c752e3, 0x0000000000de5da0, 0x00000000026ff365, 0x0000000000a3e3da, 0x0000000000116e71, 0x000000000165dce7, 0x0000000000a3dbec, 0x00000000067a6994 };
    static size_t subs[32] = { 0x000000000000002b, 0x0000000000000045, 0x0000000000000025, 0x0000000000000009, 0x0000000000000063, 0x0000000000000056, 0x0000000000000050, 0x000000000000004f, 0x0000000000000055, 0x000000000000004c, 0x0000000000000046, 0x0000000000000008, 0x0000000000000030, 0x0000000000000041, 0x0000000000000021, 0x0000000000000032, 0x000000000000004d, 0x0000000000000016, 0x0000000000000011, 0x0000000000000031, 0x0000000000000001, 0x0000000000000001, 0x000000000000002e, 0x000000000000001e, 0x0000000000000044, 0x000000000000003e, 0x000000000000004a, 0x0000000000000005, 0x0000000000000002, 0x000000000000003a, 0x0000000000000000, 0x0000000000000016 };

    char flag[33] = {0};
    for (int i = 0; i < 32; ++i)
    {
        auto v = get(idxs[i]);
        v -= subs[i];
        v /= divs[i];
        flag[i] = static_cast<char>(v);
    }

    print("Done! Your flag is: flag{");
    print(flag);
    print("}\n");

    return 0;
}