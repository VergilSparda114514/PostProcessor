#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/UI/UI.h"

class EditorLayer : public Walnut::Layer
{
public:
	EditorLayer()
	{
		m_Image = std::make_unique<Walnut::Image>("../PostApp/Resource/texture.jpg");
	}

	virtual void OnDetach() override
	{
		delete m_Image.release();
	}

	virtual void OnUIRender() override
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");

		ImGui::Image(m_Image->GetDescriptorSet(), { static_cast<float>(m_Image->GetWidth()), static_cast<float>(m_Image->GetHeight()) });

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("Composer");



		ImGui::End();
	}
private:
	std::unique_ptr<Walnut::Image> m_Image = nullptr;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Post Processor";
	spec.CustomTitlebar = true;
	spec.UseDockspace = true;
	spec.CenterWindow = true;

	Walnut::Application* app = new Walnut::Application(spec);
	std::shared_ptr<EditorLayer> editorLayer = std::make_shared<EditorLayer>();
	app->PushLayer(editorLayer);
	app->SetMenubarCallback([app, editorLayer]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});

	return app;
}