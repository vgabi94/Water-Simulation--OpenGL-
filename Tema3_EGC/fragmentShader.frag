#version 330 core
in vec3 FragPos;
in vec3 Normal;

out vec4 color;

uniform vec3 gridColor;
uniform vec3 lightColor; 
uniform vec3 lightPos;
uniform vec3 eyePos;
uniform samplerCube skybox;
uniform bool blinn;

void main()
{
	float ambientStrength = 0.7;
	vec3 ambient = ambientStrength * lightColor;
	
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.1);
	vec3 diffuse = diff * lightColor;
	
	float specularStrength = 1.0;
	vec3 viewDir = normalize(eyePos - FragPos);
	float spec = 0.0;
	if (blinn)
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);
		pow(max(dot(norm, halfwayDir), 0.1), 128);
	}
	else
	{
		vec3 reflectDir = reflect(-lightDir, norm);
		spec = pow(max(dot(viewDir, reflectDir), 0.1), 32);
	}
	vec3 specular = specularStrength * spec * lightColor;
	
	float ratio = 1.0 / 1.33;
	float fresnel = dot(viewDir, norm);
	vec3 refSkyDir = reflect(-viewDir, norm);
	vec3 fracSkyDir = refract(-viewDir, norm, ratio);
	vec4 refracColor = fresnel * texture(skybox, fracSkyDir) * vec4(gridColor, 1.0);
	vec4 reflecColor = (1.0 - fresnel) * texture(skybox, refSkyDir);
	
	vec3 result = (ambient + diffuse + specular) * gridColor;
	//color = vec4(result, 1);
	color = vec4(ambient + diffuse + specular, 1.0) * (reflecColor + refracColor);
}