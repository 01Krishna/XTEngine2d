#include "Renderer.h"

namespace XTEngine2d
{
	void Renderer::Init()
	{
		unsigned char whitePixel[] = { 255,255,255,255 };

		glGenTextures(1, &m_WhiteTexture);
		glBindTexture(GL_TEXTURE_2D, m_WhiteTexture);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			1,
			1,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			whitePixel
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	};

	void Renderer::BeginScene(const glm::mat4& vp)
	{
		m_ViewProjection = vp;
	}

	void Renderer::Draw(Shader& shader, Sprite& sprite, Transform& transform)
	{

		glBindVertexArray(QuadMesh::VAO);

		unsigned int texture = sprite.texture ? sprite.texture->GetId() : m_WhiteTexture;

		shader.use();
			
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);


		shader.setInt("ourTexture", 0);	
		shader.setVec4("uv_Rect", glm::value_ptr(sprite.uv));
		shader.setVec4("newColor", glm::value_ptr(sprite.color));
		shader.setMat4("viewProjection", glm::value_ptr(m_ViewProjection));

		glm::mat4 model = glm::mat4(1.0f);

		//model = glm::translate(model,glm::vec3(transform.position.x,transform.position.y,0.0f));

		//model = glm::translate(model,glm::vec3(transform.size.x * 0.5f,	transform.size.y * 0.5f,0.0f));

		//model = glm::rotate(model,glm::radians(transform.rotation.z),glm::vec3(0.0f, 0.0f, 1.0f));

		//model = glm::translate(model,glm::vec3(-transform.size.x * 0.5f,-transform.size.y * 0.5f,0.0f));

		//model = glm::scale(model,glm::vec3(transform.size.x,transform.size.y,1.0f));


		// Move to world position
		model = glm::translate(
			model,
			glm::vec3(
				transform.worldPosition.x,
				transform.worldPosition.y,
				0.0f));

		// Move pivot to center
		model = glm::translate(
			model,
			glm::vec3(
				transform.size.x * 0.5f,
				transform.size.y * 0.5f,
				0.0f));

		//// Rotate
		model = glm::rotate(
			model,
			glm::radians(transform.worldRotation),
			glm::vec3(0.0f, 0.0f, 1.0f));

		// Move pivot back
		model = glm::translate(
			model,
			glm::vec3(
				-transform.size.x * 0.5f,
				-transform.size.y * 0.5f,
				0.0f));

		// Scale LAST
		model = glm::scale(
			model,
			glm::vec3(
				transform.size.x,
				transform.size.y,
				1.0f));

		model = glm::scale(
			model,
			glm::vec3(
				transform.worldScale.x,
				transform.worldScale.y,
				1.0f));

		shader.setMat4("model", glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
};