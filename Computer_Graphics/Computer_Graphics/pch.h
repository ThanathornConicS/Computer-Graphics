#pragma once
#include <Windows.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <memory>
#include <crtdbg.h>

#include <string>
#include <sstream>
#include <cassert>
#include <cmath>
#include <fstream>

#include <vector>

#include "Log.h"

#include "src/KeyCode.h"

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC //to get more details
	#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#else
	#define _CRTDBG_MAP_ALLOC //to get more details
	#define DBG_NEW
#endif // DEBUG

#define APPNAME "Computer_Graphics"
#define MAX_SCENE 20

#define PI 2 * acos(0.0f)


const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;
