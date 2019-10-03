#pragma once

#include <random>
#include <iostream>
#include <vector>
#include <chrono>

// GLM
#include <glm/glm.hpp>
#include "glm/ext.hpp"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Application.h"
#include "GLFWApplication.h"
#include "PolyMesh.h"
#include "Texture2D.h"
#include "Material.h"
#include "Phong.h"
#include "Model.h"
#include "Scene.h"
#include "Rasterizer/Rasterizer.h"
#include "Raytracer/Raytracer.h"

#include "SimplexNoise.h"
#include "PerlinNoise.h"


int main(int argc, char *argv[])
{
	srand(time(NULL));
	/*
		CREATE APP CONTAINER
	*/
	GLFWApplication app = GLFWApplication::GLFWApplication("GLFW engine test", 800, 600);
	app.init();


	/*
		CREATE SCENE
	*/
	//PolyMesh mesh = PolyMesh::PolyMesh(vertices, 6, 3); // create model from list of vertices and associated UVs
	PolyMesh mesh = PolyMesh::PolyMesh(100.0f, 100.0f, 9, PolyMesh::DesertMountain);
	// material
	Phong phong = Phong::Phong(glm::vec3(.2f,.2f,.2f), glm::vec3(1.0f, .2f, .2f), glm::vec3(.7f, .7f, .7f), 12);
	// Model
	Model quad = Model::Model(&mesh, phong);

	// load texture
	Texture2D texture;
	texture.loadTexture(".//resources//textures//marbl01.jpg", true);
		
	// compute view matrix using glm::lookat
	glm::mat4  view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//app.setView(GLFWApplication::cam.GetViewMatrix());

	// projection matrix
	glm::mat4 projection = glm::perspective(45.0f, 1.0f * app.getWidth() / app.getHeight(), 0.001f, 200.0f);
	app.setProjection(projection);

	Scene scene = Scene::Scene();

	/*
	CREATE RENDERER
	*/
	Rasterizer renderer = Rasterizer();
	
	//Raytracer renderer = Raytracer::Raytracer();

	GLfloat firstFrame = (GLfloat)glfwGetTime();
	GLfloat lastFrame = firstFrame;

	// game loop
	while (app.shouldRun())
	{
		// show frame rate
		app.showFPS();

		// Set frame time
		GLfloat currentFrame = (GLfloat)glfwGetTime() - firstFrame;
		// the animation can be sped up or slowed down by multiplying currentFrame by a factor.
		currentFrame *= 1.5f;
		float dt = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// manage interaction
		app.pollEvents();
		app.HandleKeyPresses(dt);
		app.setView(GLFWApplication::cam.GetViewMatrix());


		// Regenerate terrain
		if (GLFWApplication::m_keys[GLFW_KEY_1] && !GLFWApplication::m_oldKeys[GLFW_KEY_1])
		{
			mesh = PolyMesh::PolyMesh(100.0f, 100.0f, 9, PolyMesh::DesertMountain);
		}
		else if (GLFWApplication::m_keys[GLFW_KEY_2] && !GLFWApplication::m_oldKeys[GLFW_KEY_2])
		{
			mesh = PolyMesh::PolyMesh(100.0f, 100.0f, 9, PolyMesh::Volcanic);
		}
		else if (GLFWApplication::m_keys[GLFW_KEY_3] && !GLFWApplication::m_oldKeys[GLFW_KEY_3])
		{
			mesh = PolyMesh::PolyMesh(100.0f, 100.0f, 9, PolyMesh::Highlands);
		}

		// Change terrain lighting
		if (GLFWApplication::m_keys[GLFW_KEY_SPACE] && !GLFWApplication::m_oldKeys[GLFW_KEY_SPACE])
		{
			renderer.lighting = !renderer.lighting;
		}
		/*
		**	RENDER
		*/
		
		// clear buffer
		app.clear();

		// draw geometry to buffer
		renderer.draw(quad, app); // works for rasterizer
		//renderer.draw(scene); // works for raytracer

		// display image buffer
		app.display();

	}

	app.terminate();
	mesh.deleteMesh();

	return 0;
}

