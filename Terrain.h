#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H
#include "Graphics.h"
class Terrain
{
	LPDIRECT3DDEVICE9 m_Device;
	LPDIRECT3DTEXTURE9 m_Texture;
	LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer;
	LPDIRECT3DINDEXBUFFER9 m_IndexBuffer;
	int m_VertsPerRow;
	int m_VertsPerCol;
	int m_CellSpacing;
	int m_CellsPerRow;
	int m_CellsPerCol;
	int m_Width;
	int m_Depth;
	int m_NumVertices;
	int m_NumTriangles;

	float m_HeightScale;
	std::vector<float> m_Heightmap;
	bool ReadRawFile(char* filename);
	bool ComputeVertices();
	bool ComputeIndices();
	float ComputeShade(int cellRow, int cellCol, D3DXVECTOR3* directionToLight);
	float GetHeightmapIndex(int cellRow, int cellCol);
	bool GenTexture(D3DXVECTOR3 * directionToLight);
	bool InBounds(int i, int j);
public:
	Terrain(LPDIRECT3DDEVICE9 device,
		char* textureFile,
		char* heightmapFile,
		int numVertsPerRow,
		int numVertsPerCol,
		int cellSpacing,    // space between cells
		float heightScale);
	~Terrain();
	void Render(D3DXMATRIX* world, bool drawTris);
	float GetHeight(float x, float z);
	bool LightTerrain(D3DXVECTOR3* directionToLight);
};

#endif // !TERRAIN_H
