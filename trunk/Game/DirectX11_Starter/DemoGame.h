#pragma once

#include <DirectXMath.h>
#include "DXGame.h"
#include <fstream>
#include <iostream>
using namespace std;

// Include run-time memory checking in debug builds
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// For DirectX Math
using namespace DirectX;
 
// Vertex struct for triangles
struct Vertex
{
	XMFLOAT3 Position;
	XMFLOAT4 Color;
};

struct ModelType
{
	float x,y,z;
	float tu,tv;
	float nx,ny,nz;
};

// Struct to match vertex shader's constant buffer
// You update one of these locally, then push it to the corresponding
// constant buffer on the device when it needs to be updated
struct VertexShaderConstantBuffer
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
};

// Demo class which extends the base DXGame class
class DemoGame : public DXGame
{
public:
	DemoGame(HINSTANCE hInstance);
	~DemoGame();

	// Overrides for base level methods
	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene(); 

	// For handing mouse input
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnKeyDown(WPARAM keyCode);

	//---------------------------------------------------------------Model related Variables
	ModelType* m_model;	// an array of ModelTypes used to read in and hold model data
	int m_vertexCount;	// how many verticies are in the model
	int m_indexCount;	// how many indexes are needed

private:
	// Initialization for our "game" demo
	void CreateGeometryBuffers();
	void LoadShadersAndInputLayout();

	// -------------------------------------------------------------Model Loading methods
	bool LoadModel(char*);
	void ReleaseModel();
private:
	// Buffers to hold actual geometry
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// Our basic shaders for this example
	ID3D11PixelShader* pixelShader;
	ID3D11VertexShader* vertexShader;

	// A few more odds and ends we'll need
	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* vsConstantBuffer;
	VertexShaderConstantBuffer vsConstantBufferData;

	// The matrices to go from 3D model space
	// to screen space
	XMFLOAT4X4 worldMatrix;
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
};