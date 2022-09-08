// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef PLAYGROUND_EVENT_OLD_H
#define PLAYGROUND_EVENT_OLD_H

//#include "Base.h"
//#include <functional>
//
//template<typename TFunc>
//class Event_Old;
//
//template<class RetType, class... Args>
//class Event_Old<RetType(Args...)> final
//{
//	using Callable = typename std::function<RetType(Args...)>;
//
//	struct ComparableCallable
//	{
//		Callable callable;
//		void* object = nullptr;
//		uint8_t* functor = nullptr;
//		int functor_size = 0;
//
//		ComparableCallable() = default;
//
//		~ComparableCallable()
//		{
//			delete[] functor;
//		}
//
//		ComparableCallable(const ComparableCallable& other) = default;
//		ComparableCallable(ComparableCallable&& other) noexcept = default;
//
//		ComparableCallable& operator=(const ComparableCallable& other)
//		{
//			if (other == this)
//			{
//				return *this;
//			}
//
//			callable = other.callable;
//			object = other.object;
//			functor_size = other.functor_size;
//
//			if (other.functor_size == 0)
//			{
//				functor = nullptr;
//			}
//			else
//			{
//				functor = new uint8_t[other.functor_size];
//				std::memcpy(functor, other.functor, other.functor_size);
//			}
//
//			return *this;
//		}
//
//		ComparableCallable& operator=(ComparableCallable&& other) noexcept
//		{
//			if (other == *this)
//			{
//				return *this;
//			}
//
//			callable = other.callable;
//			object = other.object;
//			functor_size = other.functor_size;
//
//			if (other.functor_size == 0)
//			{
//				functor = nullptr;
//			}
//			else
//			{
//				try
//				{
//					functor = new uint8_t[other.functor_size];
//				}
//				catch (std::exception& exception)
//				{
//					HP_DEBUGBREAK();
//				}
//
//				std::memcpy(functor, other.functor, other.functor_size);
//			}
//
//			return *this;
//		}
//
//		bool operator==(const ComparableCallable& rhs) const
//		{
//			if (object == nullptr && rhs.object == nullptr)
//			{
//				return callable.target_type() == rhs.callable.target_type();
//			}
//
//			return object == rhs.object && functor == rhs.functor
//				&& std::memcpy(functor, rhs.functor, functor_size) == 0;
//
//		}
//
//		bool operator!=(const ComparableCallable& rhs) const
//		{
//			return !(rhs == *this);
//		}
//	};
//
// public:
//	Event_Old() = default;
//	~Event_Old() = default;
//
//	Event_Old(const Event_Old& other) = delete;
//	Event_Old(Event_Old&& other) noexcept = delete;
//	Event_Old& operator=(const Event_Old& other) = delete;
//	Event_Old& operator=(Event_Old&& other) = delete;
//
//	void operator+=(Callable callable)
//	{
//		add_callable(std::move(callable));
//	}
//
//	void operator-=(Callable callable)
//	{
//		remove_callable(std::move(callable));
//	}
//
//	template<typename T>
//	void Bind(RetType(T::*p_func)(Args...), T* object)
//	{
//		ComparableCallable comparable_callable;
//
//		comparable_callable.callable = [object, p_func](Args&& ...args)
//		{
//			return (object->*p_func)(std::forward<Args>(args)...);
//		};
//
//		comparable_callable.functor_size = sizeof(p_func);
//		comparable_callable.functor = new uint8_t[comparable_callable.functor_size];
//		std::memcpy(comparable_callable.functor, static_cast<void*>(&p_func), sizeof(p_func));
//		comparable_callable.object = object;
//
//		add_callable(comparable_callable);
//	}
//
//	template<typename T>
//	void Unbind(RetType(T::*p_func)(Args...), T* object)
//	{
//		ComparableCallable comparable_callable;
//
//		comparable_callable.functor_size = sizeof(p_func);
//		comparable_callable.functor = new uint8_t[comparable_callable.functor_size];
//		std::memcpy(comparable_callable.functor, static_cast<void*>(&p_func), sizeof(p_func));
//		comparable_callable.object = object;
//
//		remove_callable(comparable_callable);
//	}
//
//	void operator()()
//	{
//		for (ComparableCallable& comparable_callable : m_callables)
//		{
//			comparable_callable.callable();
//		}
//	}
//
//	template<typename TArg0, typename ...Args2>
//	void operator()(TArg0 a1, Args2... tail)
//	{
//		for (ComparableCallable& comparable_callable : m_callables)
//		{
//			comparable_callable.callable(a1, tail...);
//		}
//	}
//
// private:
//	std::vector<ComparableCallable> m_callables;
//
//	void add_callable(const ComparableCallable& callable)
//	{
//		m_callables.push_back(callable);
//	}
//
//	void remove_callable(const ComparableCallable& callable)
//	{
//		std::erase_if(m_callables, [&callable](const ComparableCallable& callable_)
//		{
//			return callable_ == callable;
//		});
//	}
//};

#endif //PLAYGROUND_EVENT_OLD_H
