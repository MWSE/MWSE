// The MIT License (MIT)

// Copyright (c) 2013-2020 Rapptz, ThePhD and contributors

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// This file was generated with a script.
// Generated 2026-01-23 23:28:11.759874 UTC
// This header was generated with sol v3.5.0 (revision 2e3103c9)
// https://github.com/ThePhD/sol2

#ifndef SOL_SINGLE_SOL_CONFIG_HPP
#define SOL_SINGLE_SOL_CONFIG_HPP

// beginning of sol/config.hpp

// Enable LUAJIT support
#define SOL_LUAJIT 1

// Ensure that userdata and functions have their param types checked.
#define SOL_SAFE_USERTYPE 1

// Opt out of floating point/integer precision checks.
#define SOL_NO_CHECK_NUMBER_PRECISION 1

// It's _not_ wanted to propagate exceptions through Lua
#define SOL_EXCEPTIONS_SAFE_PROPAGATION 0

// end of sol/config.hpp

#endif // SOL_SINGLE_SOL_CONFIG_HPP
