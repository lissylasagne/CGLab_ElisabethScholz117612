#version 150

in vec3 pass_Normal;
in vec3 pass_Color;

in vec3 pass_vert_position;
in vec3 pass_cam_direction;

out vec4 out_Color;

void main() {
  vec3 light_position = vec3(0,0,0);
  vec3 light_color = vec3(1.0,1.0,1.0);
  float light_power = 30.0;
  vec3 ambient_color = vec3(0.1,0.1,0.1);
  vec3 diffuse_color = vec3(0.2,0.2,0.2);
  vec3 specular_color = vec3(1.0,1.0,1.0); //spekulatius
  float shininess = 50.0;
  float screen_gamma= 2.2;

  //get rays and geometry:
  //vec3 normal = normalize(pass_cam_direction);
  vec3 normal = normalize(pass_Normal+pass_cam_direction);
  vec3 light_direction = normalize(light_position - pass_vert_position);
  float distance = length(light_direction);
  distance = distance * distance;

  float lambertian = max(dot(light_direction, normal), 0.0);
  float specular = 0.0;

  if(lambertian > 0.0) {
    vec3 view_direction = normalize(-pass_vert_position);

    vec3 half_direction = normalize(light_direction + view_direction);
    float specular_angle = max(dot(half_direction, normal), 0.0);
    specular = pow(specular_angle, shininess);
  }

  vec3 color_linear = ambient_color +
                     diffuse_color * lambertian * light_color * light_power / distance +
                     specular_color * specular * light_color * light_power / distance;


  color_linear *= pass_Color;                     
  out_Color = vec4(pow(color_linear, vec3(1.0/screen_gamma)), 1.0);
}