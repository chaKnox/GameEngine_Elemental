#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class Graphics
{
private:
	bool m_vsync_enabled;
	int m_viideoCardMemrory;
	char m_videoCardDexcription[128];
	LPDIRECT3DDEVICE9 m_Device;
    LPDIRECT3D9 m_D3DInterface;
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_orthoMatrix;

	bool Render();
public:
    Graphics();
    ~Graphics();
	bool Initialized(int height, int width, HWND hWnd);
	void Shutdown();
	bool Frame();

	void BeginScene(float, float, float, float);
	void EndScene();
	LPDIRECT3DDEVICE9 GetDevice() { return m_Device; }
	void GetProjectionMatrix(D3DXMATRIX& projMat) { projMat = m_projectionMatrix; }
	void GetWorldMatrix(D3DXMATRIX& worldMat) { worldMat = m_worldMatrix; }
	void GetOrthoMatrix(D3DXMATRIX& orthoMat) { orthoMat = m_orthoMatrix; }
};

