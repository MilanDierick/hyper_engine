#ifndef PLAYGROUND_APPLICATION_H
#define PLAYGROUND_APPLICATION_H

#include "hyper.h"
#include "hyper/core/entrypoint.h"
#include "playground_layer.h"

namespace pg
{
	class playground_application : public hp::application
	{
	public:
		playground_application();
	};

	playground_application::playground_application()
	{
		push_layer(new playground_layer());
	}
} // namespace pg

hp::application* hp::create_application()
{
	return new pg::playground_application();
}

#endif //PLAYGROUND_APPLICATION_H
