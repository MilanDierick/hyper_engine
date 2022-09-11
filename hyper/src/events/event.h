// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef PLAYGROUND_EVENT_H
#define PLAYGROUND_EVENT_H

#include <functional>

namespace hp
{
	template<typename ArgType>
	class event
	{
		using callable = typename std::function<void(ArgType)>;
		
		struct comparable_callable
		{
			callable callable;
			void* object = nullptr;
			uint8_t* functor = nullptr;
			int functor_size = 0;
			
			comparable_callable() = default;
			
			explicit comparable_callable(typename std::function<void(ArgType)>&& callable)
				: callable(std::move(callable))
			{
			}
			
			~comparable_callable()
			{
				delete[] functor;
			}
			
			comparable_callable(const comparable_callable& other) = default;
			comparable_callable(comparable_callable&& callable) noexcept = default;
			
			comparable_callable& operator=(const comparable_callable& other)
			{
				if (other == this)
				{
					return *this;
				}
				
				callable = other.callable;
				object = other.object;
				functor_size = other.functor_size;
				
				if (other.functor_size == 0)
				{
					functor = nullptr;
				}
				else
				{
					functor = new uint8_t[other.functor_size];
					std::memcpy(functor, other.functor, other.functor_size);
				}
				
				return *this;
			}
			
			comparable_callable& operator=(comparable_callable&& other) noexcept
			{
				if (other == *this)
				{
					return *this;
				}
				
				callable = other.callable;
				object = other.object;
				functor_size = other.functor_size;
				
				if (other.functor_size == 0)
				{
					functor = nullptr;
				}
				else
				{
					functor = new uint8_t[other.functor_size]; // NOLINT(bugprone-unhandled-exception-at-new)
					std::memcpy(functor, other.functor, other.functor_size);
				}
				
				return *this;
			}
			
			bool operator==(const comparable_callable& rhs) const
			{
				if (object == nullptr && rhs.object == nullptr)
				{
					return callable.target_type() == rhs.callable.target_type();
				}
				
				return object == rhs.object && functor_size == rhs.functor_size && *functor == *rhs.functor;
			}
			
			bool operator!=(const comparable_callable& rhs) const
			{
				return !(rhs == *this);
			}
		};
	 
	 public:
		event() = default;
		~event() = default;
		
		event(const event& other) = delete;
		event(event&& other) noexcept = delete;
		event& operator=(const event& other) = delete;
		event& operator=(event&& other) = delete;
		
		void bind(callable callable)
		{
			auto* p_callable = new comparable_callable(std::move(callable));
			add_callable(p_callable);
		}
		
		template<typename TObject>
		void bind(void(TObject::*function)(ArgType), TObject* object)
		{
			auto* p_callable = new comparable_callable();
			
			p_callable->callable = [object, function](ArgType&& arg)
			{
				return (object->*function)(std::forward<ArgType>(arg));
			};
			
			p_callable->functor_size = sizeof(function);
			p_callable->functor = new uint8_t[p_callable->functor_size];
			std::memcpy(p_callable->functor, static_cast<void*>(&function), sizeof(function));
			p_callable->object = object;
			
			add_callable(p_callable);
		}
		
		void unbind(callable callable)
		{
			auto* p_callable = new comparable_callable(std::move(callable));
			remove_callable(p_callable);
		}
		
		template<typename TObject>
		void unbind(void(TObject::*function)(ArgType), TObject* object)
		{
			auto* p_callable = new comparable_callable();
			
			p_callable->functor_size = sizeof(function);
			p_callable->functor = new uint8_t[p_callable->functor_size];
			std::memcpy(p_callable->functor, static_cast<void*>(&function), sizeof(function));
			p_callable->object = object;
			
			remove_callable(p_callable);
		}
		
		void invoke(ArgType arg)
		{
			std::for_each(std::begin(m_callbacks), std::end(m_callbacks), [&arg](comparable_callable* p_callable)
			{
				std::invoke(p_callable->callable, arg);
			});
		}
	 
	 private:
		std::vector<comparable_callable*> m_callbacks;
		
		void add_callable(comparable_callable* p_callable)
		{
			m_callbacks.push_back(p_callable);
		}
		
		void remove_callable(comparable_callable* callable)
		{
			std::erase_if(m_callbacks, [&callable](const comparable_callable* p_other)
			{
				return *callable == *p_other;
			});
		}
	};
}

#endif //PLAYGROUND_EVENT_H
