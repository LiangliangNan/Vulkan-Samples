/* Copyright (c) 2018-2022, Arm Limited and Contributors
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

#include <string>

#include "common/optional.h"
#include "common/vk_common.h"
#include "core/instance.h"

namespace vkb {
/**
 * @brief An interface class, declaring the behaviour of a Window
 */
    class Window {
    public:
        struct Extent {
            uint32_t width;
            uint32_t height;
        };

        enum class Mode {
            Headless,
            Fullscreen,
            FullscreenBorderless,
            FullscreenStretch,
            Default
        };

        enum class Vsync {
            OFF,
            ON,
            Default
        };

        struct Properties {
            std::string title = "";
            Mode mode = Mode::Default;
            bool resizable = true;
            Vsync vsync = Vsync::Default;
            Extent extent = {1280, 720};
        };

        /**
         * @brief Constructs a Window
         * @param properties The preferred configuration of the window
         */
        Window(const Properties &properties);

        virtual ~Window() = default;

        /**
         * @brief Gets a handle from the platform's Vulkan surface
         * @param instance A Vulkan instance
         * @returns A VkSurfaceKHR handle, for use by the application
         */
        virtual VkSurfaceKHR create_surface(Instance &instance) = 0;

        /**
         * @brief Gets a handle from the platform's Vulkan surface
         * @param instance A Vulkan instance
         * @param physical_device A Vulkan PhysicalDevice
         * @returns A VkSurfaceKHR handle, for use by the application
         */
        virtual VkSurfaceKHR create_surface(VkInstance instance, VkPhysicalDevice physical_device) = 0;

	    /**
	     * @return The VkInstance extension name for the platform
	     */
	    virtual std::vector<const char *> get_surface_extension() const = 0;

        /**
         * @brief Checks if the window should be closed
         */
        virtual bool should_close() = 0;

	    virtual bool is_visible() const = 0;
	    virtual bool is_focused() const = 0;

        /**
         * @brief Handles the processing of all underlying window events
         */
        virtual void process_events();

        /**
         * @brief Requests to close the window
         */
        virtual void close() = 0;

        /**
         * @return The dot-per-inch scale factor
         */
        virtual float get_dpi_factor() const = 0;

        /**
         * @return The scale factor for systems with heterogeneous window and pixel coordinates
         */
        virtual float get_content_scale_factor() const = 0;

        /**
         * @brief Attempt to resize the window - not guaranteed to change
         *
         * @param extent The preferred window extent
         * @return Extent The new window extent
         */
        Extent resize(const Extent &extent);

        /**
         * @brief Get the display present info for the window if needed
         *
         * @param info Filled in when the method returns true
         * @param src_width The width of the surface being presented
         * @param src_height The height of the surface being presented
         * @return true if the present info was filled in and should be used
         * @return false if the extra present info should not be used. info is left untouched.
         */
        virtual bool get_display_present_info(VkDisplayPresentInfoKHR *info,
                                              uint32_t src_width, uint32_t src_height) const;

        const Extent &get_extent() const;

        Mode get_window_mode() const;

	    /** @brief Gets the application title */
	    const Properties& get_properties() const { return properties; }

	    virtual void set_title(const std::string& title);

    protected:
        Properties properties;
    };
}        // namespace vkb
