#version 330

uniform vec3 specular;
uniform vec3 diffuse;
uniform vec3 ambient;
uniform float shininess;
uniform float alfa;
uniform vec2 texture;
uniform sampler2D mytex2;
uniform vec3 colorFons;

in vec3 normalCam;
in vec3 vertexCam;
in vec2 texCoordAux2;
in float visibility;

out vec4 fragColor;


void main(void)
{
	
	// Utilitza el color interpolat que prové del vertex shader
	
	vec3 posllum, lightvector, llumfinal, poscamera, l, n, h, v;
	float powLs, intensitat, li, f, end, start;

	posllum = vec3(10.0f, -5.0f,30.0f);
	//posllum = vec3(15.0f, 0.0f, -5.0f);
	intensitat = 300.0f;
	lightvector = vec3(posllum - vertexCam);
	float denom = 4*3.14159265 * length(lightvector);
	li = intensitat / denom;
	poscamera = normalize(vec3(0.0f,0.0f,0.0f));
	vec3 L = normalize(vec3(posllum - vertexCam));
	v = normalize(poscamera - vertexCam);
	h = normalize (v+L);
	
	
	float NH = dot(normalCam,h);
	float NL = dot(normalCam,L);
	
	
	// ens assegurem que NL no sigui negatiu
	/*if ( NL < 0){
		NL = 0;
	}*/
	if (NH < 0){
		NH = 0.01;
	}
	if (shininess < 0.01){
		powLs = 1;
	}else{
		powLs = pow(NH, shininess);
	}
	vec4 texResult2 = texture2D(mytex2,texCoordAux2);
	//L= vec3(0.0,0.0,1.0);

	llumfinal.x = (ambient.x + (((texResult2.x*diffuse.x)*NL) + (specular.x*powLs)))*li;
	llumfinal.y = (ambient.y + (((texResult2.y*diffuse.y)*NL) + (specular.y*powLs)))*li;
	llumfinal.z = (ambient.z + (((texResult2.z*diffuse.z)*NL) + (specular.z*powLs)))*li;
	/*
	llumfinal.x = (ambient.x + ((diffuse.x*NL) + (specular.x*powLs)))*li;
	llumfinal.y = (ambient.y + ((diffuse.y*NL) + (specular.y*powLs)))*li;
	llumfinal.z = (ambient.z + ((diffuse.z*NL) + (specular.z*powLs)))*li;
	*/
	
	fragColor = vec4(llumfinal, alfa);

	//Afegim la boira
	if(alfa==1){
	fragColor = mix(vec4(colorFons,1.0f),fragColor, visibility);
	}


}
