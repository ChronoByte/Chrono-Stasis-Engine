#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "csModule.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public: // CORE METHODS

	ModuleWindow(Application* app, bool start_enabled = true);
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();


public: // CONFIGURATION METHODS

	void SetTitle(const char* title);

	void SetBrightness(float brightness);
	float GetBrightness() const;

	
	void SetWidth(uint width);
	void SetHeigth(uint height);
	uint GetWidth() const;
	uint GetHeight() const;

	void SetWindowSize(int width, int height);
	void GetWindowSize(int &width, int &height);

	void SetSize(uint size, int& new_w, int& new_h);
	uint GetSize() const;
	void GetDesktopDisplay(uint& width, uint& height) const;

	uint GetRefreshRate() const;

	void SetFullscreen(bool full);
	void SetResizable(bool resize);
	void SetBorderless(bool border);
	void SetFullScreenDesktop(bool desktop);
	bool GetFullscreenWindow() const;
	bool GetFullDesktopWindow() const;
	bool GetResizableWindow() const;
	bool GetBorderlessWindow() const;

	void SetMaximized(bool max);
	bool GetMaximized() const;

public:

	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

	//------TODO: JSON LOADER-------//
	int width = SCREEN_WIDTH * SCREEN_SIZE;
	int height = SCREEN_HEIGHT * SCREEN_SIZE;

	uint size = SCREEN_SIZE;

	int screen_w = SCREEN_WIDTH;
	int screen_h = SCREEN_HEIGHT;

	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fullscreen_desktop = false;

	bool maximized = false;
};

#endif // __ModuleWindow_H__