#include "csGlobals.h"
#include "csApp.h"
#include "csRenderer3D.h"
#include "glew/include/GL/glew.h"

#include "SDL\include\SDL_opengl.h"

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled)
{
	name = "Renderer3D";
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init(JSON_Object* node)
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);

	if (context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		LOG("Error initializing Glew: %s\n", glewGetErrorString(err));
		ret = false; 
	}

	if(ret == true)
	{
		
		if(App->GetVSYNC() && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", glewGetErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", glewGetErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", glewGetErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		depth_test = json_object_get_boolean(node, "Depth Test");
		cull_face = json_object_get_boolean(node, "Cull Face");
		lighting = json_object_get_boolean(node, "Lightning");
		line_smooth = json_object_get_boolean(node, "Line Smooth");
		polygon_smooth = json_object_get_boolean(node, "Polygon Smooth");
		color_material = json_object_get_boolean(node, "Color Material");
		texture_2D = json_object_get_boolean(node, "Texture 2D");
		wire_mode = json_object_get_boolean(node, "Wireframe");

		ambient_model_light = json_object_get_boolean(node, "Ambient Model");
		ambient_light = json_object_get_boolean(node, "Ambient");
		diffuse_light = json_object_get_boolean(node, "Diffuse");
		specular_light = json_object_get_boolean(node, "Specular");
		fog = json_object_get_boolean(node, "Fog");

		(depth_test) ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
		(cull_face) ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
		(line_smooth) ? glEnable(GL_LINE_SMOOTH) : glDisable(GL_LINE_SMOOTH);
		(polygon_smooth) ? glEnable(GL_POLYGON_SMOOTH) : glDisable(GL_POLYGON_SMOOTH);
		lights[0].Active(true);
		(lighting) ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
		(color_material) ? glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);
		(texture_2D) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
		(fog) ? glEnable(GL_FOG) : glDisable(GL_FOG);

		
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);


	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();
	
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}




void ModuleRenderer3D::SetDepthTest(const bool& depth) const
{
	if (depth)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void ModuleRenderer3D::SetCullFace(const bool& cullFace) const
{
	if (cullFace)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}


void ModuleRenderer3D::SetLighting(const bool& light) const
{
	if (light)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
}

void ModuleRenderer3D::SetTexture2D(const bool& tex2D) const
{
	if (tex2D)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
}

void ModuleRenderer3D::SetColorMaterial(const bool& colorMaterial) const
{
	if (colorMaterial)
		glEnable(GL_COLOR_MATERIAL);
	else
		glDisable(GL_COLOR_MATERIAL);
}

void ModuleRenderer3D::SetSmoothLine(const bool& lSmooth) const
{
	if (lSmooth)
		glEnable(GL_LINE_SMOOTH);

	else
		glDisable(GL_LINE_SMOOTH);
}

void ModuleRenderer3D::SetSmoothPolygon(const bool& pSmooth) const
{
	if (pSmooth)
		glEnable(GL_POLYGON_SMOOTH);

	else
		glDisable(GL_POLYGON_SMOOTH);
}

void ModuleRenderer3D::SetFog(const bool& fog) const
{
	if (fog)
		glEnable(GL_FOG);

	else
		glDisable(GL_FOG);
}

void ModuleRenderer3D::SetWireframe(const bool& wire) const
{
	if (wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


void ModuleRenderer3D::SetLightModelAmbient(const bool & ambient, const float color[4])
{
	GLfloat LightModelAmbient[] = { color[0], color[1], color[2], color[3] };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
}


void ModuleRenderer3D::SetAmbientLight(const bool & ambient, const float color[4]) const
{
	GLfloat LightModelAmbient[] = { color[0], color[1], color[2], color[3] };
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightModelAmbient);
}

void ModuleRenderer3D::SetDiffuseLight(const bool & diffuse, const float color[4]) const
{
	
	const GLfloat LightDiffuse[] = { color[0], color[1], color[2], color[3] };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);

}


void ModuleRenderer3D::SetSpecularLight(const bool & specular, const float color[4]) const
{
	const GLfloat LightSpecular[] = { color[0], color[1], color[2], color[3] };
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);

}

bool ModuleRenderer3D::GetDepthTest() const
{
	return depth_test;
}

bool ModuleRenderer3D::GetCullFace() const
{
	return cull_face;
}

bool ModuleRenderer3D::GetLighting() const
{
	return lighting;
}

bool ModuleRenderer3D::GetSmoothPolygon() const
{
	return polygon_smooth;
}

bool ModuleRenderer3D::GetSmoothLine() const
{
	return line_smooth;
}

bool ModuleRenderer3D::GetTexture2D() const
{
	return texture_2D;
}

bool ModuleRenderer3D::GetColorMaterial() const
{
	return color_material;
}

bool ModuleRenderer3D::GetFog() const
{
	return fog;
}

bool ModuleRenderer3D::GetWireframe() const
{
	return wire_mode;
}

bool ModuleRenderer3D::GetLightModelAmbient() const
{
	return ambient_model_light;
}

bool ModuleRenderer3D::GetAmbientLight() const
{
	return ambient_light;
}

bool ModuleRenderer3D::GetDiffuseLight() const
{
	return diffuse_light;
}

bool ModuleRenderer3D::GetSpecularLight() const
{
	return specular_light;
}

void ModuleRenderer3D::ActiveLights(bool& enable)
{
	if (enable)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);
}
