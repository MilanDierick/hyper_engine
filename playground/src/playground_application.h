#ifndef PLAYGROUND_APPLICATION_H
#define PLAYGROUND_APPLICATION_H

#include "hyper.h"

namespace pg
{
	class playground_application final : public hp::application
	{
	public:
		playground_application();
	};
}  // namespace pg

inline hp::application* hp::create_application()
{
	return new pg::playground_application();
}

#endif //PLAYGROUND_APPLICATION_H
