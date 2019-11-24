#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 ) // Warning that exceptions are disabled

#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)
#define PI 3.14159265
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef unsigned int uint;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
typedef unsigned char uchar;
typedef unsigned long long UID;
// Deletes a buffer
#define RELEASE( x ) \
    {                        \
    if( x != NULL )        \
	    {                      \
      delete x;            \
	  x = NULL;              \
	    }                      \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
    {                              \
    if( x != NULL )              \
	    {                            \
      delete[] x;                \
	  x = NULL;                    \
	    }                            \
                              \
}

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 1024
#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "Chrono Stasis Engine"
#define ORGANIZATION "ChronoByte"

// Files PATHS ---------------
#define PATHS_AMOUNT 6
#define ASSETS_FOLDER "Assets/"
#define LIBRARY_FOLDER "Library/"
#define SETTINGS_FOLDER "Settings/"

#define A_MODELS_FOLDER "Assets/Models/"
#define A_SCENES_FOLDER "Assets/Scenes/"
#define A_TEXTURES_FOLDER "Assets/Textures/"

#define L_MESHES_FOLDER "Library/Meshes/"
#define L_TEXTURES_FOLDER "Library/Textures/"
#define L_SCENES_FOLDER "Library/Scenes/"

// Own format files --------------
#define MESH_EXTENSION ".mesh"
#define TEXTURES_EXTENSION ".dds"
#define SCENES_EXTENSION ".scene.json"
#define META_EXTENSION ".meta.json"