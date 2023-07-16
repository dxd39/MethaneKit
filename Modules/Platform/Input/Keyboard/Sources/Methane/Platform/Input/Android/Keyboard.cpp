/******************************************************************************

Copyright 2019-2021 Evgeny Gorodetskiy

Licensed under the Apache License, Version 2.0 (the "License"),
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************

FILE: Methane/Platform/Linux/Keyboard.cpp
Linux platform specific types and implementation of Keyboard abstractions.

******************************************************************************/

#include <Methane/Platform/Input/Keyboard.h>

#include <Methane/Instrumentation.h>

#include <android/input.h>

namespace Methane::Platform::Input::Keyboard
{

Key KeyConverter::GetKeyByNativeCode(const NativeKey& native_key) noexcept
{
    META_FUNCTION_TASK();
    switch (native_key.key) // NOSONAR - long switch
    {
    case AKEYCODE_0:                        return Key::Num0;
    case AKEYCODE_1:                        return Key::Num1;
    case AKEYCODE_2:                        return Key::Num2;
    case AKEYCODE_3:                        return Key::Num3;
    case AKEYCODE_4:                        return Key::Num4;
    case AKEYCODE_5:                        return Key::Num5;
    case AKEYCODE_6:                        return Key::Num6;
    case AKEYCODE_7:                        return Key::Num7;
    case AKEYCODE_8:                        return Key::Num8;
    case AKEYCODE_9:                        return Key::Num9;
    case AKEYCODE_A:                        return Key::A;
    case AKEYCODE_B:                        return Key::B;
    case AKEYCODE_C:                        return Key::C;
    case AKEYCODE_D:                        return Key::D;
    case AKEYCODE_E:                        return Key::E;
    case AKEYCODE_F:                        return Key::F;
    case AKEYCODE_G:                        return Key::G;
    case AKEYCODE_H:                        return Key::H;
    case AKEYCODE_I:                        return Key::I;
    case AKEYCODE_J:                        return Key::J;
    case AKEYCODE_K:                        return Key::K;
    case AKEYCODE_L:                        return Key::L;
    case AKEYCODE_M:                        return Key::M;
    case AKEYCODE_N:                        return Key::N;
    case AKEYCODE_O:                        return Key::O;
    case AKEYCODE_P:                        return Key::P;
    case AKEYCODE_Q:                        return Key::Q;
    case AKEYCODE_R:                        return Key::R;
    case AKEYCODE_S:                        return Key::S;
    case AKEYCODE_T:                        return Key::T;
    case AKEYCODE_U:                        return Key::U;
    case AKEYCODE_V:                        return Key::V;
    case AKEYCODE_W:                        return Key::W;
    case AKEYCODE_X:                        return Key::X;
    case AKEYCODE_Y:                        return Key::Y;
    case AKEYCODE_Z:                        return Key::Z;
    case AKEYCODE_APOSTROPHE:               return Key::Apostrophe;
    case AKEYCODE_BACKSLASH:                return Key::BackSlash;
    case AKEYCODE_COMMA:                    return Key::Comma;
    case AKEYCODE_NUMPAD_EQUALS:            return Key::Equal;
    case AKEYCODE_SEMICOLON:                return Key::Semicolon;
    case AKEYCODE_SLASH:                    return Key::Slash;
    case AKEYCODE_MINUS:                    return Key::Minus;
    case AKEYCODE_PERIOD:                   return Key::Period;
    case AKEYCODE_LEFT_BRACKET:             return Key::LeftBracket;
    case AKEYCODE_RIGHT_BRACKET:            return Key::RightBracket;
    case AKEYCODE_DEL:                      return Key::Delete;
    case AKEYCODE_HOME:                     return Key::Home;
    case AKEYCODE_PAGE_UP:                  return Key::PageUp;
    case AKEYCODE_PAGE_DOWN:                return Key::PageDown;
    case AKEYCODE_ENTER:                    return Key::Enter;
    case AKEYCODE_ESCAPE:                   return Key::Escape;
    case AKEYCODE_INSERT:                   return Key::Insert;
    case AKEYCODE_BREAK:                    return Key::Pause;
    case AKEYCODE_SPACE:                    return Key::Space;
    case AKEYCODE_TAB:                      return Key::Tab;
    case AKEYCODE_F1:                       return Key::F1;
    case AKEYCODE_F2:                       return Key::F2;
    case AKEYCODE_F3:                       return Key::F3;
    case AKEYCODE_F4:                       return Key::F4;
    case AKEYCODE_F5:                       return Key::F5;
    case AKEYCODE_F6:                       return Key::F6;
    case AKEYCODE_F7:                       return Key::F7;
    case AKEYCODE_F8:                       return Key::F8;
    case AKEYCODE_F9:                       return Key::F9;
    case AKEYCODE_F10:                      return Key::F10;
    case AKEYCODE_F11:                      return Key::F11;
    case AKEYCODE_F12:                      return Key::F12;
    case AKEYCODE_SHIFT_LEFT:               return Key::LeftShift;
    case AKEYCODE_SHIFT_RIGHT:              return Key::RightShift;
    case AKEYCODE_CTRL_LEFT:                return Key::LeftControl;
    case AKEYCODE_CTRL_RIGHT:               return Key::RightControl;
    case AKEYCODE_ALT_LEFT:                 return Key::LeftAlt;
    case AKEYCODE_ALT_RIGHT:                return Key::RightAlt;
    case AKEYCODE_SYSTEM_NAVIGATION_LEFT:   return Key::Left;
    case AKEYCODE_SYSTEM_NAVIGATION_RIGHT:  return Key::Right;
    case AKEYCODE_SYSTEM_NAVIGATION_UP:     return Key::Up;
    case AKEYCODE_SYSTEM_NAVIGATION_DOWN:   return Key::Down;
    case AKEYCODE_NUMPAD_ADD:               return Key::KeyPadAdd;
    case AKEYCODE_NUMPAD_SUBTRACT:          return Key::KeyPadSubtract;
    case AKEYCODE_NUMPAD_MULTIPLY:          return Key::KeyPadMultiply;
    case AKEYCODE_NUMPAD_DIVIDE:            return Key::KeyPadDivide;
  
    default:              return Key::Unknown;
    }
}

ModifierMask KeyConverter::GetModifiersByNativeCode([[maybe_unused]] const NativeKey& native_key) noexcept
{
    META_FUNCTION_TASK();
    ModifierMask modifiers_mask;
    return modifiers_mask;
}

} // namespace Methane::Platform::Input::Keyboard