#version 330 core

#define MAX_LIGHTS 3

struct Light{ //Svjetlosni izvor
	vec3 pos; //Pozicija
	vec3 kA; //Ambijentalna komponenta (Indirektno svjetlo)
	vec3 kD; //Difuzna komponenta (Direktno svjetlo)
	vec3 kS; //Spekularna komponenta (Odsjaj)
	float constant;
	float linear;
	float quadratic;
};
struct LightDir {
	vec3 direction;
	vec3 kA;
	vec3 kD;
	vec3 kS;
};
struct LightRef {
	vec3 position;
	vec3 direction;
	float cutOff;
	vec3 kA;
	vec3 kD;
	vec3 kS;
};
struct Material{ //Materijal objekta
	vec3 kA;
	vec3 kD;
	vec3 kS;
	float shine; //Uglancanost
};

in vec3 chNor;
in vec3 chFragPos;

in vec2 chTex;

out vec4 outCol;

uniform Light uLight;
uniform LightDir uLightDir;
uniform LightRef uLightRef;
uniform Material uMaterial;
uniform vec3 uViewPos;	//Pozicija kamere (za racun spekularne komponente)

uniform bool uTextureEnabled;

uniform sampler2D uTex;

void main()
{
	vec3 totalResult = vec3(0.0);
	//tackasto osvetljenje
	vec3 resA;
	if (uTextureEnabled)
	{
		resA = uLight.kA;
	}
	else
	{
		resA = uLight.kA * uMaterial.kA;
	}
	
	vec3 normal = normalize(chNor);
	vec3 lightDirection = normalize(uLight.pos - chFragPos);
	float nD = max(dot(normal, lightDirection), 0.0);
	vec3 resD;
	if (uTextureEnabled)
	{
		resD = uLight.kD * nD;
	}
	else
	{
		resD = uLight.kD * ( nD * uMaterial.kD);
	}

	
	vec3 viewDirection = normalize(uViewPos - chFragPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float s = pow(max(dot(viewDirection, reflectionDirection), 0.0), uMaterial.shine);
	vec3 resS;
	if (uTextureEnabled)
	{
		resS = uLight.kS * s;
	}
	else
	{
		resS = uLight.kS * (s * uMaterial.kS);
	}


	float distance = length(uLight.pos - chFragPos);
	float attenuation = 1.0 / (uLight.constant + uLight.linear * distance + uLight.quadratic * (distance * distance));

	resA *= attenuation;
	resD *= attenuation;
	resS *= attenuation;
	
	totalResult += resA + resD + resS;

	//direkciono osvetljenje
	if (uTextureEnabled)
	{
		resA = uLightDir.kA;
	}
	else
	{
		resA = uLightDir.kA * uMaterial.kA;
	}

	normal = normalize(chNor);
	lightDirection = normalize(-uLightDir.direction);
	nD = max(dot(normal, lightDirection), 0.0);
	if (uTextureEnabled)
	{
		resD = uLightDir.kD * nD;
	}
	else
	{
		resD = uLightDir.kD * ( nD * uMaterial.kD);
	}
	
	viewDirection = normalize(uViewPos - chFragPos);
	reflectionDirection = reflect(-lightDirection, normal);
	s = pow(max(dot(viewDirection, reflectionDirection), 0.0), uMaterial.shine);
	if (uTextureEnabled)
	{
		resS = uLightDir.kS * s;
	}
	else
	{
		resS = uLightDir.kS * (s * uMaterial.kS);
	}

	//reflektorsko osvetljenje
	lightDirection = normalize(uLightRef.position - chFragPos);
	float theta = dot(lightDirection, normalize(-uLightRef.direction));

	if (theta > uLightRef.cutOff)
	{
		if (uTextureEnabled)
		{
			resA = uLightRef.kA;
		}
		else
		{
			resA = uLightRef.kA * uMaterial.kA;
		}

		normal = normalize(chNor);
		nD = max(dot(normal, lightDirection), 0.0);
		if (uTextureEnabled)
		{
			resD = uLightRef.kD * nD;
		}
		else
		{
			resD = uLightRef.kD * ( nD * uMaterial.kD);
		}

		viewDirection = normalize(uViewPos - chFragPos);
		reflectionDirection = reflect(-lightDirection, normal);
		s = pow(max(dot(viewDirection, reflectionDirection), 0.0), uMaterial.shine);
		if (uTextureEnabled)
		{
			resS = uLightRef.kS * s;
		}
		else
		{
			resS = uLightRef.kS * (s * uMaterial.kS);
		}

		totalResult += resA + resD + resS;
	}
	else
	{
		if (uTextureEnabled)
		{
			totalResult += uLightRef.kA;
		}
		else
		{
			totalResult += uLightRef.kA * uMaterial.kA;
		}
	}

	totalResult += resA + resD + resS;

	if (uTextureEnabled) 
	{
		outCol = texture(uTex, chTex) * vec4(totalResult, 1.0);
	}
	else 
	{
		outCol = vec4(totalResult, 0.5); //Fongov model sjencenja
	}
}