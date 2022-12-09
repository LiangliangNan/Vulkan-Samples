/* Copyright (c) 2019-2021, Arm Limited and Contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 the "License";
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "application.h"
#include "platform/platform.h"

namespace vkb
{

Application::Application(int argc, char *argv[])
{
	platform = std::make_unique<Platform>(argc, argv);

	Window::Properties prop{};
	prop.title = "Application";
	window = platform->create_window(this, prop);
}

bool Application::prepare()
{
	return true;
}

int Application::run()
{
	while (!window->should_close() && !platform->should_close())
	{
		try
		{
			auto delta_time = static_cast<float>(timer.tick<Timer::Seconds>());
			if (window->is_visible() /* && window->is_focused()*/)
			{
				platform->update(delta_time);
				update(delta_time);
			}

			window->process_events();
		}
		catch (std::exception &e)
		{
			LOGE("Error Message: {}", e.what());
			LOGE("Failed when running application {}", get_name());
			finish();
			// on_app_error(active_app->get_name());
			return EXIT_FAILURE;
		}
	}

	finish();
	return EXIT_SUCCESS;
}

void Application::finish()
{
	platform->finish();
}

void Application::update(float delta_time)
{
	fps        = 1.0f / delta_time;
	frame_time = delta_time * 1000.0f;
}

Window::Extent Application::resize(const Window::Extent &extent) {
	return window->resize(extent);
}

const std::string &Application::get_name() const
{
	return window->get_properties().title;
}

void Application::set_name(const std::string &name)
{
	window->set_title(name);
}

}        // namespace vkb
