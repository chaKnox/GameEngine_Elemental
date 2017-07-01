#include "Terrain.h"
#include <fstream>
#include <cmath>




bool Terrain::ReadRawFile(char * filename)
{
	//note: RAW file dimensions must be >= the dimensions of the terrain

	//height for each vertex
	std::vector<BYTE> l_In(m_NumVertices);
	std::ifstream l_InFile(filename, std::ios_base::binary);
	if (!l_InFile)
		MessageBox(NULL, "Terrain: Open Raw File: Failed", "Error", 0);
	l_InFile.read((char*)&l_In[0], l_In.size());
	l_InFile.close();
	m_Heightmap.resize(m_NumVertices);

	for (unsigned int i = 0; i < l_In.size(); ++i)
		m_Heightmap[i] = (float)l_In[i];
	return true;
}

bool Terrain::ComputeVertices()
{
	if (FAILED(m_Device->CreateVertexBuffer(m_NumVertices * sizeof(D3D::Vertex_UV),
		D3DUSAGE_WRITEONLY, D3D::FVF, D3DPOOL_MANAGED, &m_VertexBuffer, 0)))
	{
		MessageBox(NULL, "Terrain: CreateVB: Fail", "Error", MB_OK);
		return false;
	}
	//start generating coordinates at:
	int l_StartX = -m_Width / 2;
	int l_StartZ = m_Depth / 2;
	//end generating coordinates at:
	int l_EndX = m_Width / 2;
	int l_EndZ = m_Depth / 2;
	//compute texture increment size:
	float l_UCoordInc = 1.0f / (float)m_CellsPerRow;
	float l_VCoordInc = 1.0f / (float)m_CellsPerCol;

	D3D::Vertex_UV* l_VertexPointer = 0;
	m_VertexBuffer->Lock(0, 0, (void**)&l_VertexPointer, 0);
	int i = 0;
	for (int z = l_StartZ; z >= l_EndZ; z -= m_CellSpacing)
	{
		int j = 0;
		for (int x = l_StartX; x >= l_EndX; x -= m_CellSpacing)
		{
			//compute the index of the Vertex Buffer/heightmap based on location in nested loop
			int l_Index = i * m_VertsPerRow + j;

			l_VertexPointer[l_Index] = D3D::Vertex_UV((float)x, (float)m_Heightmap[l_Index], (float)z, (float)j*l_UCoordInc, (float)i*l_VCoordInc);
			++j;
		}//next column
		++i;
	}//next row
	m_VertexBuffer->Unlock();
	return true;
}

bool Terrain::ComputeIndices()
{
	if (FAILED(m_Device->CreateIndexBuffer(m_NumTriangles * 3 * sizeof(DWORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_IndexBuffer, 0)))
	{
		MessageBox(NULL, "Terrain: Create Index Buffer: Failed", "Error", 0);
		return false;
	}
	DWORD* l_Indeces = 0;
	m_IndexBuffer->Lock(0, 0, (void**)&l_Indeces, 0);
	//l_BaseIndex = start of a group of six indeces that describe the two triangles that make up the quad
	int l_BaseIndex = 0;
	//compute the triangles of each quad
	for (int i = 0; i < m_CellsPerCol; i++)
	{
		for (int j = 0; j < m_CellsPerRow; j++)
		{
			l_Indeces[l_BaseIndex] = i*m_VertsPerRow + j;
			l_Indeces[l_BaseIndex + 1] = i*m_VertsPerRow + j + 1;
			l_Indeces[l_BaseIndex + 2] = (i + 1)*m_VertsPerRow + j;

			l_Indeces[l_BaseIndex + 3] = (i + 1)*m_VertsPerRow + j;
			l_Indeces[l_BaseIndex + 4] = i*m_VertsPerRow + 1;
			l_Indeces[l_BaseIndex + 5] = (i + 1)*m_VertsPerRow + j + 1;
			//next quad
			l_BaseIndex += 6;
		}
	}
	m_IndexBuffer->Unlock();
	return true;
}

float Terrain::ComputeShade(int cellRow, int cellCol, D3DXVECTOR3 * directionToLight)
{
	//get the heights of 3 vertices on the quad
	float l_Height1 = (float)GetHeightmapIndex(cellRow, cellCol);
	float l_Height2 = (float)GetHeightmapIndex(cellRow, cellCol + 1);
	float l_Height3 = (float)GetHeightmapIndex(cellRow + 1, cellCol);

	// build two vectors on the quad
	D3DXVECTOR3 l_U((float)m_CellSpacing, (float)l_Height2 - (float)l_Height1, 0.0f);
	D3DXVECTOR3 l_V(0.0f, (float)l_Height3 - (float)l_Height1, -((float)m_CellSpacing));

	D3DXVECTOR3 l_Normal;
	D3DXVec3Cross(&l_Normal, &l_U, &l_V);
	D3DXVec3Normalize(&l_Normal, &l_Normal);

	float l_Cosine = D3DXVec3Dot(&l_Normal, directionToLight);

	if (l_Cosine < 0.0f)
		l_Cosine = 0.0f;
	return l_Cosine;
}

float Terrain::GetHeightmapIndex(int cellRow, int cellCol)
{
	unsigned int l_Index = cellRow*m_VertsPerRow + cellCol;
	if (l_Index >= 0 && l_Index < m_Heightmap.size())
	{
		return m_Heightmap[l_Index];
	}
	else
		return 0.0f;
}
bool Terrain::GenTexture(D3DXVECTOR3* directionToLight)
{
	// Method fills the top surface of a texture procedurally.  Then
	// lights the top surface.  Finally, it fills the other mipmap
	// surfaces based on the top surface data using D3DXFilterTexture.

	HRESULT hr = 0;

	// texel for each quad cell
	int l_TexWidth = m_CellsPerRow;
	int l_TexHeight = m_CellsPerCol;

	// create an empty texture
	if (FAILED(D3DXCreateTexture(
		m_Device,
		l_TexWidth, l_TexHeight,
		0, // create a complete mipmap chain
		0, // usage
		D3DFMT_X8R8G8B8,// 32 bit XRGB format
		D3DPOOL_MANAGED, &m_Texture)))
	{
		MessageBox(NULL, "Terrain: GenTexture()/CreateTexture: failed", "Error", 0);
		return false;
	}

	D3DSURFACE_DESC textureDesc;
	m_Texture->GetLevelDesc(0 /*level*/, &textureDesc);

	// make sure we got the requested format because our code
	// that fills the texture is hard coded to a 32 bit pixel depth.
	if (textureDesc.Format != D3DFMT_X8R8G8B8)
		return false;

	D3DLOCKED_RECT lockedRect;
	m_Texture->LockRect(0/*lock top surface*/, &lockedRect,
		0 /* lock entire tex*/, 0/*flags*/);

	DWORD* imageData = (DWORD*)lockedRect.pBits;
	for (int i = 0; i < l_TexHeight; i++)
	{
		for (int j = 0; j < l_TexWidth; j++)
		{
			D3DXCOLOR c;

			// get height of upper left vertex of quad.
			float height = (float)GetHeightmapIndex(i, j) / m_HeightScale;

			if ((height) < 42.5f) 		 c = D3D::RED;
			else if ((height) < 85.0f)	 c = D3D::YELLOW;
			else if ((height) < 127.5f) c = D3D::GREEN;
			else if ((height) < 170.0f) c = D3D::GREEN;
			else if ((height) < 212.5f) c = D3D::BLACK;
			else	                     c = D3D::WHITE;

			// fill locked data, note we divide the pitch by four because the
			// pitch is given in bytes and there are 4 bytes per DWORD.
			imageData[i * lockedRect.Pitch / 4 + j] = (D3DCOLOR)c;
		}
	}

	m_Texture->UnlockRect(0);

	if (!LightTerrain(directionToLight))
	{
		::MessageBox(0, "lightTerrain() - FAILED", 0, 0);
		return false;
	}

	hr = D3DXFilterTexture(
		m_Texture,
		0, // default palette
		0, // use top level as source level
		D3DX_DEFAULT); // default filter

	if (FAILED(hr))
	{
		::MessageBox(0, "D3DXFilterTexture() - FAILED", 0, 0);
		return false;
	}

	return true;
}

bool Terrain::LightTerrain(D3DXVECTOR3* directionToLight)
{
	D3DSURFACE_DESC l_TexDesc;
	m_Texture->GetLevelDesc(0, &l_TexDesc);
	//code that fills nexture needs 32 bit pixel depth
	if (l_TexDesc.Format != D3DFMT_X8R8G8B8)
	{
		MessageBox(NULL, "Light Terrain: Texture Desc Incompatible", "Error", 0);
		return false;
	}
	D3DLOCKED_RECT l_LockedRect;
	ZeroMemory(&l_LockedRect, sizeof(D3DLOCKED_RECT));
	m_Texture->LockRect(0, &l_LockedRect, 0, 0);
	DWORD*	l_ImageData = (DWORD*)l_LockedRect.pBits;
	for (unsigned int i = 0; i < l_TexDesc.Height; i++)
	{
		for (unsigned int j = 0; j < l_TexDesc.Width; j++)
		{
			//index into texture. 
			int l_Index = i*l_LockedRect.Pitch / 4 + j;
			//get current color of quad
			D3DXCOLOR l_Color(l_ImageData[l_Index]);

			float l_Shade = ComputeShade(i, j, directionToLight);
			l_ImageData[l_Index] = D3DXCOLOR(l_Shade, l_Shade, l_Shade, 1);
		}
	}
	return true;
}

bool Terrain::InBounds(int i, int j)
{
	return i >= 0 && i < (int)m_VertsPerCol &&
		j >= 0 && j < (int)m_VertsPerRow;
}

Terrain::Terrain(LPDIRECT3DDEVICE9 device, char * textureFile, char * heightmapFile, int numVertsPerRow, int numVertsPerCol, int cellSpacing, float heightScale)
{
	m_Device = device;
	//set variables
	m_VertsPerRow = numVertsPerRow;
	m_VertsPerCol = numVertsPerCol;
	m_CellSpacing = cellSpacing;
	m_HeightScale = heightScale;
	//calculated variables
	m_CellsPerRow = m_VertsPerRow - 1;
	m_CellsPerCol = m_VertsPerCol - 1;
	m_Width = m_CellsPerRow*m_CellSpacing;
	m_Depth = m_CellsPerCol*m_CellSpacing;
	m_NumVertices = m_VertsPerRow*m_VertsPerCol;
	m_NumTriangles = m_CellsPerRow*m_CellsPerCol * 2;
	//load heightmap
	if (!ReadRawFile(heightmapFile))
		::PostQuitMessage(0);
	//scale heights
	for (int i = m_Heightmap.size() - 1; i >= 0; i--)
	{
		m_Heightmap[i] = m_Heightmap[i] * m_HeightScale;
	}
	//compute vertices
	if (!ComputeVertices())
		::PostQuitMessage(0);
	//compute indices
	if (!ComputeIndices())
		::PostQuitMessage(0);

	GenTexture(&D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	if (FAILED(D3DXCreateTextureFromFile(m_Device, textureFile, &m_Texture)))
	{
		MessageBox(NULL, "Terrain: Create Texture: Failed", "Error", 0);
	}
}

Terrain::~Terrain()
{
	if (m_VertexBuffer)
		m_VertexBuffer->Release();
	if (m_IndexBuffer)
		m_IndexBuffer->Release();
	if (m_Texture)
		m_Texture->Release();
}

void Terrain::Render(D3DXMATRIX* world, bool drawTris)
{
	if (m_Device)
	{
		m_Device->SetTransform(D3DTS_WORLD, world);
		m_Device->SetStreamSource(0, m_VertexBuffer, 0, sizeof(D3D::Vertex_UV));
		m_Device->SetFVF(D3D::FVF);
		m_Device->SetIndices(m_IndexBuffer);
		m_Device->SetTexture(0, m_Texture);
		//turn off lighting
		m_Device->SetRenderState(D3DRS_LIGHTING, false);
		if (FAILED(m_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_NumVertices, 0, m_NumTriangles)))
			MessageBox(NULL, "Terrain: DrawIndexedPrimative: Failed", "Error", 0);
		m_Device->SetRenderState(D3DRS_LIGHTING, true);
		if (drawTris)
		{
			m_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			if (FAILED(m_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_NumVertices, 0, m_NumTriangles)))
				MessageBox(NULL, "Terrain: DrawIndexedPrimative wire frame: Failed", "Error", 0);
			m_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
	}
}

float Terrain::GetHeight(float x, float z)
{
	//translate the start point to the origin
	x = ((float)m_Width / 2.0f) + x;
	z = ((float)m_Depth / 2.0f) - z;
	//scale cell spacing to one
	x /= (float)m_CellSpacing;
	z /= (float)m_CellSpacing;
	// From now on, we will interpret our positive z-axis as
	// going in the 'down' direction, rather than the 'up' direction.
	// This allows to extract the row and column simply by 'flooring'
	// x and z:
	float l_Col = ::floorf(x);
	float l_Row = ::floorf(z);
	// get the heights of the quad we're in:
	//
	//  A   B
	//  *---*
	//  | / |
	//  *---*
	//  C   D
	float l_A = (float)GetHeightmapIndex((int)l_Row, (int)l_Col);
	float l_B = (float)GetHeightmapIndex((int)l_Row, (int)l_Col + 1);
	float l_C = (float)GetHeightmapIndex((int)l_Row + 1, (int)l_Col);
	float l_D = (float)GetHeightmapIndex((int)l_Row + 1, (int)l_Col + 1);
	//
	// Find the triangle we are in:
	//

	// Translate by the transformation that takes the upper-left
	// corner of the cell we are in to the origin.  Recall that our
	// cellspacing was nomalized to 1.  Thus we have a unit square
	// at the origin of our +x -> 'right' and +z -> 'down' system.
	float l_dx = x - l_Col;
	float l_dz = z - l_Row;
	// Note the below compuations of u and v are unneccessary, we really
	// only need the height, but we compute the entire vector to emphasis
	// the books discussion.
	float l_Height = 0.0f;
	if (l_dz < 1.0f - l_dx)  // upper triangle ABC
	{
		float uy = l_B - l_A; // A->B
		float vy = l_C - l_A; // A->C

						  // Linearly interpolate on each vector.  The height is the vertex
						  // height the vectors u and v originate from {A}, plus the heights
						  // found by interpolating on each vector u and v.
		l_Height = l_A + D3D::Lerp(0.0f, uy, l_dx) + D3D::Lerp(0.0f, vy, l_dz);
	}
	else // lower triangle DCB
	{
		float uy = l_C - l_D; // D->C
		float vy = l_B - l_D; // D->B

						  // Linearly interpolate on each vector.  The height is the vertex
						  // height the vectors u and v originate from {D}, plus the heights
						  // found by interpolating on each vector u and v.
		l_Height = l_D + D3D::Lerp(0.0f, uy, 1.0f - l_dx) + D3D::Lerp(0.0f, vy, 1.0f - l_dz);
	}

	return l_Height;
}
