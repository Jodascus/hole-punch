////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

//////////////
// INCLUDES //
//////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>

#include "lightshaderclass.h"
#include "lightclass.h"

#include <vector>

using namespace DirectX;

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	vector<char*> m_FileNames;
	vector<ModelClass*> m_List;
};

#endif