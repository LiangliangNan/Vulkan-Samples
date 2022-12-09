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

#pragma once

#include <string>

#include "platform/platform.h"
#include "platform/configuration.h"
#include "debug_info.h"
#include "input_events.h"
#include "timer.h"
#include "window.h"


namespace vkb {

    class Window;

    class Application {
    public:
        Application(int argc, char** argv);

        virtual ~Application() = default;

        /**
         * @brief Prepares the application for execution
         * @param platform The platform the application is being run on
         */
        virtual bool prepare();

	    /**
         * @brief Enters the main loop of the application.
         * @return An exit code representing the outcome of the loop. EXIT_SUCCESS on success or EXIT_FAILURE on failure.
	     */
	    int run();

	  public:
        /**
         * @brief Handles cleaning up the application
         */
        virtual void finish();

	    /**
         * @brief Updates the application
         * @param delta_time The time since the last update
	     */
	    virtual void update(float delta_time);

        /**
         * @brief Handles resizing of the window
         * @param width New width of the window
         * @param height New height of the window
         */
        virtual bool resize(const uint32_t width, const uint32_t height) { return true; }

        /**
         * @brief Handles input events of the window
         * @param input_event The input event object
         */
        virtual void input_event(const InputEvent &input_event) {}

        const std::string &get_name() const;

        void set_name(const std::string &name);

	    Window &get_window() const { return *window; }
	    Platform &get_platform() const { return *platform; }
		DebugInfo &get_debug_info() { return debug_info; }

    protected:
        float fps{0.0f};

        float frame_time{0.0f};        // In ms

        uint32_t frame_count{0};

        uint32_t last_frame_count{0};

	  private:
        // The debug info of the app
		DebugInfo debug_info{};

	  protected:
	    Timer timer;
	    std::unique_ptr<Window> window{nullptr};

	    std::unique_ptr<Platform> platform{nullptr};
    };
}        // namespace vkb
