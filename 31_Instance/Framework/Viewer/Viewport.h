#pragma once

class Viewport
{
public:
	Viewport(UINT width = 0, UINT height = 0, float x = 0, float y = 0, float minDepth = 0, float maxDepth = 1);
	~Viewport();

	void RSSetViewport();
	void Set(UINT width, UINT height, float x = 0, float y = 0, float minDepth = 0, float maxDepth = 1);

	UINT GetWidth() { return width; }
	UINT GetHeight() { return height; }

	void Project(Vector3* pOut, Vector3& source, Matrix& W, Matrix& V, Matrix& P);
	void Unproject(Vector3* pOut, Vector3& source, Matrix& W, Matrix& V, Matrix& P);

	void GetMouseRay(Vector3* pOutPosition, Vector3* pOutDirection, Matrix* world = NULL);

private:
	float x, y;
	UINT width, height;
	float minDepth, maxDepth;

	D3D11_VIEWPORT viewport;
};

class A {};