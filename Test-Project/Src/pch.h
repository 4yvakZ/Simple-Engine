#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX 

#include <wrl.h>
#include <windows.h>

#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include <vector>
#include <string>

#include <iostream>
#include <chrono>
#include <mutex>

#include "SimpleMath.h"

#if defined(DEBUG) || defined(_DEBUG)
#include <cassert>
#endif
