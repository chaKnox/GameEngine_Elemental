#include "UIWrappers.h"

Surface::Surface(LPDIRECT3DDEVICE9 pDevice)
{
    this->SetDevice(pDevice);
    this->SetSurface(NULL);
    m_BackBuffer = new Surface(pDevice);
}

Surface::~Surface()
{
    delete m_BackBuffer;
    if (m_Surface != NULL)
        m_Surface->Release();
}

HRESULT Surface::CreateSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool)
{
    return m_pDevice->CreateOffscreenPlainSurface(Width, Height, Format, Pool, &m_Surface, NULL);
}

HRESULT Surface::LoadFromFile(LPSTR Path)
{
    HRESULT Result = E_FAIL;
    D3DXIMAGE_INFO Info;
    ZeroMemory(&Info, sizeof(D3DXIMAGE_INFO));

    if (SUCCEEDED(D3DXGetImageInfoFromFile(Path, &Info)))
    {
        Result = CreateSurface(Info.Width, Info.Height, Info.Format, D3DPOOL_SYSTEMMEM);
    }
    if (Result == S_OK)
    {
        Result = D3DXLoadSurfaceFromFile(m_Surface, NULL, NULL, Path, NULL, D3DX_FILTER_NONE, 0, NULL);
    }
    else
        Result = E_FAIL;
    return Result;
}

HRESULT Surface::MakeBackBuffer(void)
{
    if (m_pDevice)
    {
        SafeRelease(m_BackBuffer->m_Surface);
        return m_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_BackBuffer->m_Surface);
    }
    return E_NOTIMPL;
}

HRESULT Surface::UpdateSurface(Surface * Source, RECT * pSourceRect, int x, int y)
{
    if ((m_pDevice) && (Source))
    {
        POINT Point;
        Point.x = x;
        Point.y = y;

        return m_pDevice->UpdateSurface(Source->GetSurface(), pSourceRect, m_Surface, NULL);
    }
    else 
        return E_FAIL;
}

void Surface::Render(void)
{
    m_BackBuffer->UpdateSurface(this, NULL, 0, 0);
}
///---------------------------------------------------------------------------------Texture wrapper-----------------------------------------------------------------------
Texture::Texture(LPDIRECT3DDEVICE9 pDevice)
{
    D3DXVECTOR2 Vec;
    Vec.x = 0;
    Vec.y = 0;
    SetDevice(pDevice);
    SetTexture(NULL);
    SetRotation(0.0f);
    SetRotationCenter(Vec);
    SetScaling(Vec);
    SetTranslation(Vec);
}

Texture::Texture(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, D3DXVECTOR2 RotationCenter, FLOAT Rotation, D3DXVECTOR2 Translation, D3DXVECTOR2 Scaling, RECT SrcRect)
{
    SetDevice(pDevice);
    InitTexture( Texture,  RotationCenter,  Rotation,  Translation,  Scaling, SrcRect);
}

void Texture::InitTexture(LPDIRECT3DTEXTURE9 Texture, D3DXVECTOR2 RotationCenter, FLOAT Rotation, D3DXVECTOR2 Translation, D3DXVECTOR2 Scaling, RECT SrcRect)
{
    SetTexture(Texture);
    SetRotation(Rotation);
    SetRotationCenter(RotationCenter);
    SetScaling(Scaling);
    SetTranslation(Translation);
    SetRect(SrcRect);
}

Texture::~Texture()
{
    if (m_Texture != NULL)
        SafeRelease(m_Texture);
}

HRESULT Texture::LoadFromFile(LPSTR Path)
{
    if (m_Texture != NULL)
        m_Texture->Release();
    return D3DXCreateTextureFromFile(m_pDevice, Path, &m_Texture);
}

///----------------------------------------------------------------------Sprite Wrapper-------------------------------------------------------------------------------

Sprite::Sprite(LPDIRECT3DDEVICE9 pDevice)
{
    SetDevice(pDevice);
    D3DXCreateSprite(m_pDevice, &m_Sprite);
}

Sprite::~Sprite()
{
    if (m_Sprite != NULL)
        SafeRelease(m_Sprite);
}

HRESULT Sprite::DrawTexture(Texture* Tex)
{
    if ((Tex->GetTexture() != NULL) && (m_Sprite != NULL))
    {
        D3DXMATRIX Mat;
        D3DXMatrixTransformation2D(&Mat, NULL, 0, &Tex->GetScaling(), &Tex->GetRotationCenter(), Tex->GetRotation(), &Tex->GetTranslation());

        m_Sprite->Begin(0);
        m_Sprite->SetTransform(&Mat);
        HRESULT Result = m_Sprite->Draw(Tex->GetTexture(), &Tex->GetRect(), NULL, NULL, 0xFFFFFFFF);
        m_Sprite->End();
        return Result;
    }
    else
        return E_FAIL;
}
