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

//#include "vk_common.h"
#include "window.h"

struct GLFWwindow;

namespace vkb {
    class Instance;

    class Application;

/**
 * @brief An implementation of GLFW, inheriting the behaviour of the Window interface
 */
    class GlfwWindow : public Window {
    public:
        GlfwWindow(Application *app, const Window::Properties &properties);

        virtual ~GlfwWindow();

        VkSurfaceKHR create_surface(Instance &instance) override;

        VkSurfaceKHR create_surface(VkInstance instance, VkPhysicalDevice physical_device) override;

	    std::vector<const char *> get_surface_extension() const override;

        bool should_close() override;

	    bool is_visible() const override;
	    bool is_focused() const override;

        void process_events() override;

        void close() override;

        float get_dpi_factor() const override;

        float get_content_scale_factor() const override;

	    void set_title(const std::string& title) override;

    private:
        GLFWwindow *handle = nullptr;
    };
}        // namespace vkb
