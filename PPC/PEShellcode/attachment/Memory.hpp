#pragma once

#include <memory>
#include <type_traits>
#include <utility>
#include <concepts>

// provides access to the game's operator new and operator delete.
namespace YRMemory {
	// the game's operator new
	__declspec(naked) inline void* __cdecl Allocate(size_t sz) 
    {
        __asm
        {
            mov eax, 0x7C8E17
            jmp eax
        } 
    }

	// the game's operator delete
	__declspec(naked) inline void __cdecl Deallocate(const void* mem) 
    {
        __asm
        {
            mov eax, 0x7C8B3D
            jmp eax
        } 
    }
}

template<typename T>
struct needs_vector_delete : std::integral_constant<bool,
	!std::is_scalar<T>::value && !std::is_trivially_destructible<T>::value> {};

// this is a stateless basic allocator definition that manages memory using the
// game's operator new and operator delete methods. do not use it directly,
// though. use std::allocator_traits, which will fill in the blanks.
template <typename T>
struct GameAllocator {
	using value_type = T;

	constexpr GameAllocator() noexcept = default;

	template <typename U>
	constexpr GameAllocator(const GameAllocator<U>&) noexcept { }

	constexpr bool operator == (const GameAllocator&) const noexcept { return true; }
	constexpr bool operator != (const GameAllocator&) const noexcept { return false; }

	T* allocate(const size_t count) const noexcept
	{
		return static_cast<T*>(YRMemory::Allocate(count * sizeof(T)));
	}

	void deallocate(T* const ptr, size_t count) const noexcept
	{
		YRMemory::Deallocate(ptr);
	}
};

// construct or destroy objects using an allocator.
class Memory {
public:
	// construct scalars
	template <typename T, typename TAlloc, typename... TArgs>
	static inline T* Create(TAlloc& alloc, TArgs&&... args)
	{
		auto const ptr = std::allocator_traits<TAlloc>::allocate(alloc, 1);
		std::allocator_traits<TAlloc>::construct(alloc, ptr, std::forward<TArgs>(args)...);
		return ptr;
	};

	// destruct scalars
	template<typename T, typename TAlloc>
	static inline void Delete(TAlloc& alloc, T* ptr)
	{
		if (ptr)
		{
			std::allocator_traits<TAlloc>::destroy(alloc, ptr);
			std::allocator_traits<TAlloc>::deallocate(alloc, ptr, 1);
		}
	};

	// construct vectors
	template <typename T, typename TAlloc, typename... TArgs>
	static inline T* CreateArray(TAlloc& alloc, size_t capacity, TArgs&&... args)
	{
		auto const ptr = std::allocator_traits<TAlloc>::allocate(alloc, capacity);
		if (capacity && !sizeof...(args) && std::is_scalar<T>::value)
		{
			// set to 0
			std::memset(ptr, 0, capacity * sizeof(T));
		}
		else
		{
			for (size_t i = 0; i < capacity; ++i)
			{
				// use args... here. can't move args, because we need to reuse them
				std::allocator_traits<TAlloc>::construct(alloc, &ptr[i], args...);
			}
		}
		return ptr;
	}

	// destruct vectors
	template<typename T, typename TAlloc>
	static inline void DeleteArray(TAlloc& alloc, T* ptr, size_t capacity)
	{
		if (ptr)
		{
			// call the destructor if required
			if (capacity && !std::is_trivially_destructible<T>::value)
			{
				for (size_t i = 0; i < capacity; ++i)
					std::allocator_traits<TAlloc>::destroy(alloc, &ptr[i]);
			}

			std::allocator_traits<TAlloc>::deallocate(alloc, ptr, capacity);
		}
	};
};

template <typename T, typename... TArgs>
requires std::constructible_from<T, TArgs...>
static inline T* GameCreate(TArgs&&... args)
{
	GameAllocator<T> alloc;
	return Memory::Create<T>(alloc, std::forward<TArgs>(args)...);
}

template<typename T>
static inline void GameDelete(T* ptr)
{
	GameAllocator<T> alloc;
	Memory::Delete(alloc, ptr);
}

template <typename T, typename... TArgs>
requires std::constructible_from<T, TArgs...>
static inline T* GameCreateArray(size_t capacity, TArgs&&... args)
{

	GameAllocator<T> alloc;
	return Memory::CreateArray<T>(alloc, capacity, std::forward<TArgs>(args)...);
}

template<typename T>
static inline void GameDeleteArray(T* ptr, size_t capacity)
{
	GameAllocator<T> alloc;
	Memory::DeleteArray(alloc, ptr, capacity);
}

struct GameDeleter
{
	template <typename T>
	void operator ()(T* ptr) noexcept
	{
		if (ptr)
			GameDelete(ptr);
	}
};

template <typename T>
using GameUniquePtr = std::unique_ptr<T, GameDeleter>;

template <typename T, typename... TArgs>
requires std::constructible_from<T, TArgs...>
static inline GameUniquePtr<T> GameCreateUnique(TArgs&&... args)
{
	auto ptr = GameCreate<T>(args...);
	GameUniquePtr<T> ret;
	ret.reset(ptr);
	return ret;
}