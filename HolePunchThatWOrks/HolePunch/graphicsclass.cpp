////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"

//store model file locations for easier reference
char* cstandingBoxer = "../HolePunch/Data/b_Stand.txt";
char* cpunchingBoxer = "../HolePunch/Data/b_Punch.txt";
char* cboxingRing = "../HolePunch/Data/BoxingRing.txt";

GraphicsClass::GraphicsClass()
{
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_LightShader = 0;
	m_Light1 = 0;
	m_Light2 = 0;
	m_Light3 = 0;
	m_Light4 = 0;
	m_HorizontalBlurShader = 0;
	m_VerticalBlurShader = 0;
	m_GlowShader = 0;
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	m_FileNames.push_back(cstandingBoxer);
	m_FileNames.push_back(cpunchingBoxer);
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
		else if(m_FileNames[i] == cpunchingBoxer)
		{
			result = m_Model->Initialize(m_D3D->GetDevice(), m_FileNames[i], L"../HolePunch/Data/b_Windup_tex.dds");
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

	// Create the first light object.
	m_Light1 = new LightClass;
	if (!m_Light1)
	{
		return false;
	}

	// Initialize the first light object.
	m_Light1->SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_Light1->SetPosition(-3.0f, 1.0f, 3.0f);

	// Create the second light object.
	m_Light2 = new LightClass;
	if (!m_Light2)
	{
		return false;
	}

	// Initialize the second light object.
	m_Light2->SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	m_Light2->SetPosition(3.0f, 1.0f, 3.0f);

	// Create the third light object.
	m_Light3 = new LightClass;
	if (!m_Light3)
	{
		return false;
	}

	// Initialize the third light object.
	m_Light3->SetDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	m_Light3->SetPosition(-3.0f, 1.0f, -3.0f);

	// Create the fourth light object.
	m_Light4 = new LightClass;
	if (!m_Light4)
	{
		return false;
	}

	// Initialize the fourth light object.
	m_Light4->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light4->SetPosition(3.0f, 1.0f, -3.0f);

	// Create the glow shader object.
	m_GlowShader = new GlowShaderClass;
	if(!m_GlowShader)
	{
		return false;
	}

	// Initialize the glow shader object.
	result = m_GlowShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the glow shader object.", L"Error", MB_OK);
		return false;
	}
	return true;
}

void GraphicsClass::Shutdown()
{
	// Release the glow shader object.
	if(m_GlowShader)
	{
		m_GlowShader->Shutdown();
		delete m_GlowShader;
		m_GlowShader = 0;
	}

	// Release the light objects.
	if (m_Light1)
	{
		delete m_Light1;
		m_Light1 = 0;
	}

	if (m_Light2)
	{
		delete m_Light2;
		m_Light2 = 0;
	}

	if (m_Light3)
	{
		delete m_Light3;
		m_Light3 = 0;
	}

	if (m_Light4)
	{
		delete m_Light4;
		m_Light4 = 0;
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
	D3DXVECTOR4 diffuseColor[4];
	D3DXVECTOR4 lightPosition[4];
	bool result;

	// Create the diffuse color array from the four light colors.
	diffuseColor[0] = m_Light1->GetDiffuseColor();
	diffuseColor[1] = m_Light2->GetDiffuseColor();
	diffuseColor[2] = m_Light3->GetDiffuseColor();
	diffuseColor[3] = m_Light4->GetDiffuseColor();

	// Create the light position array from the four light positions.
	lightPosition[0] = m_Light1->GetPosition();
	lightPosition[1] = m_Light2->GetPosition();
	lightPosition[2] = m_Light3->GetPosition();
	lightPosition[3] = m_Light4->GetPosition();

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	for (int i = 0; i < (int)m_List.size(); i++)
	{
		//translate positions based on object drawn
		if(m_FileNames[i] == cstandingBoxer)
		{
			D3DXMatrixTranslation(&viewMatrix, 0.0, 10.0, 50);
		}
		else if(m_FileNames[i] == cpunchingBoxer)
		{
			D3DXMatrixTranslation(&viewMatrix, 5.0, 5.0, 50);
		}
		else if(m_FileNames[i] == cboxingRing)
		{
			D3DXMatrixTranslation(&viewMatrix, 0.0, -10.0, 50);
		}

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_List[i]->Render(m_D3D->GetDeviceContext());

		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_List[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			m_List[i]->GetTexture(), diffuseColor, lightPosition);

		// Render the model using the glow shader if applicable
		if(m_FileNames[i] == cboxingRing){

			//result = m_VerticalBlurShader->Render(m_D3D->GetDeviceContext(), m_List[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
			//		      m_HorizontalBlurTexture->GetShaderResourceView(), screenSizeY);
			result = m_GlowShader->Render(m_D3D->GetDeviceContext(), m_List[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
				m_List[i]->GetTexture(), 5.0f);
		}			

		if (!result)
		{
			return false;
		}
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();
	return true;
}
