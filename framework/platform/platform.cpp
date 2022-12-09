/* Copyright (c) 2019-2022, Arm Limited and Contributors
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

#include "platform.h"

#include <algorithm>
#include <ctime>
#include <mutex>
#include <vector>

#include <spdlog/async_logger.h>
#include <spdlog/details/thread_pool.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "common/logging.h"
//#include "force_close/force_close.h"
#include "platform/filesystem.h"
#include "platform/parsers/CLI11.h"
#include "platform/plugins/plugin.h"
#include "glfw_window.h"

namespace vkb
{

Platform::Platform(int argc, char* argv[])
{
	auto sinks = get_platform_sinks();
	auto logger = std::make_shared<spdlog::logger>("logger", sinks.begin(), sinks.end());

#ifdef VKB_DEBUG
	logger->set_level(spdlog::level::debug);
#else
	logger->set_level(spdlog::level::info);
#endif

	logger->set_pattern(LOGGER_FORMAT);
	spdlog::set_default_logger(logger);

	LOGI("Logger initialized");

	const std::vector<Plugin *> plugins{};
	LOGW("ToDo: create plugins available to the platform; also parse the commandline and respond");
	// ...

//	parser = std::make_unique<CLI11CommandParser>("vulkan_samples", "\n\tVulkan Samples\n\n\t\tA collection of samples to demonstrate the Vulkan best practice.\n", arguments);

	// Process command line arguments
//	if (!parser->parse(associate_plugins(plugins)))
//	{
//		// show help here
//		// ...
//		return;
//	}

	// Subscribe plugins to requested hooks and store activated plugins
//	for (auto *plugin : plugins)
//	{
//		if (plugin->activate_plugin(this, *parser.get()))
//		{
//			auto &plugin_hooks = plugin->get_hooks();
//			for (auto hook : plugin_hooks)
//			{
//				auto it = hooks.find(hook);
//
//				if (it == hooks.end())
//				{
//					auto r = hooks.emplace(hook, std::vector<Plugin *>{});
//
//					if (r.second)
//					{
//						it = r.first;
//					}
//				}
//
//				it->second.emplace_back(plugin);
//			}
//
//			active_plugins.emplace_back(plugin);
//		}
//	}

//	// Platform has been closed by a plugins initialization phase
//	if (close_requested)
//	{
//		return;
//	}
}


Platform::~Platform() {
	spdlog::drop_all();
}

void Platform::update(float delta_time) {
	on_update(delta_time);
}

bool Platform::should_close() const {
    return false;
}

void Platform::finish() {
	// finish the jobs of plugins...
	LOGI("Plugins terminated");
}


std::unique_ptr<Window> Platform::create_window(Application* app, const Window::Properties &properties) {
#if __APPLE__
	return std::make_unique<GlfwWindow>(app, properties);
#else
	// Android window uses native window size
	// Required so that the vulkan sample can create a VkSurface
	std::cerr << "don't how to create a window on this platform:"
	          << "\n\tLine: " << __LINE__
	          << "\n\tFunction: " << __FUNCTION__
	          << "\n\tFile: " << __FILE_NAME__
	          << std::endl;
#endif
}


std::vector<spdlog::sink_ptr> Platform::get_platform_sinks()
{
	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	return sinks;
}


#define HOOK(enum, func)                \
	static auto res = hooks.find(enum); \
	if (res != hooks.end())             \
	{                                   \
		for (auto plugin : res->second) \
		{                               \
			plugin->func;               \
		}                               \
	}

void Platform::on_post_draw(RenderContext &context)
{
	HOOK(Hook::PostDraw, on_post_draw(context));
}

void Platform::on_update(float delta_time)
{
	HOOK(Hook::OnUpdate, on_update(delta_time));
}

void Platform::on_platform_close()
{
	HOOK(Hook::OnPlatformClose, on_platform_close());
}

#undef HOOK

}        // namespace vkb
