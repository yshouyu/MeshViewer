// MeshViewer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <time.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include "Textfile.h"
#include "mesh.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;

const GLfloat gPi = 3.14159265;
const GLfloat gColorRed[4] = { 1.0, 0.0, 0.0, 0.5 };
const GLfloat gColorGreen[4] = { 0.0, 1.0, 0.0, 0.5 };
const GLfloat gColorBlue[4] = { 0.0, 0.0, 1.0, 0.5 };
const GLfloat gColorBlack[4] = { 0.0, 0.0, 0.0, 0.5 };
const GLfloat gColorGray[4] = { 0.5, 0.5, 0.5, 0.5 };
const GLfloat gColorWhite[4] = { 1.0, 1.0, 1.0, 0.5 };
const GLfloat* gDefaultLightColor = gColorGray;

GLfloat gEyePos[3] = { 0.0, 0.0, 3.0 };
GLfloat gLookAt[3]  = { 0.0, 0.0, 0.0 };
GLfloat gLookUp[3] = { 0.0, 1.0, 0.0 };
GLfloat gOffset[3] = { 0.0f, 0.0f, 0.0f };
GLfloat gAngel[3] = { 0.0f, 0.0f, 0.0f };
GLfloat gStep = 0.5f;
glm::mat4 gViewMatrix;
glm::mat4 gProjectionMatrix;
GLfloat gLightPos[] = { 1.0f, 1.0, 1.0};
GLfloat gAmbientStrength = 0.5f;
const GLfloat* gAmbientColor = gDefaultLightColor;
const GLfloat* gDiffuseColor = gDefaultLightColor;
const GLfloat* gSpecularColor = gDefaultLightColor;
GLfloat gSpecularStrength = 0.5f;
bool gLightEnable = true;
enum LightingModel
{
	Lambert,
	Phong,
};
LightingModel gLightingModel = Lambert;

enum ShadingModel 
{
	Flat,
	Smooth,
};
ShadingModel gShadingModel = Smooth;

CMesh* gMesh = NULL;
char gObjPath[128];
/***************shader**********************/
GLuint gVao, gVbo[3];
struct Shader {
	bool success;
	GLuint vert;
	GLuint frag;
	GLuint handler;
	std::string log;
};

void compileShader(Shader* shader, const char* vertSrc, const char* fragSrc)
{
	// vert
	{
		GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
		shader->vert = vertShader;
		//error check  
		if (!shader->vert)
		{
			shader->success = false;
			return;
		}
		glShaderSource(shader->vert, 1, &vertSrc, NULL);
		glCompileShader(shader->vert);

		GLint flag;
		glGetShaderiv(shader->vert, GL_COMPILE_STATUS, &flag);
		if (flag == GL_FALSE)
		{
			shader->success = false;
			GLint logLength;
			glGetShaderiv(shader->vert, GL_INFO_LOG_LENGTH, &logLength);
			if (logLength > 0)
			{
				char* log = (char*)malloc(logLength);
				GLsizei tmp;
				glGetShaderInfoLog(shader->vert, logLength, &tmp, log);
				shader->log = std::string(log);
			}
			return;
		}
	}

	//frag
	{
		shader->frag = glCreateShader(GL_FRAGMENT_SHADER);
		//error check  
		if (!shader->frag)
		{
			shader->success = false;
			return;
		}
		glShaderSource(shader->frag, 1, &fragSrc, NULL);
		glCompileShader(shader->frag);
		GLint flag;
		glGetShaderiv(shader->frag, GL_COMPILE_STATUS, &flag);
		if (flag == GL_FALSE)
		{
			shader->success = false;
			GLint logLength;
			glGetShaderiv(shader->frag, GL_INFO_LOG_LENGTH, &logLength);
			if (logLength > 0)
			{
				char* log = (char*)malloc(logLength);
				GLsizei tmp;
				glGetShaderInfoLog(shader->frag, logLength, &tmp, log);
				shader->log = std::string(log);
			}
			return;
		}
	}

	shader->handler = glCreateProgram();
	if (!shader->handler)
	{
		shader->success = false;
		return;
	}
	//link shader program to created program
	glAttachShader(shader->handler, shader->vert);
	glAttachShader(shader->handler, shader->frag);
	glLinkProgram(shader->handler);

	GLint linkFlag;
	glGetProgramiv(shader->handler, GL_LINK_STATUS, &linkFlag);
	if (linkFlag == GL_FALSE)
	{
		shader->success = false;
		GLint logLength;
		glGetProgramiv(shader->handler, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0)
		{
			char* log = (char *)malloc(logLength);
			GLsizei tmp;
			glGetProgramInfoLog(shader->handler, logLength, &tmp, log);
			shader->log = std::string(log);
		}
		return;
	}
	shader->success = true;
}

Shader* initShader(const char* vertFile, const char* fragFile)
{
	Shader* result = new Shader();
	const char* vertSrc = readtext(vertFile);
	const char* fragSrc = readtext(fragFile);
	if (!vertSrc || !fragSrc)
	{
		result->success = false;
		return result;
	}
	compileShader(result, vertSrc, fragSrc);
	return result;
}

std::string shaderList[] = {
	"Lambert", "lambert.vert", "lambert.frag",
	"Phong", "phong.vert", "phong.frag",
	"LambertFlat", "lambert_flat.vert", "lambert_flat.frag",
	"PhongFlat", "phong_flat.vert", "phong_flat.frag",
};
std::unordered_map<std::string, Shader*> gShaderMap;
void InitShaderList()
{
	for (int i = 0; i < 12; i+=3)
	{
		gShaderMap.insert(std::make_pair(shaderList[i], initShader(shaderList[i + 1].c_str(), shaderList[i + 2].c_str())));
	}
}

Shader* GetShader(const std::string& shaderName)
{
	if (gShaderMap.find(shaderName) != gShaderMap.end())
	{
		Shader* shader = gShaderMap.find(shaderName)->second;
		if (shader->success)
		{
			return shader;
		}
		else
		{
			printf("shader compile fail: %s\n", shader->log.c_str());
			return NULL;
		}
	}
	return NULL;
}

void UseShader(const std::string& shaderName)
{
	Shader* shader = GetShader(shaderName);
	if (shader)
	{
		glUseProgram(shader->handler);
	}
}
void SetShaderUniformfv(const std::string& shaderName, const std::string& propertyName, int valueNum, GLfloat* values)
{
	Shader* shader = GetShader(shaderName);
	if (shader)
	{
		GLuint location = glGetUniformLocation(shader->handler, propertyName.c_str());
		switch (valueNum)
		{
		case 1:
			glUniform1fv(location, 1, values);
			break;
		case 2:
			glUniform2fv(location, 1, values);
			break;
		case 3:
			glUniform3fv(location, 1, values);
			break;
		case 4:
			glUniform4fv(location, 1, values);
			break;
		default:
			break;
		}
	}
}

void SetShaderUniformMatrixfv(const std::string& shaderName, const std::string& propertyName, int valueNum, GLfloat* values)
{
	Shader* shader = GetShader(shaderName);
	if (shader)
	{
		GLuint location = glGetUniformLocation(shader->handler, propertyName.c_str());
		switch (valueNum)
		{
		case 2:
			glUniformMatrix2fv(location, 1, GL_FALSE, values);
			break;
		case 3:
			glUniformMatrix3fv(location, 1, GL_FALSE, values);
			break;
		case 4:
			glUniformMatrix4fv(location, 1, GL_FALSE, values);
			break;
		default:
			break;
		}
	}
}

void LoadMesh()
{
	if (!gMesh)//load mesh
	{
		gMesh = meshOBJLoad(gObjPath);
		if (!gMesh)
			exit(0);
		meshunt(gMesh);
		meshfn(gMesh);
		meshvn(gMesh, 90.0);
		convertToShaderData(gMesh);
	}
}

void LoadNewMesh()
{
	CMesh* newMesh = meshOBJLoad(gObjPath);
	if (newMesh)
	{
		gMesh = newMesh;
		meshunt(gMesh);
		meshfn(gMesh);
		meshvn(gMesh, 90.0);
		convertToShaderData(gMesh);
	}
}

void InitRenderBuffers()
{
	glGenVertexArrays(1, &gVao);
	glBindVertexArray(gVao);
	glGenBuffers(3, gVbo);
	glBindVertexArray(0);
}

void UpdateRenderBuffers()
{
	glBindVertexArray(gVao);
	// pos
	{
		glBindBuffer(GL_ARRAY_BUFFER, gVbo[0]);
		glBufferData(GL_ARRAY_BUFFER, gMesh->trinums * 12 * sizeof(float), gMesh->objShaderData, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	}
	// color
	{
		glBindBuffer(GL_ARRAY_BUFFER, gVbo[1]);
		glBufferData(GL_ARRAY_BUFFER, gMesh->trinums * 12 * sizeof(float), gMesh->colorShaderData, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	}
	// normal
	{
		glBindBuffer(GL_ARRAY_BUFFER, gVbo[2]);
		glBufferData(GL_ARRAY_BUFFER, gMesh->trinums * 12 * sizeof(float), gMesh->normalShaderData, GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	}
	glBindVertexArray(0);
}

void DisplayShader()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 modelMatrix;
	modelMatrix = glm::translate(modelMatrix, glm::vec3(gOffset[0], gOffset[1], gOffset[2]));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(gAngel[0]), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(gAngel[1]), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(gAngel[2]), glm::vec3(0.0f, 0.0f, 1.0f));

	glPushMatrix();
	/*glRotatef(gAngel[0], 1, 0, 0);
	glRotatef(gAngel[1], 0, 1, 0);
	glRotatef(gAngel[2], 0, 0, 1);
	glTranslatef(gOffset[0], gOffset[1], gOffset[2]);*/

	float glprojectionMatrix[16];
	glGetFloatv(GL_PROJECTION_MATRIX, glprojectionMatrix);

	std::string shaderName;
	switch (gLightingModel)
	{
	case Lambert:
		shaderName = "Lambert";
		break;
	case Phong:
		shaderName = "Phong";
		break;
	}
	if (gShadingModel == Flat)
	{
		shaderName += "Flat";
	}
	
	UseShader(shaderName);
	SetShaderUniformfv(shaderName, "AmbientColor", 4, (GLfloat*)gAmbientColor);
	SetShaderUniformfv(shaderName, "AmbientStrength", 1, &gAmbientStrength);
	if (gLightEnable)
	{
		SetShaderUniformfv(shaderName, "DiffuseColor", 4, (GLfloat*)gDiffuseColor);
		SetShaderUniformfv(shaderName, "SpecularColor", 4, (GLfloat*)gSpecularColor);
	}
	else
	{
		SetShaderUniformfv(shaderName, "DiffuseColor", 4, (GLfloat*)gColorBlack);
		SetShaderUniformfv(shaderName, "SpecularColor", 4, (GLfloat*)gColorBlack);
	}
	
	SetShaderUniformfv(shaderName, "LightPos", 3, gLightPos);
	SetShaderUniformfv(shaderName, "CameraPos", 3, gEyePos);
	SetShaderUniformMatrixfv(shaderName, "model", 4, glm::value_ptr(modelMatrix));
	SetShaderUniformMatrixfv(shaderName, "view", 4, glm::value_ptr(gViewMatrix));
	SetShaderUniformMatrixfv(shaderName, "projection", 4, glm::value_ptr(gProjectionMatrix));
	SetShaderUniformMatrixfv(shaderName, "glprojection", 4, glprojectionMatrix);
	glBindVertexArray(gVao);
	glDrawArrays(GL_TRIANGLES, 0, gMesh->trinums * 3);
	glBindVertexArray(0);
	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

//keyboard
void KeyboardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		gOffset[1] += gStep;
		break;
	case 's':
		gOffset[1] -= gStep;
		break;
	case 'a':
		gOffset[0] -= gStep;
		break;
	case 'd':
		gOffset[0] += gStep;
		break;
	case 'q':
		gOffset[2] -= gStep;
		break;
	case 'e':
		gOffset[2] += gStep;
		break;
	case 'i':
		gAngel[0] -= gStep;
		break;
	case 'k':
		gAngel[0] += gStep;
		break;
	case 'j':
		gAngel[2] += gStep;
		break;
	case 'l':
		gAngel[2] -= gStep;
		break;
	case 'u':
		gAngel[1] -= gStep;
		break;
	case 'o':
		gAngel[1] += gStep;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)w/h, 1.0, 8.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(gEyePos[0], gEyePos[1], gEyePos[2], gLookAt[0], gLookAt[1], gLookAt[2], gLookUp[0], gLookUp[1], gLookUp[2]);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	// update matrix
	{
		gProjectionMatrix = glm::perspective(60.0f, (float)w / (float)h, 1.0f, 8.0f);
		gViewMatrix = glm::lookAt(glm::vec3(gEyePos[0], gEyePos[1], gEyePos[2]),
						glm::vec3(gLookAt[0], gLookAt[1], gLookAt[2]),
						glm::vec3(gLookUp[0], gLookUp[1], gLookUp[2]));
	}
}

void RenderScene(CMesh* mesh, int mode)
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, (GLfloat*)gAmbientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, (GLfloat*)gDiffuseColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 80);
	if (gLightingModel == Lambert)
	{
		glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat*)gColorBlack);
	}
	else if(gLightingModel == Phong)
	{
		glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat*)gSpecularColor);
	}

	if (gShadingModel == Smooth)
	{
		glShadeModel(GL_SMOOTH);
	}
	else if(gShadingModel == Flat)
	{
		glShadeModel(GL_FLAT);
	}

	static int i;
	static Group_t* gp;
	static Triangle_t* triangle;
	gp = mesh->gps;
	while (gp)
	{

		glBegin(GL_TRIANGLES);
		for (i = 0; i < gp->trinums; i++)
		{
			triangle = &(mesh->tri[gp->tri[i]]);
			glNormal3fv(&mesh->normals[3 * triangle->Nind[0]]);
			glVertex3fv(&mesh->verts[3 * triangle->Vind[0]]);
			glNormal3fv(&mesh->normals[3 * triangle->Nind[1]]);
			glVertex3fv(&mesh->verts[3 * triangle->Vind[1]]);
			glNormal3fv(&mesh->normals[3 * triangle->Nind[2]]);
			glVertex3fv(&mesh->verts[3 * triangle->Vind[2]]);
		}
		glEnd();
		gp = gp->next;
	}
}

void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (gLightEnable)
	{
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION, gLightPos);
	}
	else
	{
		glDisable(GL_LIGHT0);
	}
	
	glPushMatrix();		
	glRotatef(gAngel[0], 1, 0, 0);
	glRotatef(gAngel[1], 0, 1, 0);
	glRotatef(gAngel[2], 0, 0, 1);
	glTranslatef(gOffset[0], gOffset[1], gOffset[2]);

	RenderScene(gMesh, MESH_SMOOTH);
	glPopMatrix();


	glFlush();
	glutSwapBuffers();
}

void MenuProcessFunc(int value)
{
	int tag = 0;
	GLint params[2];
	switch (value)
	{
	case '1':
	{
		gOffset[0] = gOffset[1] = gOffset[2] = 0.0f;
		gAngel[0] = gAngel[1] = gAngel[2] = 0.0f;
		break;
	}
	case '2':
	{
		TCHAR szBuffer[MAX_PATH] = { 0 };
		OPENFILENAME file = { 0 };
		file.hwndOwner = NULL;
		file.lStructSize = sizeof(file);
		file.lpstrFilter = _T("obj files(*.obj)\0*.obj\0 all files(*.*)\0*.*\0");//filter obj files 
		file.lpstrInitialDir = _T("C:\\");//inital file address	
		file.lpstrFile = szBuffer;//buffer for file  
		file.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
		file.nFilterIndex = 0;
		file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
		BOOL bSel = GetOpenFileName(&file);
		strcpy(gObjPath, file.lpstrFile);
		LoadNewMesh();
		UpdateRenderBuffers();
		break;
	}
	default:
		break;
	}
	glutPostRedisplay();
}

void LightMenuProcessFunc(int value)
{
	int tag = 0;
	GLint params[2];
	switch (value)
	{
	case '1':
	{
		gLightEnable = true;
		break;
	}
	case '2':
	{
		gLightEnable = false;
		break;
	}
	default:
		break;
	}
	glutPostRedisplay();
}

void LightingModelMenuProcessFunc(int value)
{
	int tag = 0;
	GLint params[2];
	switch (value)
	{
	case '1':
	{
		gLightingModel = Lambert;
		break;
	}
	case '2':
	{
		gLightingModel = Phong;
		break;
	}
	default:
		break;
	}
	glutPostRedisplay();
}

void ShadingModelMenuProcessFunc(int value)
{
	int tag = 0;
	GLint params[2];
	switch (value)
	{
	case '1':
	{
		gShadingModel = Smooth;
		break;
	}
	case '2':
	{
		gShadingModel = Flat;
		break;
	}
	default:
		break;
	}
	glutPostRedisplay();
}

void AmbientColorMenuProcessFunc(int value)
{
	int tag = 0;
	GLint params[2];
	switch (value)
	{
	case '1':
	{
		gAmbientColor = gColorWhite;
		break;
	}
	case '2':
	{
		gAmbientColor = gColorGray;
		break;
	}
	case '3':
	{
		gAmbientColor = gColorRed;
		break;
	}
	case '4':
	{
		gAmbientColor = gColorGreen;
		break;
	}
	case '5':
	{
		gAmbientColor = gColorBlue;
		break;
	}
	case '6':
	{
		gAmbientColor = gColorBlack;
		break;
	}
	default:
		break;
	}
	glutPostRedisplay();
}

void DiffuseColorMenuProcessFunc(int value)
{
	int tag = 0;
	GLint params[2];
	switch (value)
	{
	case '1':
	{
		gDiffuseColor = gColorWhite;
		break;
	}
	case '2':
	{
		gDiffuseColor = gColorGray;
		break;
	}
	case '3':
	{
		gDiffuseColor = gColorRed;
		break;
	}
	case '4':
	{
		gDiffuseColor = gColorGreen;
		break;
	}
	case '5':
	{
		gDiffuseColor = gColorBlue;
		break;
	}
	case '6':
	{
		gDiffuseColor = gColorBlack;
		break;
	}
	default:
		break;
	}
	glutPostRedisplay();
}

void SpecularColorMenuProcessFunc(int value)
{
	int tag = 0;
	GLint params[2];
	switch (value)
	{
	case '1':
	{
		gSpecularColor = gColorWhite;
		break;
	}
	case '2':
	{
		gSpecularColor = gColorGray;
		break;
	}
	case '3':
	{
		gSpecularColor = gColorRed;
		break;
	}
	case '4':
	{
		gSpecularColor = gColorGreen;
		break;
	}
	case '5':
	{
		gSpecularColor = gColorBlue;
		break;
	}
	case '6':
	{
		gSpecularColor = gColorBlack;
		break;
	}
	default:
		break;
	}
	glutPostRedisplay();
}

void CreateWindowMenu()
{
	// light
	GLuint lightMenu = glutCreateMenu(LightMenuProcessFunc);
	glutAddMenuEntry("On", '1');
	glutAddMenuEntry("Off", '2');

	// lighting Model
	GLuint lightingModelMenu = glutCreateMenu(LightingModelMenuProcessFunc);
	glutAddMenuEntry("Lambert Model", '1');
	glutAddMenuEntry("Phong Model", '2');

	// shading Model
	GLuint shadingModelMenu = glutCreateMenu(ShadingModelMenuProcessFunc);
	glutAddMenuEntry("Smooth Model", '1');
	glutAddMenuEntry("Flat Model", '2');

	// ambient color
	GLuint ambientColorMenu = glutCreateMenu(AmbientColorMenuProcessFunc);
	glutAddMenuEntry("White", '1');
	glutAddMenuEntry("Gray", '2');
	glutAddMenuEntry("Red", '3');
	glutAddMenuEntry("Green", '4');
	glutAddMenuEntry("Blue", '5');
	glutAddMenuEntry("Black", '6');


	// diffuse color
	GLuint diffuseColorMenu = glutCreateMenu(DiffuseColorMenuProcessFunc);
	glutAddMenuEntry("White", '1');
	glutAddMenuEntry("Gray", '2');
	glutAddMenuEntry("Red", '3');
	glutAddMenuEntry("Green", '4');
	glutAddMenuEntry("Blue", '5');
	glutAddMenuEntry("Black", '6');


	// specular color
	GLuint specularColorMenu = glutCreateMenu(SpecularColorMenuProcessFunc);
	glutAddMenuEntry("White", '1');
	glutAddMenuEntry("Gray", '2');
	glutAddMenuEntry("Red", '3');
	glutAddMenuEntry("Green", '4');
	glutAddMenuEntry("Blue", '5');
	glutAddMenuEntry("Black", '6');


	glutCreateMenu(MenuProcessFunc);
	glutAddSubMenu("Light", lightMenu);
	glutAddSubMenu("Lighting Model", lightingModelMenu);
	glutAddSubMenu("Shading Model", shadingModelMenu);
	glutAddSubMenu("Ambient Color", ambientColorMenu);
	glutAddSubMenu("Diffuse Color", diffuseColorMenu);
	glutAddSubMenu("Specular Color", specularColorMenu);

	glutAddMenuEntry("Reset", '1');
	glutAddMenuEntry("Load Model", '2');

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void CreateWindow1()
{
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("FIXED PIPELINE");
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyboardFunc);
	CreateWindowMenu();
}

void CreateWindow2()
{
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(1000, 100);
	glutCreateWindow("GLSL");
	glutDisplayFunc(DisplayShader);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyboardFunc);
	CreateWindowMenu();
}

bool InitGlew()
{
	GLuint error_code = GLEW_OK;
	if ((error_code = glewInit()) != GLEW_OK)
	{
		printf("glew init error: %s\n", glewGetErrorString(error_code));
		return false;
	}
	return true;
}

//main function
int main(int argc, char** argv)
{	
	if(argc < 2)//read defult obj file which is bunny.obj
	{
		strcpy(gObjPath, "bunny.obj");
	}
	else
	{
		strcpy(gObjPath, argv[1]);
	}

	LoadMesh();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	
	CreateWindow1();
	CreateWindow2();
	if (!InitGlew())
	{
		return 1;
	}
	InitShaderList();
	InitRenderBuffers();
	UpdateRenderBuffers();
	glutPostRedisplay();
	glutMainLoop();
	return 0;
}
