/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2025 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/**
 * Main include header for the SDL library, version 3.2.26
 *
 * It is almost always best to include just this one header instead of
 * picking out individual headers included here. There are exceptions to
 * this rule--SDL_main.h is special and not included here--but usually
 * letting SDL.h include the kitchen sink for you is the correct approach.
 */

#ifndef SDL_h_
#define SDL_h_

#include <includes/SDL_stdinc.h>
#include <includes/SDL_assert.h>
#include <includes/SDL_asyncio.h>
#include <includes/SDL_atomic.h>
#include <includes/SDL_audio.h>
#include <includes/SDL_bits.h>
#include <includes/SDL_blendmode.h>
#include <includes/SDL_camera.h>
#include <includes/SDL_clipboard.h>
#include <includes/SDL_cpuinfo.h>
#include <includes/SDL_dialog.h>
#include <includes/SDL_endian.h>
#include <includes/SDL_error.h>
#include <includes/SDL_events.h>
#include <includes/SDL_filesystem.h>
#include <includes/SDL_gamepad.h>
#include <includes/SDL_gpu.h>
#include <includes/SDL_guid.h>
#include <includes/SDL_haptic.h>
#include <includes/SDL_hidapi.h>
#include <includes/SDL_hints.h>
#include <includes/SDL_init.h>
#include <includes/SDL_iostream.h>
#include <includes/SDL_joystick.h>
#include <includes/SDL_keyboard.h>
#include <includes/SDL_keycode.h>
#include <includes/SDL_loadso.h>
#include <includes/SDL_locale.h>
#include <includes/SDL_log.h>
#include <includes/SDL_messagebox.h>
#include <includes/SDL_metal.h>
#include <includes/SDL_misc.h>
#include <includes/SDL_mouse.h>
#include <includes/SDL_mutex.h>
#include <includes/SDL_pen.h>
#include <includes/SDL_pixels.h>
#include <includes/SDL_platform.h>
#include <includes/SDL_power.h>
#include <includes/SDL_process.h>
#include <includes/SDL_properties.h>
#include <includes/SDL_rect.h>
#include <includes/SDL_render.h>
#include <includes/SDL_scancode.h>
#include <includes/SDL_sensor.h>
#include <includes/SDL_storage.h>
#include <includes/SDL_surface.h>
#include <includes/SDL_system.h>
#include <includes/SDL_thread.h>
#include <includes/SDL_time.h>
#include <includes/SDL_timer.h>
#include <includes/SDL_tray.h>
#include <includes/SDL_touch.h>
#include <includes/SDL_version.h>
#include <includes/SDL_video.h>
#include <includes/SDL_oldnames.h>

#endif /* SDL_h_ */
