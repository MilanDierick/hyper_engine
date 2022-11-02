// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "layer_stack.h"

namespace hp
{
	layer_stack::layer_stack() : m_layer_insert_index(0)
	{
	
	}
	
	layer_stack::~layer_stack()
	{
		for (layer* layer : m_layers)
		{
			layer->on_detach();
			delete layer;
		}
	}
	
	void layer_stack::push_layer(layer* layer)
	{
		m_layers.emplace(m_layers.begin() + m_layer_insert_index, layer);
		++m_layer_insert_index;
	}
	
	void layer_stack::push_overlay(layer* layer)
	{
		m_layers.emplace_back(layer);
	}
	
	void layer_stack::pop_layer(layer* layer)
	{
		const auto iter = std::ranges::find(m_layers, layer);
		if (iter != m_layers.end())
		{
			m_layers.erase(iter);
			--m_layer_insert_index;
		}
	}
	
	void layer_stack::pop_overlay(layer* layer)
	{
		const auto iter = std::ranges::find(m_layers, layer);
		if (iter != m_layers.end())
		{
			m_layers.erase(iter);
		}
	}
	
	std::vector<layer*>::iterator layer_stack::begin()
	{
		return std::begin(m_layers);
	}
	
	std::vector<layer*>::iterator layer_stack::end()
	{
		return std::end(m_layers);
	}
	
	std::vector<layer*>::const_iterator layer_stack::cbegin() const
	{
		return std::cbegin(m_layers);
	}
	
	std::vector<layer*>::const_iterator layer_stack::cend() const
	{
		return std::cend(m_layers);
	}
}  // namespace hp