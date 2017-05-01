#version 120

uniform sampler2D u_texid;
uniform mat3 normal_matrix;

varying vec2 v_texcoord;
varying vec3 v_normal;
varying vec3 v_vertex;

varying mat4 modelview_matrix;

vec4 light_position = vec4(-5.0, 10.0, 3.8, 1.0);
vec4 light_ambient = vec4(0.3, 0.1, 0.1, 1.0);
vec4 light_diffuse = vec4(0.5, 0.5, 0.5, 1.0);
vec4 light_specular = vec4(1.0, 1.0, 1.0, 1.0);
vec4 material_ambient = vec4(0.1, 0.1, 0.1,1.0);
vec4 material_diffuse = vec4(0.3, 0.3, 0.2, 1.0);
vec4 material_specular = vec4(0.1, 0.1, 0.1, 1.0);
float material_shininess = 10.0;

vec4 phong_reflection(vec3 normal_v){
	
	vec4 color = vec4(0,0,0,1);
	vec3 lightpos = (modelview_matrix * light_position).xyz;
	vec3 normal = normalize(normal_matrix * normal_v);
	vec3 light_vector = normalize(lightpos - v_normal );
	vec3 reflect_vector = reflect(-light_vector, normal);
	vec3 view_vector = normalize(-v_vertex);

	float kd = max(dot(light_vector, normal), 0.0);
	float ks = pow(max(0.0, dot(reflect_vector, view_vector) ), material_shininess);

	vec4 ambient = material_ambient * light_ambient;
	vec4 diffuse = kd*material_diffuse;
	vec4 specular = ks * material_specular;

	color = ambient + diffuse + specular;

	return color;
}

void main() {
  vec3 normal = texture2D(u_texid, v_texcoord).xyz;
  //gl_FragColor = phong_reflection(normal);
  gl_FragColor = texture2D(u_texid, v_texcoord) + phong_reflection(normal);
  //gl_FragColor = texture2D(u_texid, v_texcoord);
}