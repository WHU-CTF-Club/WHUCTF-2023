# Description
有时候，计算的方法很重要。

# Difficulty
:star::star::star::star:

# Flag
`flag{Th3ts_WhY_wE_N33d_A_B3tter_Algo!}`

# Platform (Optional)
Ubuntu 22.04 LTS

# Hint (Optional)
C++20引入了协程机制，你可以在[这里](https://en.cppreference.com/w/cpp/coroutine/coroutine_handle)找到一个样例程序，并使用`g++ test.cpp -std=c++20 -o test`来尝试编译它。

# WriteUp
逆向后发现就是算斐波那契数列，抄个矩阵加速的板子就行。
```cpp
#include <iostream>
#include <numbers>
#include <numeric>

constexpr unsigned long long MOD = 0xFFFFFFFF;

template<typename T, unsigned long long MOD>
class mint
{
public:
    explicit mint() : value{ 0 } {}
    explicit mint(const T& v) : value{ v } {}
    explicit mint(T&& v) : value{ v } {}
    explicit mint(const mint& another) { value = another.value; }
    explicit mint(mint&& another) { value = another.value; }
    mint& operator=(const mint& another) { value = another.value; return *this; }
    mint& operator=(mint&& another) { value = another.value; return *this; }
    mint& operator=(const T& v) { value = v; return *this; }
    mint& operator=(T&& v) { value = v; return *this; }

    operator T() const
    {
        return value;
    }

    mint& operator+=(const mint& another) { value += another.value; update(); return *this; }
    friend mint operator+(const mint& a, const mint& b) { mint result{ a }; result += b; return (mint)result; }

    mint& operator-=(const mint& another) { value -= another.value; update(); return *this; }
    friend mint operator-(const mint& a, const mint& b) { mint result{ a }; result -= b; return (mint)result; }

    mint& operator*=(const mint& another) { value *= another.value; update(); return *this; }
    friend mint operator*(const mint& a, const mint& b) { mint result{ a }; result *= b; return (mint)result; }

    mint& operator/=(const mint& another) { value /= another.value; update(); return *this; }
    friend mint operator/(const mint& a, const mint& b) { mint result{ a }; result /= b; return (mint)result; }

    mint& operator%=(const mint& another) { value %= another.value; update(); return *this; }
    friend mint operator%(const mint& a, const mint& b) { mint result{ a }; result %= b; return (mint)result; }


private:
    void update()
    {
        value = value % MOD;
    }

    T value;
};

template<typename T>
class matrix
{
public:
    matrix() = default;
    ~matrix() = default;

    matrix& operator*=(const matrix& another)
    {
        *this = *this * another;
        return *this;
    }

    friend matrix operator*(const matrix& a, const matrix& b)
    {
        matrix result;
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                for (int k = 0; k < 2; ++k)
                    result[i][j] += a[i][k] * b[k][j];
            }
        }
        return result;
    }

    T* operator[](int idx)
    {
        return data[idx];
    }

    const T* operator[](int idx) const
    {
        return data[idx];
    }

private:
    T data[2][2];
};

size_t fib(size_t n)
{
    if (n == 0)
        return 0; 
    else if (n == 1)
        return 1; 

    using matrix_t = matrix<mint<unsigned long long, MOD>>;

    matrix_t identity;
    identity[0][0] = identity[1][1] = 1;
    identity[0][1] = identity[1][0] = 0;
    matrix_t a;
    a[0][0] = a[0][1] = a[1][0] = 1;
    a[1][1] = 0;

    n -= 1;
    while (n > 0)
    {
        if (n % 2 != 0)
            identity *= a;
        a *= a;
        n /= 2;
    }

    return identity[0][0];
}

#include <random>

// flag{Th3ts_WhY_wE_N33d_A_B3tter_Algo!}
int main()
{
    size_t idxs[32] = { 0x0f705d0a8f3f4f5f, 0x0af4f65c7f0274ec, 0x03a1dd096b65a01a, 0x5a8528422f6d09b2, 0x435bf69613c3dbc8, 0x35094fc0a11a1222, 0x46164fb35afaabab, 0x9b286f15feb95e22, 0xe52d79256cf22943, 0xf66797457072cbc1, 0x07f5b564860a6721, 0x55bc75ae208f9f46, 0x3d12fabac77ac707, 0x653fe0ac0fa48f0e, 0x8bbe5efc003b4bb4, 0xbd3e420b2cddfbc3, 0x517099a5473cfcad, 0x5497dee39a94724a, 0xb93ccb3d9eb2b076, 0x352b2ada9679a90d, 0x9b80784b9fddba0a, 0x7505573a5a07f82f, 0xa9fe0c4d81f6500c, 0x15a395d8224ea931, 0x485a0c6d43148670, 0xf733fd07c3a23ea5, 0x24ff13037a00bf2a, 0xfdd4207650095fb0, 0x368df4a095720ab6, 0x533e9586eaef1285, 0x87ef103dac5f8d3c, 0x00c3912b47abce9f };
    size_t divs[32] = { 0x0000000001930adc, 0x00000000009194fa, 0x0000000004981de3, 0x00000000005353d3, 0x0000000000b47401, 0x00000000005a20a7, 0x000000000001222c, 0x00000000000b287e, 0x0000000000b6fe6d, 0x0000000001e7ff68, 0x00000000020e19f1, 0x00000000024e45eb, 0x0000000002687c57, 0x0000000002f36730, 0x0000000001f14c89, 0x00000000023230f1, 0x0000000000eeb5e5, 0x0000000000c524cd, 0x0000000002bd88b1, 0x0000000001821d58, 0x00000000002b65ed, 0x0000000001b9ad8d, 0x0000000000eba098, 0x000000000187f94f, 0x0000000001c752e3, 0x0000000000de5da0, 0x00000000026ff365, 0x0000000000a3e3da, 0x0000000000116e71, 0x000000000165dce7, 0x0000000000a3dbec, 0x00000000067a6994 };
    size_t subs[32] = { 0x000000000000002b, 0x0000000000000045, 0x0000000000000025, 0x0000000000000009, 0x0000000000000063, 0x0000000000000056, 0x0000000000000050, 0x000000000000004f, 0x0000000000000055, 0x000000000000004c, 0x0000000000000046, 0x0000000000000008, 0x0000000000000030, 0x0000000000000041, 0x0000000000000021, 0x0000000000000032, 0x000000000000004d, 0x0000000000000016, 0x0000000000000011, 0x0000000000000031, 0x0000000000000001, 0x0000000000000001, 0x000000000000002e, 0x000000000000001e, 0x0000000000000044, 0x000000000000003e, 0x000000000000004a, 0x0000000000000005, 0x0000000000000002, 0x000000000000003a, 0x0000000000000000, 0x0000000000000016 };

    std::string result;
    for (int i = 0; i < 32; ++i)
    {
        auto v = fib(idxs[i]);
        v -= subs[i];
        v /= divs[i];
        result += static_cast<char>(v);
    }

    std::cout << "flag{" << result << "}" << std::endl;
}
```