#pragma once
#include "stdafx.h"
#include "common.h"
#include <iostream>
#include <fstream>
#include<sstream>
class WinOpenGL_API CShader :public CObject
{
public:
	CShader();
	~CShader();
	std::string GetFileStr(const char* fileName);
	void AddShader(GLenum shaderType, std::string shaderSource);
	void LinkShaderProgram();
	void UseProgram();
public:
	GLuint m_shaderProgram;
};
