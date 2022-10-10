// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_TIMER_H
#define HYPER_TIMER_H

#include <chrono>

namespace hp
{
	class timer
	{
	 public:
		timer()
		{
			reset();
		}
		
		void reset()
		{
			m_start = std::chrono::high_resolution_clock::now();
		}
		
		float elapsed()
		{
			return static_cast<float>((std::chrono::high_resolution_clock::now() - m_start).count()) * 0.001F * 0.001F * 0.001F;
		};
		
		float elapsed_milliseconds()
		{
			return elapsed() * 1000.0F;
		}
	 
	 private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
	};
}  // namespace hp

#endif //HYPER_TIMER_H
