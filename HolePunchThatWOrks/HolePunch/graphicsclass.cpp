////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_LightShader = 0;
	m_Light = 0;
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Store file locations for easier reference
	char* cstandingBoxer = "../HolePunch/Data/b_Stand.txt";
	char* cboxingRing = "../HolePunch/Data/BoxingRing.txt";

	m_FileNames.push_back(cstandingBoxer);
	m_FileNames.push_back(cboxingRing);
	
	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		//MessageBox(hwnd, "Could not initialize Direct3D", "Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// Create the model object.
	for (int i = 0; i < (int)m_FileNames.size(); i++)
	{
		m_Model = new ModelClass;
		if (!m_Model)
		{
			return false;
		}

		// Initialize the model object.
		if(m_FileNames[i] == cstandingBoxer)
		{
			result = m_Model->Initialize(m_D3D->GetDevice(), m_FileNames[i], L"../HolePunch/Data/test.dds");
		}
		else if(m_FileNames[i] == cboxingRing)
		{
			result = m_Model->Initialize(m_D3D->GetDevice(), m_FileNames[i], L"../HolePunch/Data/test.dds");
		}

		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
		m_List.push_back(m_Model);
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	//intialize light variables
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, .5f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, -1.0f, 0.0f);

	return true;
}

void GraphicsClass::Shutdown()
{
	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	for (int i = 0; i < (int)m_List.size(); i++)
	{
		m_List[i]->Shutdown();
		delete m_List[i];
		m_List[i] = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
	return;
}

bool GraphicsClass::Frame()
{
	bool result;
	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}
	return true;
}

bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	D3DXMatrixTranslation(&viewMatrix, 0.0, -10.0, 50);

	for (int i = 0; i < (int)m_List.size(); i++)
	{
		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_List[i]->Render(m_D3D->GetDeviceContext());

		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_List[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			m_List[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor() , m_Light->GetDiffuseColor());
		if (!result)
		{
			return false;
		}
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();
	return true;
}