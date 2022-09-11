// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef PLAYGROUND_GRAPHICS_CONTEXT_H
#define PLAYGROUND_GRAPHICS_CONTEXT_H

namespace hp
{
	class graphics_context
	{
	 public:
		virtual ~graphics_context() = default;
		
		virtual void init() = 0;
		virtual void swap_buffers() = 0;
	};
}

#endif //PLAYGROUND_GRAPHICS_CONTEXT_H
