#pragma once
#ifndef UIWRAPPERS_H_
#define UIWRAPPERS_H_
#include <d3d9.h>
#include <d3dx9.h>

template <class T> 
void SafeRelease(T*& pT)
{
    if( pT )
    {
        pT->Release();
        pT = NULL;
    }
}

class Surface
{
protected:
    LPDIRECT3DSURFACE9 m_Surface;
    Surface* m_BackBuffer;
    LPDIRECT3DDEVICE9 m_pDevice;
public:
    Surface(LPDIRECT3DDEVICE9 pDevice);
    ~Surface();
    LPDIRECT3DSURFACE9 GetSurface() const{ return m_Surface; }
    void SetSurface(LPDIRECT3DSURFACE9 Surf) { m_Surface = Surf; }
    LPDIRECT3DDEVICE9 GetDevice()const { return m_pDevice; }
    void SetDevice(LPDIRECT3DDEVICE9 pDevice) { m_pDevice = pDevice; }
    HRESULT CreateSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool);
    HRESULT LoadFromFile(LPSTR Path);
    HRESULT MakeBackBuffer(void);
    HRESULT UpdateSurface(Surface* Source, RECT* pSourceRect, int x, int y);
    void Render(void);
};

class Texture
{
protected:
    LPDIRECT3DTEXTURE9 m_Texture;
    LPDIRECT3DDEVICE9 m_pDevice;
    D3DXVECTOR2 m_RotationCenter;
    D3DXVECTOR2 m_Translation;
    D3DXVECTOR2 m_Scaling;
    FLOAT m_Rotation;
    RECT m_SrcRect;
public:
    Texture(LPDIRECT3DDEVICE9 pDevice);
    Texture(LPDIRECT3DDEVICE9 pDevice, LPSTR Path,  D3DXVECTOR2 RotationCenter, FLOAT Rotation, D3DXVECTOR2 Translation, D3DXVECTOR2 Scaling, RECT SrcRect);
    void InitTexture( D3DXVECTOR2 RotationCenter, FLOAT Rotation, D3DXVECTOR2 Translation, D3DXVECTOR2 Scaling, RECT SrcRect);
    ~Texture();
    LPDIRECT3DTEXTURE9 GetTexture()const { return m_Texture; }
    void SetTexture(LPDIRECT3DTEXTURE9 Texture) { m_Texture = Texture; }
    LPDIRECT3DDEVICE9 GetDevice() const { return m_pDevice; }
    void SetDevice(LPDIRECT3DDEVICE9 pDevice) { m_pDevice = pDevice; }
    D3DXVECTOR2 GetRotationCenter() const { return m_RotationCenter; }
    void SetRotationCenter(D3DXVECTOR2 RotationCenter) { m_RotationCenter = RotationCenter; }
    D3DXVECTOR2 GetTranslation() const { return m_Translation; }
    void SetTranslation(D3DXVECTOR2 Translation) { m_Translation = Translation; }
    D3DXVECTOR2 GetScaling() { return m_Scaling; }
    void SetScaling(D3DXVECTOR2 Scaling) { m_Scaling = Scaling; }
    FLOAT GetRotation()const { return m_Rotation; }
    void SetRotation(FLOAT Rotation) { m_Rotation = Rotation; }
    RECT GetRect()const { return m_SrcRect; }
    void SetRect(RECT SrcRect) { m_SrcRect = SrcRect; }
    HRESULT LoadFromFile(LPSTR Path);
};

class Sprite
{
protected:
    LPD3DXSPRITE m_Sprite;
    LPDIRECT3DDEVICE9 m_pDevice;
public:
    Sprite(LPDIRECT3DDEVICE9 pDevice);
    ~Sprite();
    LPD3DXSPRITE GetSprite() const { return m_Sprite; }
    void SetSprite(LPD3DXSPRITE Sprite) { m_Sprite = Sprite; }
    LPDIRECT3DDEVICE9 GetDevice()const { return m_pDevice; }
    void SetDevice(LPDIRECT3DDEVICE9 pDevice) { m_pDevice = pDevice; }
    HRESULT DrawTexture(Texture* Tex);
};
#endif // !UIWRAPPERS_H_
