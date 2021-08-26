// License (OLC - 3)
// ~~~~~~~~~~~~~~~~~
// 
// Copyright 2018 - 2020 OneLoneCoder.com
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions or derivations of source code must retain the above
// copyright notice, this list of conditions and the following disclaimer.
// 
// 2. Redistributions or derivative works in binary form must reproduce
// the above copyright notice. This list of conditions and the following
// disclaimer must be reproduced in the documentation and/or other
// materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived
// from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "Application.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

/// <summary>
/// Mandelbrot.
/// </summary>
/// <returns>1 for successful exit.</returns>
int WINAPI wWinMain(_In_ HINSTANCE t_instance, _In_opt_ HINSTANCE, _In_ PWSTR t_cmdLine, _In_ int t_cmdShow)
{
	MessageBoxW(NULL, L"KEYS:\n\nPress '1' for no multi-threading.\nPress '2' for multi-threading using thread pooling (set by default).\n\nUse up and down arrow keys to increase/decrease iterations.\nMore iterations mean more detail, but also increases CPU usage.\n\nUse 'Q' and 'A' keys to zoom in and out.\n\nMove around by holding mouse right button and moving mouse.\n\nPress 'ESC' to exit (or you could just close the app window).\n\nIf you keep zooming in, eventually the fractal values will become so small that the algorithm will go crazy and display unsatisfactory results. This is not my fault - it's because computers are bad at representing miniscule values.", L"Mandelbrot", MB_OK | MB_ICONASTERISK);

	Application &f_app = Application();
	f_app.run();

	return 1;
}