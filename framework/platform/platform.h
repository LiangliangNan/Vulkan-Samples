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

#pragma once

#include "platform/plugins/plugin.h"
#include "platform/window.h"

#if defined(VK_USE_PLATFORM_XLIB_KHR)
#	undef Success
#endif

namespace vkb
{

class Application;

class Platform
{
  public:
	/**
	 * @brief Constructor.
	 * It initialize the platform and creates plugins available to the platform.
	 */
	explicit Platform(int argc, char *argv[]);
	virtual ~Platform();

	/**
	 * @brief Update the plugins.
	 */
	virtual void update(float delta_time);

	/**
	 * Should the application be closed (a plugin can request a close of the application).
	 * @return true if the application has been requested to close.
	 */
	virtual bool should_close() const;

	/**
	 * @brief Finishes the jobs of the plugins.
	 */
	virtual void finish();

	/**
	 * @brief Handles the creation of the window
	 * @param properties Preferred window configuration
	 */
	static std::unique_ptr<Window> create_window(Application *app, const Window::Properties &properties = Window::Properties());

	template <class T>
	T *get_plugin() const;

	template <class T>
	bool using_plugin() const;

	/// Implementation of post draw (for, e.g., additional draw of a plugin).
	void on_post_draw(RenderContext &context);

  protected:

	std::vector<Plugin *> active_plugins;

	std::unordered_map<Hook, std::vector<Plugin *>> hooks;

	virtual std::vector<spdlog::sink_ptr> get_platform_sinks();

	void on_update(float delta_time);
	void on_platform_close();

  private:
	/// Static so can be set via JNI code in android_platform.cpp
	//	static std::vector<std::string> arguments;
};

template <class T>
bool Platform::using_plugin() const
{
	return !plugins::with_tags<T>(active_plugins).empty();
}

template <class T>
T *Platform::get_plugin() const
{
	assert(using_plugin<T>() && "Plugin is not enabled but was requested");
	const auto plugins = plugins::with_tags<T>(active_plugins);
	return dynamic_cast<T *>(plugins[0]);
}
}        // namespace vkb
