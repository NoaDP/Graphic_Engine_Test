#version 330
 
in vec3 vertexPos;
in vec3 normal;
in int texCoord;
out vec2 texCoordAux2;
out vec3 normalCam;
out vec3 vertexCam;
out float visibility;

uniform mat4 modelview;
uniform mat4 projection;

const float density = 0.1f;
const float gradient = 3.0f;

void main()
{	
	// Posició del vèrtex actual
	gl_Position = projection * modelview * vec4( vertexPos , 1.0 );
	vec4 positionRelativeToCam = modelview * vec4( vertexPos , 1.0 );
	//calculem la matriu normalMatrix
	mat3 NormalMatrix = transpose(inverse(mat3(modelview)));
	
	// Càlcul de la posicio i normal en coordenades de camera
	//passem el vertex a coordenades de camera
	vertexCam = vec3(modelview * vec4( vertexPos , 1.0 ));       
	
	//passem la normal a coordenades de camera
	normalCam = normalize(NormalMatrix * normal);
	
	vec2 texPosition = vec2(texCoord, 0.0f);
	texCoordAux2 = texPosition;

	//Boira
	float distance = length(positionRelativeToCam.xyz);
	//Fog Factor
	visibility = exp(-pow((distance*density),gradient)); 
	visibility = clamp(visibility,0.0f,1.0f);

}
