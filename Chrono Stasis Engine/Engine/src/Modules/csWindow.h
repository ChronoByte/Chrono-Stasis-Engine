#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "csModule.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public: // CORE METHODS

	ModuleWindow(bool start_enabled = true);
	virtual ~ModuleWindow();

	bool Init(JSON_Object* node);
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

	SDL_Surface* iconEngine = nullptr;

	//------TODO: JSON LOADER-------//
	int width;
	int height;

	uint size = SCREEN_SIZE;

	int screen_w = SCREEN_WIDTH;
	int screen_h = SCREEN_HEIGHT;

	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fullscreen_desktop = false;

	bool maximized = false;

	int brightness = 100;

	
};

#endif // __ModuleWindow_H__