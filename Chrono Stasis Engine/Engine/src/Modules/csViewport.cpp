#include "csViewport.h"
#include "glew/include/GL/glew.h"
#include "csApp.h"
#include "ComponentCamera.h"
#include "Dependencies/glm/glmath.h"

Viewport::Viewport()
{
}

Viewport::Viewport(const uint & width, const uint & height, ComponentCamera * camera) 
	: width(width), height(height), camera(camera)
{
}

Viewport::~Viewport()
{
	camera = nullptr; 

	// Deletes buffers
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteRenderbuffers(1, &depthBuffer);
	glDeleteTextures(1, &renderTexture);
	glDeleteTextures(1, &zBufferTexture);

}

void Viewport::SetCamera(ComponentCamera * camera)
{
	this->camera = camera; 
}

void Viewport::SetSize(const uint & width, const uint & height)
{
	if(camera != nullptr)
		camera->SetAspectRatio((float)width / (float)height);

	this->width = width;
	this->height = height; 

	CreateBuffers(); 
}

void Viewport::CreateBuffers()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Clean what's already created
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteRenderbuffers(1, &depthBuffer);
	glDeleteTextures(1, &renderTexture);
	glDeleteTextures(1, &zBufferTexture);

	// Create Frame Buffer
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	// Create Render Texture 
	glGenTextures(1, &renderTexture);
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	// Create ZBuffer Texture 
	glGenTextures(1, &zBufferTexture);
	glBindTexture(GL_TEXTURE_2D, zBufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	// Create Depth buffer 
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	// Attach both textures to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, zBufferTexture, 0);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG("ERROR");
	}

	// Unbind everything
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Viewport::SetView()
{
	// Bind buffers so it futurely renders on it

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glViewport(0, 0, width, height);

	// Load Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	mat4x4 ProjectionMatrix = perspective(camera->GetVerticalFOV() * RADTODEG,
		(float)width / (float)height,
		camera->GetNearPlaneDistance(),
		camera->GetFarPlaneDistance());

	//glLoadMatrixf(&ProjectionMatrix);
	glLoadMatrixf(camera->GetProjectionMatrix().ptr());

	// Load Model View Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((GLfloat*)&camera->GetViewMatrix());

	// Clear screen with bg color
	const float* color = camera->GetColor();
	glClearColor(color[0], color[1], color[2], color[3]);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

}

void Viewport::CreateTextures()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, zBufferTexture);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}
