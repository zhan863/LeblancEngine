//======================================================================
//
//	MJP's DX11 Sample Framework
//  http://mynameismjp.wordpress.com/
//
//  All code and content licensed under Microsoft Public License (Ms-PL)
//
//======================================================================
#pragma once
#include "LeblancEngine/BasicInclude/LeblancPCH.h"

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 texcoord;
};

class LeblancMesh
{
public:
	ID3D11Buffer* m_vertex_buffer = nullptr;
	ID3D11Buffer* m_index_buffer = nullptr;

	LeblancMesh();
	~LeblancMesh();

	void release();
};