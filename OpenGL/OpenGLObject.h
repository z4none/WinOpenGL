#pragma once
#include "common.h"
#include "Shader.h"
class WinOpenGL_API OpenGLObject
{
public:
	OpenGLObject();
	~OpenGLObject();
	bool InitGlew(CWnd* window);
	bool InitializeOpenGL(CDC* pDC);
	bool SetupPixelFormat(CDC* pDC);
	void DrawScene();
	void CreateSceneData();
	void DestroyOGL();
	void ResetViewPort(int x,int y,int width,int height);
public:
	CClientDC	*m_pDC;
	CRect	m_oldRect;
	HGLRC	m_hRc;
	GLuint m_vertexArray;
	GLuint m_vertexBuffer;

	glm::mat4 m_model;
	glm::mat4 m_view;
	glm::mat4 m_projection;
	float m_rotate;
};

