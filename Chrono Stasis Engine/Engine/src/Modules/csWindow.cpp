#include "csGlobals.h"
#include "csApp.h"
#include "csWindow.h"

ModuleWindow::ModuleWindow(bool start_enabled) : Module(start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetBrightness(float brightness)
{
	SDL_SetWindowBrightness(window, brightness);
}

float ModuleWindow::GetBrightness() const
{
	return SDL_GetWindowBrightness(window);
}


void ModuleWindow::SetSize(uint size, int& new_w, int& new_h)
{
	new_w = screen_w * size;
	new_h = screen_h * size;
		
	this->size = size;
	
	SDL_SetWindowSize(window, new_w, new_h);

	if (!fullscreen)
		App->renderer3D->OnResize(new_w, new_h);
}

uint ModuleWindow::GetSize() const
{
	return size;
}

void ModuleWindow::SetWindowSize(int w, int h)
{
	if (w > 0 && h > 0)
	{
		width = w;
		height = h;
		SDL_SetWindowSize(window, width, height);

		if (!fullscreen)
			App->renderer3D->OnResize(width, height);
	}
}

void ModuleWindow::GetWindowSize(int & width, int & height)
{
	SDL_GetWindowSize(window, &width, &height);
}


void ModuleWindow::SetWidth(uint width)
{
	SDL_SetWindowSize(window, width, height);
	this->width = width;

	if (!fullscreen)
		App->renderer3D->OnResize(width, height);
}

uint ModuleWindow::GetWidth() const
{
	return width;
}

void ModuleWindow::SetHeigth(uint height)
{
	SDL_SetWindowSize(window, width, height);
	this->height = height;

	if (!fullscreen)
		App->renderer3D->OnResize(width, height);
}

uint ModuleWindow::GetHeight() const
{
	return height;
}


uint ModuleWindow::GetRefreshRate() const
{
	uint refresh = 0;

	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) == 0) refresh = dm.refresh_rate;
	else LOG("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());

	return refresh;
}

void ModuleWindow::GetDesktopDisplay(uint& width, uint& height) const
{
	uint refresh = 0;

	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) == 0) width = dm.w, height = dm.h;
	else LOG("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
}


void ModuleWindow::SetFullscreen(bool full)
{
	this->fullscreen = full;

	if (full) SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	else SDL_SetWindowFullscreen(window, 0);
}

bool ModuleWindow::GetFullscreenWindow() const
{
	return fullscreen;
}

void ModuleWindow::SetResizable(bool resize)
{
	this->resizable = resize;

	if (resize) SDL_SetWindowResizable(window, SDL_TRUE);
	else SDL_SetWindowResizable(window, SDL_FALSE);
}

bool ModuleWindow::GetResizableWindow() const
{
	return resizable;
}

void ModuleWindow::SetBorderless(bool border)
{
	if (!(fullscreen && fullscreen_desktop))
	{
		this->borderless = borderless;

		if (borderless) SDL_SetWindowBordered(window, SDL_FALSE);
		else SDL_SetWindowBordered(window, SDL_TRUE);
	}
}

bool ModuleWindow::GetBorderlessWindow() const
{
	return borderless;
}

void ModuleWindow::SetFullScreenDesktop(bool desktop)
{
		this->fullscreen_desktop = desktop;

		if (fullscreen_desktop) SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		else SDL_SetWindowFullscreen(window, 0);
				
}

bool ModuleWindow::GetFullDesktopWindow() const
{
	return fullscreen_desktop;
}

void ModuleWindow::SetMaximized(bool max)
{
	maximized = max;

	if (max) SDL_MaximizeWindow(window);
	else SDL_RestoreWindow(window);
}

bool ModuleWindow::GetMaximized() const
{
	return maximized;
}