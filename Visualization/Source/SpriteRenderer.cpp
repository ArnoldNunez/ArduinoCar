#include "..\Header\SpriteRenderer.h"


ArduinoCar_Visualization::SpriteRenderer::SpriteRenderer(Shader& shader)
{
	this->mShader = shader;
	this->initRenderData();
}

ArduinoCar_Visualization::SpriteRenderer::~SpriteRenderer()
{
}

void ArduinoCar_Visualization::SpriteRenderer::DrawSprite(Texture2D& texture, glm::vec2 position,
	glm::vec2 size, float rotate, glm::vec3 color)
{
	/**
	 * The convention is to first scale, then rotate, and finally translate the objects. But
	 * the matrix multiplication is done in reverse order so therefore we translate, rotate, and 
	 * then scale the object.
	 */

	// Prepare transformations
	this->mShader.Use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));	// Translate to position

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));	// Translate half with and height
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));	// To rotate around center.
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));	// Undo initial translation

	model = glm::scale(model, glm::vec3(size, 1.0f));	// Apply any scaling to the sprite

	// Set the shader uniforms
	this->mShader.SetMatrix4("model", model);
	this->mShader.SetVector3f("spriteColor", color);

	// Draw texture
	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}



void ArduinoCar_Visualization::SpriteRenderer::initRenderData()
{
	// Configure VAO/VBO
	unsigned int VBO;
	float vertices[] =
	{
		// pos      // tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
