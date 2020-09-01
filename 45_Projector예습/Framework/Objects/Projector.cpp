#include "Framework.h"
#include "Projector.h"

Projector::Projector(Shader * shader, wstring mapFile, UINT width, UINT height)
	: shader(shader), width(width), height(height)
{	//ī�޶� �������ž�..
	camera = new Fixity();
	//camera->Position(0, 0, -20);
	//projection = new Perspective((float)width, (float)height, 10, 100, Math::PI * 0.25f);
	
	//projection = new Perspective((float)width, (float)height, 10, 100, Math::PI * 0.25f);
	camera->Position(15, 30, 0);
	//������ �ٶ󺸵��� x�� 90�� �����ڴ�.
	camera->RotationDegree(90, 0, 0);
	//orthographic�� near far �ִ�.
	projection = new Orthographic((float)width, (float)height);
	//���鿡�� �ϰ������ �������Ҷ��� �θ��� �ȴ�. �������͸� ������ �������� �ϸ� �ȴ�. ��������ٰ� ���ؼ� ���� �����ָ� �ȴ�.
	map = new Texture(mapFile);
	buffer = new ConstantBuffer(&desc, sizeof(Desc));


	sMap = shader->AsSRV("ProjectorMap");
	sMap->SetResource(map->SRV());

	sBuffer = shader->AsConstantBuffer("CB_Projector");
}

Projector::~Projector()
{
	SafeDelete(camera);
	SafeDelete(projection);

	SafeDelete(map);
	SafeDelete(buffer);
}

void Projector::Update()
{
	Vector3 position;
	camera->Position(&position);

	ImGui::SliderFloat3("Position", position, -100, 100);
	camera->Position(position);

	ImGui::ColorEdit3("Color", desc.Color);
	//������Ʈ �۽���Ƽ��
	//Perspective
	{
		//static float width = this->width, height = this->height;
		//static float n = 1.0f, f = 100.0f;
		//static float fov = 0.25f;

		//ImGui::SliderFloat("Width", &width, 0, 100);
		//ImGui::SliderFloat("Height", &height, 0, 100);
		//ImGui::SliderFloat("Near", &n, 0, 200);
		//ImGui::SliderFloat("Far", &f, 0, 200);
		//ImGui::SliderFloat("Fov", &fov, 0, Math::PI * 2.0f);

		//((Perspective *)projection)->Set(width, height, n, f, Math::PI * fov);
	}

	//Orthographic
	{
		static float width = this->width, height = this->height;
		static float n = 1.0f, f = 100.0f;

		ImGui::SliderFloat("Width", &width, 0, 100);
		ImGui::SliderFloat("Height", &height, 0, 100);
		ImGui::SliderFloat("Near", &n, 0, 200);
		ImGui::SliderFloat("Far", &f, 0, 200);

		((Orthographic *)projection)->Set(width, height, n, f);
	}

	camera->GetMatrix(&desc.View);
	projection->GetMatrix(&desc.Projection);
}

//c������ ���� �������ִ°͹ۿ� ����.
void Projector::Render()
{
	buffer->Render();
	sBuffer->SetConstantBuffer(buffer->Buffer());
}
//���۸� �о��ִ� ���Ҹ� �Ұž�.