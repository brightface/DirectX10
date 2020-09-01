#include "stdafx.h"
#include "Execute.h"
#include "Graphics.h"

Execute::Execute()
{
	graphics = new Graphics();
	graphics->Initialize();
	graphics->CreateBackBuffer(static_cast<uint>(Settings::Get().GetWidth()), static_cast<uint>(Settings::Get().GetHeight()));

	/*
			  |      1
			  |
		--------------
			  |
		0	  |      2
	*/

	//Vertex Data
	{
		vertices = new VertexColor[3];
		vertices[0].position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
		vertices[0].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		vertices[1].position = D3DXVECTOR3(+0.5f, +0.5f, 0.0f);
		vertices[1].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		vertices[2].position = D3DXVECTOR3(+0.5f, -0.5f, 0.0f);
		vertices[2].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}

	//Vertex Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = sizeof(VertexColor) * 3;

		D3D11_SUBRESOURCE_DATA sub_data;
		ZeroMemory(&sub_data, sizeof(D3D11_SUBRESOURCE_DATA));
		sub_data.pSysMem = vertices;

		auto hr = graphics->GetDevice()->CreateBuffer(&desc, &sub_data, &vertex_buffer);
		assert(SUCCEEDED(hr));
	}

	//Vertex Shader
	{	
		auto hr = D3DX11CompileFromFileA //이걸로 함수를 써서 컴파일 할수있따. color.hlsl
		(
			"Color.hlsl",
			nullptr,
			nullptr,
			"VS",
			"vs_5_0", //버전
			0, //플레그
			0, // 플레그
			nullptr, //몰라도 돼
			&vs_blob,//블롭이 들어가
			nullptr, //에러메세지
			nullptr //쓸필요가 없어 앞에서 리턴받고 있어서
		); //이제 블롭이 만들어졌겟지
		assert(SUCCEEDED(hr));
		//확인을한다. 
		hr = graphics->GetDevice()->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), nullptr, &vertex_shader);
		//블롭정보(시작주소), 블롭사이즈, 몰라도 돼, 버택스 쉐이더,
		assert(SUCCEEDED(hr));
	}

	//Input Layout
	{ //확인하는 곳 정보가 다르면 터져버려 ㅇㅕ기까지가 ia단계
		D3D11_INPUT_ELEMENT_DESC layout_desc[]
		{//sementicname,0부터시작,Dxgi포멧rgb4바이트rgb,0번부터,바이트의 시작점, 인풋정보, 인스턴스 데이터(지금은 안쓰니 0)
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//12부터 시작한다. 여기는 플롯4였는데 갑자기 플롯 3네? 그냥 벡터 3로 넘긴다. 문제가 안생긴다. 
		};
		//여기까지 LAYout 마들어졌다.
		auto hr = graphics->GetDevice()->CreateInputLayout(layout_desc, 2, vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), &input_layout);
		assert(SUCCEEDED(hr));
	}
}

Execute::~Execute()
{
	SAFE_RELEASE(input_layout);
	//먼저 지워주고 나중에 지워준다.
	SAFE_RELEASE(vertex_shader);
	SAFE_RELEASE(vs_blob);

	SAFE_RELEASE(vertex_buffer);

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE(graphics);
}

void Execute::Update()
{
}

void Execute::Render()
{
	uint stride = sizeof(VertexColor);
	uint offset = 0;

	graphics->Begin();
	{
		//IA
		graphics->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
		graphics->GetDeviceContext()->IASetInputLayout(input_layout);
		graphics->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//VS
		graphics->GetDeviceContext()->VSSetShader(vertex_shader, nullptr, 0);
	}
	graphics->End();
}

//-0.5, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f

//       o

//0      0