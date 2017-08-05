#include "Model.h"




Model::Model(LPDIRECT3DDEVICE9 Device, char* file, float scale, D3DXMATRIX  worldTrans):m_pDevice(Device), m_File(file), m_Scale(scale), m_TransMat(worldTrans)
{
	m_pDevice = Device;
	m_Mesh = NULL;
	D3D::InitVertexPNT(Device);
	LoadMeshes();
}

Model::~Model()
{
	m_Mesh->Release();
	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		m_Textures[i]->Release();
	}
}

void Model::LoadMeshes()
{
	ID3DXBuffer* l_AdjBuffer = 0;
	ID3DXBuffer* l_MtrlBuffer = 0;
	DWORD l_NumMtrls = 0;

	if (FAILED(D3DXLoadMeshFromX(m_File.c_str(), D3DXMESH_MANAGED, m_pDevice, &l_AdjBuffer, &l_MtrlBuffer, 0, &l_NumMtrls, &m_Mesh)))
	{
		MessageBox(0, "Model: LoadMeshes: LoadMeshFromX: Failed", "Error", NULL);
	}

	//// Step 2: Find out if the mesh already has normal info?

	//D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
	//if (FAILED(m_Mesh->GetDeclaration(elems)))
	//	MessageBox(NULL, "Model: Check Normals Failed", "Error", NULL);

	//bool hasNormals = false;
	//D3DVERTEXELEMENT9 term = D3DDECL_END();
	//for (int i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	//{
	//	// Did we reach D3DDECL_END() {0xFF,0,D3DDECLTYPE_UNUSED, 0,0,0}?
	//	if (elems[i].Stream == 0xff)
	//		break;

	//	if (elems[i].Type == D3DDECLTYPE_FLOAT3 &&
	//		elems[i].Usage == D3DDECLUSAGE_NORMAL &&
	//		elems[i].UsageIndex == 0)
	//	{
	//		hasNormals = true;
	//		break;
	//	}
	//}

	//// Step 3: Change vertex format to VertexPNT.

	//D3DVERTEXELEMENT9 elements[64];
	//UINT numElements = 0;
	//D3D::VertexPNT::Decl->GetDeclaration(elements, &numElements);

	//ID3DXMesh* temp = 0;
	//if (FAILED(m_Mesh->CloneMesh(D3DXMESH_SYSTEMMEM,
	//	elements, m_pDevice, &temp)))
	//	MessageBox(NULL, "Model: ChangeVertexFormat Failed", "Error", NULL);
	//ReleaseCOM(m_Mesh);
	//m_Mesh = temp;

	//// Step 4: If the mesh did not have normals, generate them.

	//if (hasNormals == false)
	//	if (FAILED(D3DXComputeNormals(m_Mesh, 0)))
	//		MessageBox(NULL, "Model: Compute Normals: Failed", "Error", NULL);


	//// Step 5: Optimize the mesh.

	//if (FAILED(m_Mesh->Optimize(D3DXMESH_MANAGED |
	//	D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
	//	(DWORD*)l_AdjBuffer->GetBufferPointer(), 0, 0, 0, NULL)))
	//	MessageBox(NULL, "Model: Optimize Mesh: Failed", "Error", NULL);
	//l_AdjBuffer->Release();
	////create bounding box array
	//DWORD l_NumEntries;
	//m_Mesh->GetAttributeTable(NULL, &l_NumEntries);
	//D3DXATTRIBUTERANGE *l_AtTable = new D3DXATTRIBUTERANGE[l_NumEntries];
	//m_Mesh->GetAttributeTable(l_AtTable, &l_NumEntries);

	//D3D::VertexPNT* l_v = 0;
	//for (int i = 0; i < l_NumEntries; i++)//loop through the attribute table
	//{
	//	D3D::BoundingBox* l_Box;
	//	l_Box = new D3D::BoundingBox();
	//	D3DXVECTOR3 l_FirstPos{ 0,0,0 };
	//	m_Mesh->LockVertexBuffer(0, (void**)&l_v);
	//	DWORD FVF = (DWORD)D3DXGetFVFVertexSize(m_Mesh->GetFVF());
	//	if (FAILED(D3DXComputeBoundingBox(&l_v[i].pos, l_AtTable[i].VertexCount, FVF, &l_Box->MIN, &l_Box->MAX)))
	//		MessageBox(NULL, "Modle: ComputeBoundingBox: Failed", "Error", NULL);
	//	m_BoundVol.push_back(l_Box);
	//	m_Mesh->UnlockVertexBuffer();
	//}


	if (l_MtrlBuffer&&l_NumMtrls)
	{
		D3DXMATERIAL* l_Mtrls = (D3DXMATERIAL*)l_MtrlBuffer->GetBufferPointer();
		for (unsigned int i = 0; i < l_NumMtrls; i++)
		{
			//l_Mtrls[i].MatD3D.Ambient = l_Mtrls[i].MatD3D.Diffuse;
			m_Materials.push_back(l_Mtrls[i].MatD3D);
			//check for texture
			if (l_Mtrls[i].pTextureFilename)
			{
				char l_PathBuffer[_MAX_PATH];
				char l_Drive[_MAX_DRIVE];
				char l_Dir[_MAX_DIR];
				char l_FileName[_MAX_FNAME];
				char l_Ext[_MAX_EXT];
				_splitpath_s(m_File.c_str(), l_Drive, _MAX_DRIVE, l_Dir, _MAX_DIR, l_FileName, _MAX_FNAME, l_Ext, _MAX_EXT);

				LPDIRECT3DTEXTURE9 l_Tex = 0;
				char l_TextureFileName[100];
				sprintf_s(l_TextureFileName, "%s/%s", l_Dir, l_Mtrls[i].pTextureFilename);
				if (FAILED(D3DXCreateTextureFromFile(m_pDevice, l_TextureFileName, &l_Tex)))
				{
					MessageBox(NULL, "Model:LoadMeshes(): CreateTexture: Failed", "Error", NULL);
				}
				m_Textures.push_back(l_Tex);
			}
			else
			{
				m_Textures.push_back(0);
			}
		}
	}
	l_MtrlBuffer->Release();

	////stuff needed to retrieve bounding box data
	////create adjacency buffer
	//DWORD* l_pAdjBuff = new DWORD[m_Mesh->GetNumFaces() * 3];
	////generate adjacency
	//m_Mesh->GenerateAdjacency(0.0001f, l_pAdjBuff);
	////optimize mesh, must do
	//m_Mesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT, l_pAdjBuff, NULL, NULL, NULL);
	////delete adjacency
	//delete l_pAdjBuff;




}

void Model::Render()
{
	D3DXMATRIX l_ScaleMat;
	D3DXMatrixScaling(&l_ScaleMat, m_Scale, m_Scale, m_Scale);
	l_ScaleMat = l_ScaleMat * m_TransMat;
	m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, true);
	m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	for (unsigned int i = 0; i < m_Materials.size(); i++)
	{
		m_pDevice->SetMaterial(&D3D::WHITE_MTRL);
		if (FAILED(m_pDevice->SetTexture(0, m_Textures[i])))
		{
			MessageBox(NULL, "Model:Render():SetTexture():Failed", "Error", NULL);
		}
		m_pDevice->SetTransform(D3DTS_WORLD, &l_ScaleMat);
		m_Mesh->DrawSubset(i);
		//m_BoundVol[i]->Render(m_pDevice);
	}
	m_pDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, false);
	m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, false);
}

void Model::ReleaseMeshes()
{
	m_Mesh->Release();
	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		m_Textures[i]->Release();
	}
}

void Model::UpdateLocation(D3DXVECTOR3 NewLocation)
{
	D3DXMATRIX l_Temp;
	ZeroMemory(&l_Temp, sizeof(D3DXMATRIX));
	D3DXMatrixTranslation(&l_Temp, NewLocation.x, NewLocation.y, NewLocation.z+10);
	m_TransMat = m_TransMat*l_Temp;
}
