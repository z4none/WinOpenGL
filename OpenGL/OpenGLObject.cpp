#include "stdafx.h"
#include "OpenGLObject.h"


OpenGLObject::OpenGLObject()
{
	m_rotate = 0.0f;
}


OpenGLObject::~OpenGLObject()
{
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteVertexArrays(1, &m_vertexArray);
}

bool OpenGLObject::InitGlew(CWnd *window)
{
	//创建临时窗口，只是为了初始化glew库
	PIXELFORMATDESCRIPTOR pfd;
	CWnd* tempWnd = new CWnd();
	tempWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 20, 20), window, NULL);
	HDC temHdc = tempWnd->GetDC()->GetSafeHdc();
	//尝试设置像素格式
	if (!SetPixelFormat(temHdc, 1, &pfd))//每个窗口只能设置一次
		return false;
	HGLRC temphRC = wglCreateContext(temHdc);//创建一个临时的环境为了初始化glew,初始化后才能够使用wglChoosePixelFormatARB，wglCreateContextAttribsARB函数 
	wglMakeCurrent(temHdc, temphRC);//只有设置当前opengl环境后才能够初始化glew库 
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		MessageBox(NULL,L"GLEW初始化失败！", L"提示", MB_ICONERROR);
		return false;
	}
	if (!WGLEW_ARB_create_context || !WGLEW_ARB_pixel_format)
	{
		return false;
	}
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(temphRC);
	tempWnd->DestroyWindow();//销毁临时窗口
	delete tempWnd;
	return true;
}

bool OpenGLObject::InitializeOpenGL(CDC* pDC)
{
	SetupPixelFormat(pDC);
	::wglMakeCurrent(pDC->GetSafeHdc(), m_hRc);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	return true;
}

bool OpenGLObject::SetupPixelFormat(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	int nPixelFormat = -1;
	int nPixCount = 0;
	float fPixAttribs[] = { 0, 0 };
	int iPixAttribs[] = {
		WGL_SUPPORT_OPENGL_ARB, 1,//支持OPENGL渲染
		WGL_DRAW_TO_WINDOW_ARB, 1,//像素格式可以运行到窗口
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,//让HW加速
		WGL_COLOR_BITS_ARB, 24,//R,G,B每种颜色8bit
		WGL_DEPTH_BITS_ARB, 24,//24位深度
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,//双缓冲上下文
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,//pf should be RGBA type
		WGL_STENCIL_BITS_ARB, 8,//模版缓冲区8bit
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,//开启多重采样
		WGL_SAMPLES_ARB, 4,//多重采样样本为4
		0
	};
	//新的查询像素格式的函数
	wglChoosePixelFormatARB(pDC->GetSafeHdc(), iPixAttribs, fPixAttribs, 1, &nPixelFormat, (UINT*)&nPixCount);
	//多重采样时，如果硬件不支持就使用下面的代码关闭多重采样
	if (nPixelFormat == -1)
	{
		//try again without MSAA
		iPixAttribs[18] = 1;
		wglChoosePixelFormatARB(pDC->GetSafeHdc(), iPixAttribs, fPixAttribs, 1, &nPixelFormat, (UINT*)&nPixCount);
	}
	//获取一个像素格式，现在设置它为当前的上下文
	if (!SetPixelFormat(pDC->GetSafeHdc(), nPixelFormat, &pfd))
	{
		MessageBox(NULL,L"设置像素格式失败！", L"提示", MB_ICONERROR);
		return false;
	}
	GLint attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,//主版本3
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,//次版本3
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,//要求返回兼容模式环境,如果不指定或指定为WGL_CONTEXT_CORE_PROFILE_BIT_ARB会返回只包含核心功能的环境 
		0
	};
	m_hRc = wglCreateContextAttribsARB(pDC->GetSafeHdc(), 0, attribs);
	if (m_hRc == NULL)
	{
		MessageBox(NULL,L"不能创建OGL3.3 context\n尝试创建3.2？", L"提示", MB_OK);
		attribs[3] = 2;
		m_hRc = wglCreateContextAttribsARB(pDC->GetSafeHdc(), 0, attribs);
		if (m_hRc == NULL)
		{
			MessageBox(NULL,L"opengl3.2 create failed!", L"提示", MB_OK);
			return false;
		}
	}
	wglMakeCurrent(NULL, NULL);
	return true;
}

void OpenGLObject::DrawScene()
{
	glClearColor(0.2f, 0.5f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CShader m_Shader;
	std::string vertexShaderCode = m_Shader.GetFileStr("..\\ShaderFile\\vertex.glsl");
	std::string fragmentShaderCode = m_Shader.GetFileStr("..\\ShaderFile\\fragment.glsl");
	m_Shader.AddShader(GL_VERTEX_SHADER, vertexShaderCode);
	m_Shader.AddShader(GL_FRAGMENT_SHADER, fragmentShaderCode);
	m_Shader.LinkShaderProgram();
	m_rotate += 1.0f;
	if (m_rotate >= 360.0f)
		m_rotate = 0.0f;
	glm::mat4 model;
	model = glm::rotate(model, glm::radians(m_rotate), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 view;
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_Shader.UseProgram();


	glUniformMatrix4fv(glGetUniformLocation(m_Shader.m_shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(m_Shader.m_shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
	glUniformMatrix4fv(glGetUniformLocation(m_Shader.m_shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(m_vertexArray);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
	SwapBuffers(wglGetCurrentDC());
}

void OpenGLObject::CreateSceneData()
{
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vertexArray);
	glBindVertexArray(m_vertexArray);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void OpenGLObject::DestroyOGL()
{
	if (wglGetCurrentContext() != NULL)
		wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	if (m_hRc != NULL)
		wglDeleteContext(m_hRc);
	if (m_pDC)
		delete m_pDC;
}

void OpenGLObject::ResetViewPort(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}
