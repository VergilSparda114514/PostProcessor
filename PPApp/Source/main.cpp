#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/UI/UI.h"

#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <glm/glm.hpp>

static float Luminance(const glm::vec3& color)
{
	return 0.2126f * color.r + 0.7152f * color.g + 0.0722f * color.b;
}

class Editor : public Walnut::Layer
{
public:
	Editor()
	{
		int width, height, channels;
		unsigned char* data = stbi_load("Resource/Images/texture.jpg", &width, &height, &channels, STBI_rgb_alpha);

		m_Data.insert(m_Data.begin(), data, data + width * height * 4);

		stbi_image_free(data);

		m_Image = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	virtual void OnUIRender() override
	{
		std::vector<uint8_t> data = m_Data;

		for (size_t i = 0; i < data.size(); i += 4)
		{
			uint8_t r = data[i + 0];
			uint8_t g = data[i + 1];
			uint8_t b = data[i + 2];

			glm::vec3 color = { r / 255.0f, g / 255.0f, b / 255.0f };
			float luminance = Luminance(color);

			data[i + 0] = glm::step(min, luminance) * glm::step(luminance, max) * 255;
			data[i + 1] = glm::step(min, luminance) * glm::step(luminance, max) * 255;
			data[i + 2] = glm::step(min, luminance) * glm::step(luminance, max) * 255;
			data[i + 3] = 255;
		}

		m_Image->SetData(data.data());

		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
			ImGui::Begin("Viewport");

			uint32_t width = ImGui::GetContentRegionAvail().x;
			uint32_t height = ImGui::GetContentRegionAvail().y;

			width = std::min(width, m_Image->GetWidth());
			height = std::min(height, m_Image->GetHeight());

			ImGui::Image(m_Image->GetDescriptorSet(), { (float)width, (float)height });

			ImGui::End();
			ImGui::PopStyleVar();
		}

		{
			ImGui::Begin("Inspector");

			ImGui::SliderFloat("Min", &min, 0.0f, 1.0f);
			ImGui::SliderFloat("Max", &max, 0.0f, 1.0f);

			ImGui::End();
		}
	}

	void Save()
	{
		stbi_write_png("Resource/Images/texture_out.png", m_Image->GetWidth(), m_Image->GetHeight(), 4, m_Data.data(), m_Image->GetWidth() * 4);
	}
private:
	std::shared_ptr<Walnut::Image> m_Image;
	std::vector<uint8_t> m_Data{};

	float min = 0.0f;
	float max = 1.0f;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Post Processor";
	spec.CustomTitlebar = true;

	Walnut::Application* app = new Walnut::Application(spec);

	std::shared_ptr<Editor> editor = std::make_shared<Editor>();
	app->PushLayer(editor);
	app->SetMenubarCallback([app, editor]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				editor->Save();
			}

			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}

			ImGui::EndMenu();
		}
	});

	return app;
}