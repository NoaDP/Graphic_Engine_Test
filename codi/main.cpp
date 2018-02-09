#include "loadShader.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include <GL/glut.h>
#include <math.h>
#include <conio.h>
#include <IL/il.h>
#include <Windows.h>
#include <MMSystem.h>

typedef struct {
	GLuint g_Vao = 0;
	int numTriangles = 0;
	glm::vec3 color;
	glm::vec3 diffuse;
	glm::vec3 ambient;
	GLfloat shininess;
	glm::vec3 specular;
	GLfloat alfa;
	bool tex;
	GLfloat textureId;
}objeto;



// ------------------------------------------------------------------------------------------
// Variables globals
// ------------------------------------------------------------------------------------------

float angle;
float tiempoFrameAnterior = 0; //temps1
float tiempoFrame = 0; //temps2
float dx; //deplaçament
int tecla_R;
int tecla_L;
int girat_R;
int girat_L;
float r = 5, t = 5, n = 5;
float l = -5, f = -5, b = -5;
float rp = 1, tp = 1, np = -1;
float lp = -1, fp = -15, bp = -1;
float vel = 1;
float i = 0;
float j;
float c = 0;
int d = 0;
int es3 = 0;
int desp_anterior = 0;
int compt_cizalla = 0;
int derecha = 0;
int mov_anterior = 0;
int pos_anterior = 0;
int n_fragment = 0;
int tipusReshape = 1;
GLuint id;

//MATRIUS
glm::mat4 modelview_inicial = glm::mat4(1.0f);
glm::mat4 identidad = glm::mat4(1.0f);
glm::mat4 modelview = glm::mat4(0.01f); //apliquem una reduccio d'escala 
glm::mat4 modelview_t = glm::mat4(1.0f);//traslladem el cotxe al centre de la imatge
glm::mat4 modelview_top = glm::mat4(1.0f);//Visió Top
glm::mat4 modelview_gir = glm::mat4(1.0f);//Visió Top
glm::mat4 modelview_rotacio = glm::mat4(1.0f);//Rotacio
glm::mat4 m_rotacio = glm::mat4(1.0f);//Rotació / Temps
glm::mat4 m_translacio = glm::mat4(1.0f);//traslacio /temps
glm::mat4 modelview2 = glm::mat4(1.0f);
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 down = glm::mat4(1.0f);
glm::mat4 campfire = glm::mat4(1.0f);
glm::mat4 campfireR = glm::mat4(1.0f);


//camaras
glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 up = glm::mat4(1.0f);
glm::mat4 cam_Projection = glm::mat4(1.0f);
glm::mat4 Projeccio_ortogonal = glm::mat4(1.0f);
glm::mat4 cam_Ortogonal = glm::mat4(1.0f);
glm::mat4 m_projection = glm::mat4(1.0f);
glm::mat4 cam_rotacio = glm::mat4(1.0f);
glm::mat4 camera_down = glm::mat4(1.0f);
glm::mat4 camera_enretira = glm::mat4(1.0f);
glm::mat4 camera_traslacio = glm::mat4(1.0f);

// creacio de la carretera
glm::mat4 carretera_amunt = glm::mat4(1.0f);
glm::mat4 shaer = glm::mat4(1.0f);
glm::mat4 carretera_desplacament = glm::mat4(1.0f);
int fragment[8];
glm::mat4 cizalla = glm::mat4(1.0f);
glm::mat4 posInit = glm::mat4(1.0f);
typedef struct{
	glm::mat4 cizalla = glm::mat4(1.0f);
	glm::mat4 desplazamiento = glm::mat4(1.0f);
	glm::mat4 posicion = glm::mat4(1.0f);
}fragmento;
int valorcizalla[1000];
glm::mat4 posInitCarretera = glm::mat4(1.0f);


//FLAGS
bool right;
bool left;
bool perspectiva;

// Ample i alt de la finestra per defecte
int g_ViewportWidth = 512;
int g_ViewportHeight = 512;
float width;
float height;
float aspect = (float)g_ViewportWidth / (float)g_ViewportHeight;

// Color de fons
const glm::vec3 g_ColorFons(0.1f, 0.2f, 0.4f);



// Identificador del programa de shaders
GLuint g_ShaderProgram = 0;

// Identificador del Vertex Array Object (VAO) que conté la geometria
GLuint g_Vao = 0;
GLuint g_VaoSuelo = 0;

// Nombre de triangles de la malla
GLuint g_NumTriangles = 0;
int numTriangles = 0;




//textures
GLuint textureID;
ILuint imageID;
unsigned char *textura;
int t_width;
int t_height;

int nObjetos = 0;
objeto* objetos;
int compt = 0;
int casella = 0;
int nObjetos2 = 0;
objeto* objetos2;


//

GLuint loadImage(const char* path) {
	
	char tpath[512] = "../assets/";
	strcat(tpath, path);
	printf(tpath);
	printf("\n");
	GLuint imageId = ilGenImage();
	ilBindImage(imageId);

	if (ilLoadImage((ILstring)tpath)) {
		bool success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		if (!success) {
			exit(-1);
		}

		int width = ilGetInteger(IL_IMAGE_WIDTH);
		int height = ilGetInteger(IL_IMAGE_HEIGHT);

		GLfloat* pixels = (GLfloat*)malloc(sizeof(GLfloat)*width*height * 3);
		ilCopyPixels(0, 0, 0, width, height, 1, IL_RGB, IL_UNSIGNED_BYTE, pixels);
	

		GLuint textureID;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

		glGenerateMipmap(GL_TEXTURE_2D);

		free(pixels);
		ilDeleteImages(1, &imageId);
		return textureID;
		
	}
	else {
		std::cout << ilGetError();
		printf("\n");
		return 0;
	}
	

}
GLuint loadCarretera(const char* path) {

	char tpath[512] = "../assets/";
	strcat(tpath, path);
	printf(tpath);
	printf("\n");
	
	GLuint imageId = ilGenImage();
	ilBindImage(imageId);

	if (ilLoadImage((ILstring)tpath)) {
		bool success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		if (!success) {
			exit(-1);
		}

		int width = ilGetInteger(IL_IMAGE_WIDTH);
		int height = ilGetInteger(IL_IMAGE_HEIGHT);

		GLfloat* pixels = (GLfloat*)malloc(sizeof(GLfloat)*width*height * 3);
		ilCopyPixels(0, 0, 0, width, height, 1, IL_RGB, IL_UNSIGNED_BYTE, pixels);
		
		GLuint textureID;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

		glGenerateMipmap(GL_TEXTURE_2D);
		free(pixels);
		ilDeleteImages(1, &imageId);
		return textureID;

	}
	else {
		std::cout << ilGetError();
		printf("\n");
		return 0;
	}


}

glm::vec3 DistanciaVertices(aiVector3D vertice2, aiVector3D vertice1) {
	float difX, difY, difz;
	glm::vec3 vector;
	difX = vertice2.x - vertice1.x;
	difY = vertice2.y - vertice1.y;
	difz = vertice2.z - vertice1.z;
	vector.x = difX;
	vector.y = difY;
	vector.z = difz;

	return vector;
}


void creaSuelo() {
	
	const GLfloat verticesSuelo[] = {
		//Carreterra (1 / dimensio_p)
		-100.0f, -20.0f, -0.01f,//31
		-100.f, 20.0f, -0.01f,//32
		100.0f, -20.0f,-0.01f,//33
		100.0f,20.0f, -0.01f//34
	};


	const GLuint indicesSuelo[] = { 0, 1, 2, 1, 2, 3 };

	//Passa la informacio al fragment shader
	GLuint buffer;

	// Crea vao on posarem tota la geometria
	glGenVertexArrays(1, &g_VaoSuelo);
	glBindVertexArray(g_VaoSuelo);

	// Crea VBO per vèrtexs
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesSuelo), verticesSuelo, GL_STATIC_DRAW);

	// Activa atribut de vèrtexs
	GLuint vertexLoc = glGetAttribLocation(g_ShaderProgram, "vertexPos");
	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Crea VBO per índexs
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesSuelo), indicesSuelo, GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void carregaModels() {
	
	const GLfloat vertices[] = {
		//Carretera (1 / dimensio_p)
		2.0f, 0.0f, 0.0f,//31
		-2.f, 0.0f, 0.0f,//32
		2.0f, -5.0f,0.0f,//33
		-2.0f,-5.0f, 0.0f//34
	};


	const GLuint indices[] = { 0, 1, 2, 1, 2, 3};

	const GLfloat texturas[] = {
		0.0f, 0.0f,
		2048.0f, 0.0f,
		2048.0f, 2048.0f,
		0.0f, 2048.0f //34
	};
	//Passa la informacio al fragment shader
	GLuint buffer;

	// Crea vao on posarem tota la geometria
	glGenVertexArrays(1, &g_Vao);
	glBindVertexArray(g_Vao);

	// Crea VBO per vèrtexs
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Activa atribut de vèrtexs
	GLuint vertexLoc = glGetAttribLocation(g_ShaderProgram, "vertexPos");
	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Crea VBO per índexs
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Crea VBO per coordenades de textura
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texturas), texturas, GL_STATIC_DRAW);

	// Activa atribut de coordenades de textura
	GLuint texLoc = glGetAttribLocation(g_ShaderProgram, "texCoord");
	glEnableVertexAttribArray(texLoc);
	glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);

	g_NumTriangles = sizeof(indices) / (sizeof(GLuint) * 3);

	id = loadCarretera("Road3.png");
	glEnable(GL_TEXTURE_2D);
	ilDeleteImages(1, &id);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------//
	
	//Importem el cotxe
Assimp::Importer importer;
const aiScene* scene =
importer.ReadFile("../assets/Gta-spano-2010 obj.obj", aiProcessPreset_TargetRealtime_Fast | aiProcess_Triangulate);
if (!scene)
{
	std::cout << importer.GetErrorString();
	return;
}

nObjetos = scene->mNumMeshes;
objetos = new objeto[nObjetos];

for (int o = 0; o < nObjetos; o++) {

	// Crea vao on posarem tota la geometria
	glGenVertexArrays(1, &objetos[o].g_Vao);
	glBindVertexArray(objetos[o].g_Vao);

	aiMesh* malla = scene->mMeshes[o];


	//Recorrem totes les meshes

	GLfloat* array_vertex_normal = (GLfloat *)malloc(sizeof(GLfloat) * malla->mNumVertices * 3);
	glm::vec3* array_face_normals = (glm::vec3*)malloc(sizeof(glm::vec3) * malla->mNumFaces);
	
	aiColor4D aux;
	//obtenemos el color difuso
	aiMaterial* material = scene->mMaterials[malla->mMaterialIndex];
	aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &aux);
	objetos[o].diffuse.x = aux.r;
	objetos[o].diffuse.y = aux.g;
	objetos[o].diffuse.z = aux.b;

	//obtenemos el color ambiente
	aiMaterial* material2 = scene->mMaterials[malla->mMaterialIndex];
	aiGetMaterialColor(material2, AI_MATKEY_COLOR_AMBIENT, &aux);
	objetos[o].ambient.x = aux.r;
	objetos[o].ambient.y = aux.g;
	objetos[o].ambient.z = aux.b;

	//obtenemos la exponente especular
	aiMaterial* material3 = scene->mMaterials[malla->mMaterialIndex];
	material3->Get(AI_MATKEY_SHININESS, objetos[o].shininess);

	//obtenemos el color especular
	aiMaterial* material4 = scene->mMaterials[malla->mMaterialIndex];
	aiGetMaterialColor(material4, AI_MATKEY_COLOR_SPECULAR, &aux);
	objetos[o].specular.x = aux.r;
	objetos[o].specular.y = aux.g;
	objetos[o].specular.z = aux.b;

	//obtenemos el parámetro alfa
	aiMaterial* material5 = scene->mMaterials[malla->mMaterialIndex];
	material5->Get(AI_MATKEY_OPACITY, objetos[o].alfa);



	GLfloat* vertices = (GLfloat*)malloc(sizeof(GLfloat) * 3 * malla->mNumVertices);


	for (int i = 0; i < malla->mNumVertices; i++) {
	//	aiVector3D& mVectices = malla->mVertices[i];
		vertices[i * 3] = malla->mVertices[i].x;
		vertices[i * 3 + 1] = malla->mVertices[i].y;
		vertices[i * 3 + 2] = malla->mVertices[i].z;
	}

	//Envia al fragment shader
	GLuint buffer;
	/////////////////////////////


	// Crea VBO per vèrtexs
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * malla->mNumVertices, vertices, GL_STATIC_DRAW);

	// Activa atribut de vèrtexs
	GLuint vertexLoc = glGetAttribLocation(g_ShaderProgram, "vertexPos");
	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);


	GLuint* indices = (GLuint*)malloc(malla->mNumFaces * 3 * sizeof(GLuint));

	for (int i = 0; i < malla->mNumFaces; i++) {
		aiFace& vector_face = malla->mFaces[i];
		if (vector_face.mNumIndices == 3) {
			indices[i * 3] = vector_face.mIndices[0];
			indices[i * 3 + 1] = vector_face.mIndices[1];
			indices[i * 3 + 2] = vector_face.mIndices[2];

			objetos[o].numTriangles++;
		}
	}

	
	objetos[o].numTriangles = malla->mNumFaces;

	//Calculem normals de los vetices a partir de las normales de las caras
	
	for (int i = 0; i < malla->mNumFaces; i++) {
		array_face_normals[i].x = 0.0f;
		array_face_normals[i].y = 0.0f;
		array_face_normals[i].z = 0.0f;
		glm::vec3 vec_Aux;
		aiVector3D A = malla->mVertices[malla->mFaces[i].mIndices[0]];
		aiVector3D B = malla->mVertices[malla->mFaces[i].mIndices[1]];
		aiVector3D C = malla->mVertices[malla->mFaces[i].mIndices[2]];
		glm::vec3 vec1 = DistanciaVertices(B, A);
		glm::vec3 vec2 = DistanciaVertices(C, A);
		vec_Aux = glm::normalize(glm::cross(vec1, vec2));
		array_face_normals[i].x = vec_Aux.x;
		array_face_normals[i].y = vec_Aux.y;
		array_face_normals[i].z = vec_Aux.z;
	}

	for (int i = 0; i < malla->mNumVertices; i++) {
		array_vertex_normal[i * 3] = 0.0f;
		array_vertex_normal[i * 3 + 1] = 0.0f;
		array_vertex_normal[i * 3 + 2] = 0.0f;
		for (int j = 0; j < malla->mNumFaces; j++) {
			if (malla->mFaces[j].mIndices[0] == i || malla->mFaces[j].mIndices[1] == i || malla->mFaces[j].mIndices[2] == i) {
				array_vertex_normal[i * 3] += array_face_normals[j].x;
				array_vertex_normal[i * 3 + 1] += array_face_normals[j].y;
				array_vertex_normal[i * 3 + 2] += array_face_normals[j].z;
			}
		}
	}

	for (int i = 0; i < malla->mNumVertices; i++){
		glm::vec3 array_normal = glm::vec3(array_vertex_normal[i * 3], array_vertex_normal[i * 3 + 1], array_vertex_normal[i * 3 + 2]);
		array_normal = glm::normalize(array_normal);
		array_vertex_normal[i * 3] = array_normal.x;
		array_vertex_normal[i * 3 + 1] = array_normal.y;
		array_vertex_normal[i * 3 + 2] = array_normal.z;
	}

	// Crea VBO per índexs
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)*objetos[o].numTriangles * 3, indices, GL_STATIC_DRAW);

	//creem el VBO de vertexNormals
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * malla->mNumVertices, array_vertex_normal, GL_STATIC_DRAW);

	// Activa atribut de vèrtexs
	GLuint vertexNormals = glGetAttribLocation(g_ShaderProgram, "normal");
	glEnableVertexAttribArray(vertexNormals);
	glVertexAttribPointer(vertexNormals, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//tratamos el fichero de textura
	aiString path;
	GLfloat* texCoord = (GLfloat*)malloc(sizeof(GLfloat) * (malla->mNumVertices * 2));

	if (AI_SUCCESS == scene->mMaterials[malla->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &path)) {
		objetos[o].tex = true;
		objetos[o].textureId = loadImage(path.C_Str());
		if (objetos[o].textureId > 0) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, objetos[o].textureId);
		
		}
	}
	else {
		objetos[o].tex = false;
	}

	GLuint k = 0;
	for (GLuint j = 0; j < malla->mNumVertices; j++) {
		texCoord[k] = malla->mTextureCoords[0][j].x;
		k++;
		texCoord[k] = malla->mTextureCoords[0][j].y;
		k++;
	}

	// Crea VBO per coordenades de textura
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (malla->mNumVertices * 2), texCoord, GL_STATIC_DRAW);

	// Activa atribut de coordenades de textura
	GLuint texLoc = glGetAttribLocation(g_ShaderProgram, "texCoord");
	glEnableVertexAttribArray(texLoc);
	glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	

	free(indices);
	free(vertices);
	free(array_face_normals);
	free(array_vertex_normal);
	free(texCoord);

	}
	
	//Cargamos el sonido
	//PlaySound(TEXT("../assets/vehicle091.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

// ------------------------------------------------------------------------------------------
// Funció que carrega els recursos utilitzats en el programa
// ------------------------------------------------------------------------------------------
void carregaExtra() {

	//Importem el cotxe
	Assimp::Importer importer;
	const aiScene* scene2 =
		importer.ReadFile("../assets/pedestal2.obj", aiProcessPreset_TargetRealtime_Fast | aiProcess_Triangulate);
	if (!scene2)
	{
		std::cout << importer.GetErrorString();
		return;
	}

	nObjetos2 = scene2->mNumMeshes;
	objetos2 = new objeto[nObjetos2];

	for (int o = 0; o < nObjetos2; o++) {

		// Crea vao on posarem tota la geometria
		glGenVertexArrays(1, &objetos2[o].g_Vao);
		glBindVertexArray(objetos2[o].g_Vao);

		aiMesh* malla = scene2->mMeshes[o];

		//Recorrem totes les meshes
		GLfloat* array_vertex_normal = (GLfloat *)malloc(sizeof(GLfloat) * malla->mNumVertices * 3);
		glm::vec3* array_face_normals = (glm::vec3*)malloc(sizeof(glm::vec3) * malla->mNumFaces);

		aiColor4D aux;

		//obtenemos el color difuso
		aiMaterial* material = scene2->mMaterials[malla->mMaterialIndex];
		aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &aux);
		objetos2[o].diffuse.x = aux.r;
		objetos2[o].diffuse.y = aux.g;
		objetos2[o].diffuse.z = aux.b;

		//obtenemos el color ambiente
		aiMaterial* material2 = scene2->mMaterials[malla->mMaterialIndex];
		aiGetMaterialColor(material2, AI_MATKEY_COLOR_AMBIENT, &aux);
		objetos2[o].ambient.x = aux.r;
		objetos2[o].ambient.y = aux.g;
		objetos2[o].ambient.z = aux.b;

		//obtenemos la exponente especular
		aiMaterial* material3 = scene2->mMaterials[malla->mMaterialIndex];
		material3->Get(AI_MATKEY_SHININESS, objetos2[o].shininess);

		//obtenemos el color especular
		aiMaterial* material4 = scene2->mMaterials[malla->mMaterialIndex];
		aiGetMaterialColor(material4, AI_MATKEY_COLOR_SPECULAR, &aux);
		objetos2[o].specular.x = aux.r;
		objetos2[o].specular.y = aux.g;
		objetos2[o].specular.z = aux.b;

		//obtenemos el parámetro alfa
		aiMaterial* material5 = scene2->mMaterials[malla->mMaterialIndex];
		material5->Get(AI_MATKEY_OPACITY, objetos2[o].alfa);



		GLfloat* vertices = (GLfloat*)malloc(sizeof(GLfloat) * 3 * malla->mNumVertices);


		for (int i = 0; i < malla->mNumVertices; i++) {
			aiVector3D& mVectices = malla->mVertices[i];
			vertices[i * 3] = malla->mVertices[i].x;
			vertices[i * 3 + 1] = malla->mVertices[i].y;
			vertices[i * 3 + 2] = malla->mVertices[i].z;
		}

		//Envia al fragment shader
		GLuint buffer;
		/////////////////////////////


		// Crea VBO per vèrtexs
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * malla->mNumVertices, vertices, GL_STATIC_DRAW);

		// Activa atribut de vèrtexs
		GLuint vertexLoc = glGetAttribLocation(g_ShaderProgram, "vertexPos");
		glEnableVertexAttribArray(vertexLoc);
		glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);


		GLuint* indices = (GLuint*)malloc(malla->mNumFaces * 3 * sizeof(GLuint));

		for (int i = 0; i < malla->mNumFaces; i++) {
			aiFace& vector_face = malla->mFaces[i];
			if (vector_face.mNumIndices == 3) {
				indices[i * 3] = vector_face.mIndices[0];
				indices[i * 3 + 1] = vector_face.mIndices[1];
				indices[i * 3 + 2] = vector_face.mIndices[2];

				objetos2[o].numTriangles++;
			}
		}


		objetos2[o].numTriangles = malla->mNumFaces;

		//Calculem normals de los vetices a partir de las normales de las caras

		for (int i = 0; i < malla->mNumFaces; i++) {
			array_face_normals[i].x = 0.0f;
			array_face_normals[i].y = 0.0f;
			array_face_normals[i].z = 0.0f;
			glm::vec3 vec_Aux;
			aiVector3D A = malla->mVertices[malla->mFaces[i].mIndices[0]];
			aiVector3D B = malla->mVertices[malla->mFaces[i].mIndices[1]];
			aiVector3D C = malla->mVertices[malla->mFaces[i].mIndices[2]];
			glm::vec3 vec1 = DistanciaVertices(B, A);
			glm::vec3 vec2 = DistanciaVertices(C, A);
			vec_Aux = glm::cross(vec1, vec2);
			array_face_normals[i].x = vec_Aux.x;
			array_face_normals[i].y = vec_Aux.y;
			array_face_normals[i].z = vec_Aux.z;
		}

		for (int i = 0; i < malla->mNumVertices; i++) {
			array_vertex_normal[i * 3] = 0.0f;
			array_vertex_normal[i * 3 + 1] = 0.0f;
			array_vertex_normal[i * 3 + 2] = 0.0f;
			for (int j = 0; j < malla->mNumFaces; j++) {
				if (malla->mFaces[j].mIndices[0] == i || malla->mFaces[j].mIndices[1] == i || malla->mFaces[j].mIndices[2] == i) {
					array_vertex_normal[i * 3] += array_face_normals[j].x;
					array_vertex_normal[i * 3 + 1] += array_face_normals[j].y;
					array_vertex_normal[i * 3 + 2] += array_face_normals[j].z;
				}
			}
			glm::vec3 array_normal = glm::vec3(array_vertex_normal[i * 3], array_vertex_normal[i * 3 + 1], array_vertex_normal[i * 3 + 2]);
			array_normal = glm::normalize(array_normal);
			array_vertex_normal[i * 3] = array_normal.x;
			array_vertex_normal[i * 3 + 1] = array_normal.y;
			array_vertex_normal[i * 3 + 2] = array_normal.z;
		}

		// Crea VBO per índexs
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)*objetos2[o].numTriangles * 3, indices, GL_STATIC_DRAW);

		//creem el VBO de vertexNormals
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * malla->mNumVertices, array_vertex_normal, GL_STATIC_DRAW);

		// Activa atribut de vèrtexs
		GLuint vertexNormals = glGetAttribLocation(g_ShaderProgram, "normal");
		glEnableVertexAttribArray(vertexNormals);
		glVertexAttribPointer(vertexNormals, 3, GL_FLOAT, GL_FALSE, 0, 0);

		

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		free(indices);
		free(vertices);
		free(array_face_normals);
		free(array_vertex_normal);
	}
	
	//Cargamos el sonido
	PlaySound(TEXT("../assets/vehicle091.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

}
void loadResources()
{
	// Carrega els shaders i crea el programa de shaders
	g_ShaderProgram = loadShaders("shader.vert", "shader.frag");

	carregaModels();
	carregaExtra();
	creaSuelo();
}

// --------------------------------------------------------------
// Callback de pintat
// --------------------------------------------------------------

void camara_p(unsigned char key, int x, int y) {

	if (key == 27)
	{
		// Surt del programa al premer ESC (Key 27)
		exit(EXIT_SUCCESS);
	}
	if (key == 'p' && perspectiva == TRUE) {
		tipusReshape = 2;
		projection = cam_Projection;
	}
	if (key == 'p' && perspectiva == FALSE) {
		tipusReshape = 1;
		projection = cam_Ortogonal;
	}
	if (perspectiva == TRUE) {
		perspectiva = FALSE;
	}
	else {
		perspectiva = TRUE;
	}
}

// --------------------------------------------------------------
// Callback canvi mida de finestra
// --------------------------------------------------------------

void onReshape(int w, int h)
{

	width = w;
	height = h;

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

}


void onIdle()
{
	//onDisplay();
	
	glutKeyboardUpFunc(camara_p);

	// Se llama a glut para que al escalar la ventana esta se ajuste a la nueva zona -> devuelve with i height que serviaran para volver a definir el viewport (nuevo)
	//Codi que modifica l'estat del joc
	//POSICIO DEL COTXE
	
	//Matriu de rotació en l'eix Z
	m_rotacio[0][0] = cos(angle);
	m_rotacio[0][1] = sin(angle);
	m_rotacio[1][1] = cos(angle);
	m_rotacio[1][0] = -sin(angle);
	
	tiempoFrame = glutGet(GLUT_ELAPSED_TIME) - tiempoFrameAnterior;

	tiempoFrameAnterior = glutGet(GLUT_ELAPSED_TIME);

	//GIR DRET O ESQUERRE
	if (right == TRUE) {
		right = FALSE;
		tecla_R = 1;
		m_translacio[3][0] += tiempoFrame*vel / 200;
		camera_traslacio[3][0] -= tiempoFrame*vel / 200;
	}
	if (left == TRUE) {
		left = FALSE;
		tecla_L = 1;
		m_translacio[3][0] += tiempoFrame*-vel / 200;
		camera_traslacio[3][0] -= tiempoFrame*-vel / 200;
	}

	//Pinta per mostrat els canvis
	//fuerza que se pinte el frame del estado actual
	glutPostRedisplay();
	//Temps minim entre iteracions
	//como mucho vamos a tener 60 fps 
	Sleep(1000 / 60);   //Pausa la execución durante 10 milisegundos. Si evitamos el sleep, la memoria de la CPU se llenaria, 
						//por tanto evita el consumo execisivo de recursos. Como maximo 100 imagenes por segundo, por tanto 
						//modifica el frmewrite.
}

void reshapePerspectiva() {
	aspect = (float)width / (float)height;
	rp = 1 * aspect;

	//camara
	m_projection[0][0] = -np / rp;

	//projecció ortogonal
	cam_Projection = m_projection*camera_enretira*up;
	projection = cam_Projection;
}
void reshapeOrtogonal() {
	aspect = (float)width / (float)height;
	r = 5 * aspect;

	//camara
	cam_Ortogonal[0][0] = 1 / r;

	//projecció ortogonal
	cam_Ortogonal = cam_Ortogonal *Projeccio_ortogonal;
	projection = cam_Ortogonal;

}

void onDisplay()
{

	// Neteja el back buffer
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(g_ShaderProgram);

	glutInitDisplayMode(GLUT_DEPTH);
	glClearDepth(1);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);



	//importamos las columnas
	for (int p = 0; p < 2; p++) {
		if (p % 2 == 0) {
			campfire[3][0] = 6.0f;
		}
		else{
			campfire[3][0] = -4.0f;
		}
		
		campfire[3][1] = -c + 35.0f;
		//pintamos las columnas
		for (int i = 0; i < nObjetos2; i++) {
			// Selecciona el programa i VAO que s'utilitzaran al pintar
			glBindVertexArray(objetos2[i].g_Vao);
			//solo hemos de pasarle modelview

			float valcos = cos(3.141598 / 2);
			float valsin = sin(3.141598 / 2);

			//rotamos el coche
			
			//rotacion sobre el eje y 
			campfireR[0][1] = 0.0f;
			campfireR[1][1] = valcos;
			campfireR[1][0] = 0.0f;
			campfireR[2][2] = valcos;
			campfireR[2][1] = -valsin;
			campfireR[1][2] = valsin;

			modelview = identidad*camera_traslacio*campfire*campfireR;
			glUniformMatrix4fv(glGetUniformLocation(g_ShaderProgram, "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
			// solo hemos de pasarle la matriz de proyeccion
			glUniformMatrix4fv(glGetUniformLocation(g_ShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			glUniform3f(glGetUniformLocation(g_ShaderProgram, "specular"), objetos[i].specular.x, objetos[i].specular.y, objetos[i].specular.z);
			glUniform3f(glGetUniformLocation(g_ShaderProgram, "ambient"), objetos[i].ambient.x, objetos[i].ambient.y, objetos[i].ambient.z);
			glUniform3f(glGetUniformLocation(g_ShaderProgram, "diffuse"), objetos[i].diffuse.x, objetos[i].diffuse.y, objetos[i].diffuse.z);
			glUniform1f(glGetUniformLocation(g_ShaderProgram, "shininess"), objetos[i].shininess);
			glUniform1f(glGetUniformLocation(g_ShaderProgram, "alfa"), objetos[i].alfa);

			glUniform3f(glGetUniformLocation(g_ShaderProgram, "colorFons"), 0.0f, 0.2f, 0.4f);

			// Pinta al back buffer
			glDrawElements(GL_TRIANGLES, 3 * objetos[i].numTriangles, GL_UNSIGNED_INT, 0);
		}
	}
	
	

	//pintamos el coche
	for (int i = 0; i < nObjetos; i++) {
		// Selecciona el programa i VAO que s'utilitzaran al pintar
		glBindVertexArray(objetos[i].g_Vao);
		//solo hemos de pasarle modelview
		
		view = m_translacio*camera_traslacio*m_rotacio;
		model = modelview_inicial;
		modelview = view*model;
		
		glUniformMatrix4fv(glGetUniformLocation(g_ShaderProgram, "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
		
		// solo hemos de pasarle la matriz de proyeccion
		glUniformMatrix4fv(glGetUniformLocation(g_ShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3f(glGetUniformLocation(g_ShaderProgram, "specular"), objetos[i].specular.x, objetos[i].specular.y, objetos[i].specular.z);
		glUniform3f(glGetUniformLocation(g_ShaderProgram, "ambient"), objetos[i].ambient.x, objetos[i].ambient.y, objetos[i].ambient.z);
		glUniform3f(glGetUniformLocation(g_ShaderProgram, "diffuse"), objetos[i].diffuse.x, objetos[i].diffuse.y, objetos[i].diffuse.z);
		glUniform1f(glGetUniformLocation(g_ShaderProgram, "shininess"), objetos[i].shininess);
		
		glUniform3f(glGetUniformLocation(g_ShaderProgram, "colorFons"), 0.0f, 0.2f, 0.4f);

		if (objetos[i].tex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, objetos[i].textureId);
			glUniform1ui(glGetUniformLocation(g_ShaderProgram, "mytex2"), objetos[i].textureId);
		}
		glUniform1f(glGetUniformLocation(g_ShaderProgram, "alfa"), objetos[i].alfa);
		// Pinta al back buffer
		glDrawElements(GL_TRIANGLES, 3 * objetos[i].numTriangles, GL_UNSIGNED_INT, 0);
	}

	//Aplicamos la alfa
	/*
	for (int i = 0; i < nObjetos; i++) {
	// Selecciona el programa i VAO que s'utilitzaran al pintar
		glBindVertexArray(objetos[i].g_Vao);
		//solo hemos de pasarle modelview

		view = m_translacio*camera_traslacio*m_rotacio;
		model = modelview_inicial;
		modelview = view*model;

		glUniformMatrix4fv(glGetUniformLocation(g_ShaderProgram, "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));

		// solo hemos de pasarle la matriz de proyeccion
		glUniformMatrix4fv(glGetUniformLocation(g_ShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3f(glGetUniformLocation(g_ShaderProgram, "specular"), objetos[i].specular.x, objetos[i].specular.y, objetos[i].specular.z);
		glUniform3f(glGetUniformLocation(g_ShaderProgram, "ambient"), objetos[i].ambient.x, objetos[i].ambient.y, objetos[i].ambient.z);
		glUniform3f(glGetUniformLocation(g_ShaderProgram, "diffuse"), objetos[i].diffuse.x, objetos[i].diffuse.y, objetos[i].diffuse.z);
		glUniform1f(glGetUniformLocation(g_ShaderProgram, "shininess"), objetos[i].shininess);

		glUniform3f(glGetUniformLocation(g_ShaderProgram, "colorFons"), 0.0f, 0.2f, 0.4f);

		if (objetos[i].tex) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, objetos[i].textureId);
		glUniform1ui(glGetUniformLocation(g_ShaderProgram, "mytex2"), objetos[i].textureId);
	}
	glUniform1f(glGetUniformLocation(g_ShaderProgram, "alfa"), objetos[i].alfa);


	//Transarencies
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);

	if (objetos[i].alfa < 1.0f) {
	//Suma els dos colors
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Pinta al back buffer
	glDrawElements(GL_TRIANGLES, 3 * objetos[i].numTriangles, GL_UNSIGNED_INT, 0);
	glDisable(GL_BLEND);

	}

	}
	*/

	//calculamos las transformaciones de la carretera
	for (n_fragment = 0; n_fragment < 100; n_fragment++) {
		
		pos_anterior = carretera_desplacament[3][0];
		if (valorcizalla[n_fragment - 2] == 2 && valorcizalla[n_fragment -1] == 1 && valorcizalla[n_fragment] == 3 && valorcizalla[n_fragment +1] == 1) valorcizalla[n_fragment] = 1;
		posInit[3][2] = 0.0f;
		cizalla[1][2] = 0.0f;

		switch (valorcizalla[n_fragment]) {
		case 3:
			//cizalla derecha 
			cizalla[1][0] = 0.5f;
			posInit[3][0] = 2.5 + carretera_desplacament[3][0];
			//if (posInit[3][0] > 3) carretera_desplacament[3][0] = pos_anterior;
			if (derecha == 4) posInit[3][0] = 2.5 + carretera_desplacament[3][0];
			
			derecha = 1;
			break;
		case 2:
			//cizalla izquierda
			cizalla[1][0] = -0.5f;
			posInit[3][0] = -1.0 + carretera_desplacament[3][0];
			derecha = -1;
			cizalla[1][2] = 1.0f;

			//carretera_desplacament[3][0] = 2.5f;
		case 1:
			carretera_desplacament = glm::mat4(1.0f);
			if(derecha == 1) carretera_desplacament[3][0] = 2.5f;
			if (derecha == -1) carretera_desplacament[3][0] = 1.0f;
			derecha = 4;
			cizalla[1][2] = 0.0f;


			break;
		}
		
		//generamos los fragmentos de carretera y les aplicamos las transformaciones
		glBindVertexArray(g_Vao);
		carretera_desplacament[3][1] = -i + n_fragment*5;
	
		glUniformMatrix4fv(glGetUniformLocation(g_ShaderProgram, "modelview"), 1, GL_FALSE, glm::value_ptr(posInit*camera_traslacio*carretera_desplacament*cizalla));
		glUniformMatrix4fv(glGetUniformLocation(g_ShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	
		glUniform3f(glGetUniformLocation(g_ShaderProgram, "specular"), 0.7f, 0.7f,0.7f);
		glUniform3f(glGetUniformLocation(g_ShaderProgram, "ambient"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(g_ShaderProgram, "diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(g_ShaderProgram, "shininess"), 0.0f);
		glUniform1f(glGetUniformLocation(g_ShaderProgram, "alfa"), 1.0f);

		glUniform3f(glGetUniformLocation(g_ShaderProgram, "colorFons"), 0.1f, 0.2f, 0.3f);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, id);
		glUniform1ui(glGetUniformLocation(g_ShaderProgram, "mytex2"), id);

		glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);
		
		//reiniciamos valores
		desp_anterior = 0;
		cizalla[1][0] = 0;
		posInit[3][0] = 0;
		carretera_desplacament[3][0] = 0;
	}

	// forcem un bucle infinit que vagi printant la carretera
	j++;
	if (j == 5 ) {
		i++;
		c++;
		j = 0;
	}
	if (c > 50) c = 0;
	if (i > 150) {
		i = 0;
	
	}
	
	//definim el reshape per evitar deformacions
	if (tipusReshape == 1) reshapeOrtogonal();
	if (tipusReshape == 2) reshapePerspectiva();

	//hacemos el suelo
	glBindVertexArray(g_VaoSuelo);
	carretera_desplacament[3][1] = -i + n_fragment * 5;

	glUniformMatrix4fv(glGetUniformLocation(g_ShaderProgram, "modelview"), 1, GL_FALSE, glm::value_ptr(camera_traslacio));
	glUniformMatrix4fv(glGetUniformLocation(g_ShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUniform3f(glGetUniformLocation(g_ShaderProgram, "specular"), 0.9f, 0.9f, 0.9f);
	glUniform3f(glGetUniformLocation(g_ShaderProgram, "ambient"), 0.1f, 0.1f, 0.1f);
	glUniform3f(glGetUniformLocation(g_ShaderProgram, "diffuse"), 0.0f, 0.5f, 1.0f);
	glUniform1f(glGetUniformLocation(g_ShaderProgram, "shininess"), 0.0f);
	glUniform1f(glGetUniformLocation(g_ShaderProgram, "alfa"), 1.0f);
	glUniform3f(glGetUniformLocation(g_ShaderProgram, "colorFons"), 0.1f, 0.2f, 0.3f);


	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);

	
	glClear(GL_DEPTH_BUFFER_BIT);
	
	// Copia back buffer al front buffer
	glutSwapBuffers();
	glBindVertexArray(0);
	glUseProgram(0);
}


// --------------------------------------------------------------
// Callback entrada teclat
// --------------------------------------------------------------
// se activa cada vez que se suelta una tecla
//void onkeydown
void onKeyUp(unsigned char key, int x, int y)
{
	if (key == 27)		// ESC
	{
		// Surt del programa
		exit(EXIT_SUCCESS);
	}
}

void control_key(int key, int x, int y)
{

	switch (key)
	{
	case GLUT_KEY_RIGHT:
		angle = -0.7701; //45º
		right = TRUE;
		break;

	case GLUT_KEY_LEFT:
		angle = 0.7701; //90º + 45º
		left = TRUE;
		break;
	default: break;

	}
}

void control_off(int key, int x, int y)
{
	if (tecla_R == 1) {
		angle = 0;
		tecla_R = 0;
	}
	if (tecla_L == 1) {
		angle = 0;
		tecla_L = 0;
	}

	m_rotacio[0][0] = cos(angle);
	m_rotacio[0][1] = sin(angle);
	m_rotacio[1][1] = cos(angle);
	m_rotacio[1][0] = -sin(angle);

	glutPostRedisplay();
}



int main(int argc, char* argv[])
{
	//Definim les matrius de transformació

	modelview[3][3] = 1.0f;

	float valcos = cos(3.141598 / 2);
	float valsin = sin(3.141598 / 2);

	//rotamos el coche

	//rotacion sobre el eje y 
	modelview_rotacio[0][1] = 0.0f;
	modelview_rotacio[1][1] = valcos;
	modelview_rotacio[1][0] = 0.0f;
	modelview_rotacio[2][2] = valcos;
	modelview_rotacio[2][1] = -valsin;
	modelview_rotacio[1][2] = valsin;

	//rotamos alrededor de z 
	glm::mat4 modelview_rotacioAux = glm::mat4(1.0f);
	valcos = cos(3.141598);
	valsin = sin(3.141598);
	modelview_rotacioAux[0][0] = valcos;
	modelview_rotacioAux[1][0] = -valsin;
	modelview_rotacioAux[1][1] = valcos;
	modelview_rotacioAux[0][1] = -valsin;

	// col.loquem el cotxe  correctament
	modelview_rotacio = modelview_rotacioAux * modelview_rotacio;

	//CAMARAS

	// matriu de camara ortogonal 
	Projeccio_ortogonal[2][2] = 0.0f;
	Projeccio_ortogonal[3][2] = -1.f;


	//Fem servir les matrius

	modelview = modelview_rotacio * modelview;
	modelview_inicial = modelview;

	//camara
	cam_Ortogonal[0][0] = 1 / r;
	cam_Ortogonal[1][1] = 1 / t;
	cam_Ortogonal[2][2] = 2 / (f - n);
	cam_Ortogonal[3][2] = -(f + n) / (f - n);

	//projecció ortogonal
	cam_Ortogonal = cam_Ortogonal *Projeccio_ortogonal;
	projection = cam_Ortogonal;

	//Matriu projeccio perspectiva
	m_projection[0][0] = -np / rp;
	m_projection[1][1] = -np / tp;
	m_projection[2][2] = -(fp + np) / (fp - np);
	m_projection[3][3] = 0.0f;
	m_projection[3][2] = 2 * fp*np / (fp - np);
	m_projection[2][3] = -1.0f;

	//Rotació en l'eix y

	float valcos2 = cos(-3.141598 / 4);
	float valsin2 = sin(-3.141598 / 4);

	camera_enretira[0][1] = 0.0f;
	camera_enretira[1][1] = valcos2;
	camera_enretira[1][0] = 0.0f;
	camera_enretira[2][2] = valcos2;
	camera_enretira[2][1] = -valsin2;
	camera_enretira[1][2] = valsin2;
	camera_enretira[3][1] = -0.0f;

	//Pujem la camara en l'eix z i la transladem y
	up[3][0] = 0.0f;
	up[3][1] = 3.0f;
	up[3][2] = -4.f;

	//projeccio perspectiva
	cam_Projection = m_projection*camera_enretira*up;

	girat_R = 0;
	girat_L = 0;
	perspectiva = TRUE;

	//Devil
	ilInit();
	// Inicialitzem el entorn de glut per tal de crear una finestra
	glutInit(&argc, argv);
	glutInitContextVersion(2, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(600, 200);
	glutInitWindowSize(g_ViewportWidth, g_ViewportHeight);
	glutCreateWindow("Practica GFX");

	// Inicialitzem OpenGL
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
		return EXIT_FAILURE;
	}

	// Defineix el color de fons de la finestra
	glClearColor(g_ColorFons.x, g_ColorFons.y, g_ColorFons.z, 1.0f);

	//Boira
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Carreguem els recursos
	loadResources();

	// Definim els callbacks
	glutIdleFunc(onIdle); //Se llama siempre a glut para que surga la animacón sin depender. A cada idle se tiene que haber movido el coche proporcionalmente al tiempo entre dos idles
	glutDisplayFunc(onDisplay); // Se llama a glut para que pinte la pantalla solo si se llama a la funcion display
	glutSpecialFunc(control_key);
	glutSpecialUpFunc(control_off);

	//glutKeyboardUpFunc(onKeyUp);  // Se llama a glut unicamente cuando se pulsa una tecla
	glutReshapeFunc(onReshape); // Se llama a glut para que al escalar la ventana esta se ajuste a la nueva zona -> devuelve with i height que serviaran para volver a definir el viewport (nuevo)

	for (int i = 0; i < 1000; i++) {
		//valorcizalla [i] = rand() % 100 + 1;
		if (i % 2 == 0) {
			valorcizalla[i] = 1;
		}
		else {
			if (i % 3 == 0) {
				valorcizalla[i] = 2;
			}
			else {
				valorcizalla[i] = 3;
			}
		}
	}
	// Iniciem el bucle principal
	glutMainLoop();
	delete objetos;
	return EXIT_SUCCESS;
}



//NOTAS
/* Exercicio_3: La camara a de seguir al coche -> al aplicar la transformacion de camara (Trans*CambioBase) -> La Vista(camara) ha de depender del al posicion de choe. x=0 -> x=posCoche */