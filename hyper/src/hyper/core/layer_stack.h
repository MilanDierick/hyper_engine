// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_LAYER_STACK_H
#define HYPER_LAYER_STACK_H

#include "layer.h"

#include <vector>

namespace hp
{
	class layer_stack
	{
	 public:
		layer_stack();
		~layer_stack();
		
		layer_stack(const layer_stack& other) = delete;
		layer_stack(layer_stack&& other) noexcept = delete;
		layer_stack& operator=(const layer_stack& other) = delete;
		layer_stack& operator=(layer_stack&& other) = delete;
		
		void push_layer(layer* layer);
		void push_overlay(layer* layer);
		void pop_layer(layer* layer);
		void pop_overlay(layer* layer);
		
		[[nodiscard]] std::vector<layer*>::iterator begin();
		[[nodiscard]] std::vector<layer*>::iterator end();
		[[nodiscard]] std::vector<layer*>::const_iterator cbegin() const;
		[[nodiscard]] std::vector<layer*>::const_iterator cend() const;
	 
	 private:
		std::vector<layer*> m_layers;
		int32_t m_layer_insert_index;
	};
	
}  // namespace hp

#endif //HYPER_LAYER_STACK_H
