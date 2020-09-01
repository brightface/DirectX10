#include "stdafx.h"
#include "AnimationDemo.h"

void AnimationDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(13, 2, 0);
	Context::Get()->GetCamera()->Position(29, 4, -12);

	shader = new Shader(L"21_Animation.fx");

	Kachujin();
	Police();
}

void AnimationDemo::Update()
{
	static int clip = 0;
	static float speed = 1.0f;
	static float takeTime = 1.0f;

	static bool bBlendMode = false;
	static float blendAlpha = 0.0f;

	ImGui::Checkbox("BlendMode", &bBlendMode);
	if (bBlendMode == false)
	{
		ImGui::InputInt("Clip", &clip);
		clip %= 5;

		ImGui::SliderFloat("Speed", &speed, 0.1f, 5.0f);
		ImGui::SliderFloat("TakeTime", &takeTime, 0.1f, 5.0f);

		if (ImGui::Button("Apply"))
			kachujin->PlayTweenMode(clip, speed, takeTime);
	}
	else
	{
		ImGui::SliderFloat("Alpha", &blendAlpha, 0.0f, 2.0f);
		kachujin->SetBlendAlpha(blendAlpha);

		if (ImGui::Button("Apply"))
			kachujin->PlayBlendMode(0, 1, 2);
	}



	ImGui::SliderFloat3("Direction2", direction, -1, +1);
	shader->AsVector("LightDirection")->SetFloatVector(direction);

	if (kachujin != NULL)
		kachujin->Update();
	if (police != NULL)
		police->Update();
}

void AnimationDemo::Render()
{
	if (kachujin != NULL)
	{
		static int pass = 0;
		ImGui::InputInt("Animation Pass", &pass);
		pass %= 2;
		kachujin->Pass(pass);
		//업데이트 후에 랜더링 되잖아.
		

		Matrix bones[MAX_MODEL_TRANSFORMS];
		//40번이었나? 그럴거야.
		kachujin->GetAttachBones(bones);
		colliderObject->Transform->World(bones[37]);
		//안나오면 결과를 살펴봐야지 
		//input도 정상적으로 들어갔나 확인.
		// output 리턴이 되었다는 이야기인데. world_11=1.0f 쉐이더에 써서.
		
		//계산이 잘못되었다는 소리네 그러면
		//쉐이더는 디버깅이 불편해.

		//쉐이더 월드를 넘겨주는게 없네.
		//모델 애니메이터로 가.//얘를 통해서 쉐이더로 넘어가는거잖아.
		//트랜스 폼에 있는 월드로 밀어주면 되겠지.

		//computeShader로 들어가야지 Shader 가 아니라

		colliderObject->Collider->Render();//콜라이더를 랜더링 하면 이제 보일거야.

		kachujin->Render();
		police->Render();
	}
}

void AnimationDemo::Kachujin()
{
	kachujin = new ModelAnimator(shader);
	kachujin->ReadMesh(L"Kachujin/Mesh");
	kachujin->ReadMaterial(L"Kachujin/Mesh");
	kachujin->ReadClip(L"Kachujin/Idle");
	kachujin->ReadClip(L"Kachujin/Walk");
	kachujin->ReadClip(L"Kachujin/Run");
	kachujin->ReadClip(L"Kachujin/Slash");
	kachujin->ReadClip(L"Kachujin/HipHop");

	//트랜스폼으로부터 트랜스폼 카운터를 받는다.
	//UINT count = kachujin ->GetTransformCount();
	kachujin->GetTransform()->Position(30, 0, 0);
	kachujin->GetTransform()->Scale(0.025f, 0.025f, 0.025f);

	//weapon
	//무기를 받는다. 모델 랜더

	//collider 
	//충돌체를 만든다.
	colliderObject = new ColliderObject();
	colliderObject->Init = new Transform();
	//위치를 받아온다음에 얘한테 리턴할거야.
	colliderObject->Transform = new Transform();
	//뒤 매개변수는 언제든지 바꿔도 되니까. 참조로 움직이니까.
	colliderObject->Collider = new Collider(colliderObject->Transform, colliderObject->Init);

	colliderObject->Init->Position(-25, 10, -5.0f);
	colliderObject->Init->Scale(5, 5, 75);
	colliderObject->Init->Rotation(0, 30, 1);
}

void AnimationDemo::Police()
{
	police = new ModelAnimator(shader);
	police->ReadMesh(L"Kachujin/Mesh");
	police->ReadMaterial(L"Kachujin/Mesh");
	police->ReadClip(L"Kachujin/Idle");
	police->ReadClip(L"Kachujin/Walk");
	police->ReadClip(L"Kachujin/Run");
	police->ReadClip(L"Kachujin/Slash");
	police->ReadClip(L"Kachujin/HipHop");

	police->GetTransform()->Position(50, 0, 0);
	police->GetTransform()->Scale(0.025f, 0.025f, 0.025f);
	
}

void AnimationDemo::Enemy()
{
}
