// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_APPLICATION_H
#define HYPER_APPLICATION_H

namespace hyper
{
	class application
	{
	public:
		application();
		~application() = default;

		application(const application& other) = delete;
		application(application&& other) noexcept = delete;
		application& operator=(const application& other) = delete;
		application& operator=(application&& other) = delete;

		void execute();
		void terminate();

		[[nodiscard]] application* instance();
		[[nodiscard]] application* instance() const;

	private:
		static application* m_instance;
		bool m_is_running;
	};

	extern application* create_application();
} // namespace hyper

#endif //HYPER_APPLICATION_H
