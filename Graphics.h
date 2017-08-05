#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "Input.h"
#include "MainMenu.h"
#include "Gotham.h"
#include "GameTimer.h"
#define screenWidth 1600
#define screenHeight 900
#define ReleaseCOM(x) { if(x){ x->Release();x = 0; } }

extern GameTimer g_TickTock;

namespace D3D {

	LRESULT CALLBACK MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	const D3DCOLOR       BLACK(D3DCOLOR_XRGB(0, 0, 0));
	const D3DCOLOR       WHITE(D3DCOLOR_XRGB(255, 255, 255));
	const D3DXCOLOR        RED(D3DCOLOR_XRGB(255, 0, 0));
	const D3DXCOLOR      GREEN(D3DCOLOR_XRGB(0, 255, 0));
	const D3DXCOLOR       BLUE(D3DCOLOR_XRGB(0, 0, 255));
	const D3DXCOLOR     YELLOW(D3DCOLOR_XRGB(255, 255, 0));
	const D3DXCOLOR       CYAN(D3DCOLOR_XRGB(0, 255, 255));
	const D3DXCOLOR    MAGENTA(D3DCOLOR_XRGB(255, 0, 255));


	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);
	const D3DMATERIAL9 WHITE_MTRL = InitMtrl(WHITE, WHITE, WHITE, WHITE, 2.0f);
	const D3DMATERIAL9 RED_MTRL = InitMtrl(RED, RED, RED, RED, 2.0f);
	const D3DMATERIAL9 GREEN_MTRL = InitMtrl(GREEN, GREEN, GREEN, GREEN, 2.0f);
	const D3DMATERIAL9 BLUE_MTRL = InitMtrl(BLUE, BLUE, BLUE, BLUE, 2.0f);
	const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, YELLOW, 2.0f);
	const D3DMATERIAL9 BLACK_MTRL = InitMtrl(BLACK, BLACK, BLACK, BLACK, 2.0f);

	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
	D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
	D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);



	 //	lerp for terrain interpolation
	float Lerp(float a, float b, float t);

	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;

	//	vertex struct for environment objects
	struct Vertex_UV {
		Vertex_UV() {}
		Vertex_UV(float X, float Y, float Z, float U, float V) : x(X), y(Y), z(Z), u(U), v(V) {}
		float x, y, z, u, v;
	};

	struct VertexPNT
	{
		VertexPNT()
			:pos(0.0f, 0.0f, 0.0f),
			normal(0.0f, 0.0f, 0.0f),
			tex0(0.0f, 0.0f) {}
		VertexPNT(float x, float y, float z,
			float nx, float ny, float nz,
			float u, float v) :pos(x, y, z), normal(nx, ny, nz), tex0(u, v) {}
		VertexPNT(const D3DXVECTOR3& v, const D3DXVECTOR3& n, const D3DXVECTOR2& uv)
			:pos(v), normal(n), tex0(uv) {}

		D3DXVECTOR3 pos;
		D3DXVECTOR3 normal;
		D3DXVECTOR2 tex0;
		LPDIRECT3DDEVICE9 m_Device;

		static IDirect3DVertexDeclaration9* Decl;
	};

	void InitVertexPNT(LPDIRECT3DDEVICE9 Device);
	void DestroyVertexPNT();

	struct BoundingBox {
		ID3DXMesh* m_BoundingMesh;
		D3DXVECTOR3 MIN, MAX;

		BoundingBox() : MIN(INFINITY, INFINITY, INFINITY), MAX(-INFINITY, -INFINITY, -INFINITY) {}

		D3DXVECTOR3 center() {
			return 0.5f*(MIN + MAX);
		}

		bool isPointInside(D3DXVECTOR3 &p);
		void CreateBoundingMesh(LPDIRECT3DDEVICE9 Device);
		void Render(LPDIRECT3DDEVICE9 Device);
	};


	struct BoundingSphere {
		BoundingSphere();

		D3DXVECTOR3 _center;
		float _radius;
	};


	template<class T> void Release(T t)
	{
		if (t)
		{
			t->Release();
			t = 0;
		}
	}

	template<class T> void Delete(T t)
	{
		if (t)
		{
			delete t;
			t = 0;
		}
	}


}

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
	Input * m_Input;
	Sprite* sprt;
	Texture* tex;
	StateMachine* m_MM;
	StateMachine* m_Gotham;
public:
	Keyboard* m_Keyboard;
	Mouse* m_Mouse;
	Joystick* m_Joystick;
	Graphics();
	~Graphics();
	bool Initialized(int height, int width, HINSTANCE hInstance);
	void Shutdown();
	bool Frame();
	void Update();
	void RecvMessages(UINT msg, WPARAM wParam, LPARAM lParam, void * Data);
	bool Render();
	LPDIRECT3DDEVICE9 GetDevice() { return m_Device; }
	void GetProjectionMatrix(D3DXMATRIX& projMat) { projMat = m_projectionMatrix; }
	void GetWorldMatrix(D3DXMATRIX& worldMat) { worldMat = m_worldMatrix; }
	void GetOrthoMatrix(D3DXMATRIX& orthoMat) { orthoMat = m_orthoMatrix; }
};

