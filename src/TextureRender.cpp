#include "TextureRender.h"
#include <stdexcept>
#include <string>

static const GLchar *const fragment_shader_source = R"(
  #version 130
  uniform sampler2D usamp;
  in vec2 vtexcoord;
  out vec4 frag_color;
  void main(void) {
    frag_color = texture(usamp, vtexcoord);
  }
)";

static const GLchar *const vertex_shader_source = R"(
  #version 130
  in vec4 position;
  in vec2 texcoord;
  uniform mat4 transform;
  out vec2 vtexcoord;
  void main() {
    gl_Position = transform * position;
    vtexcoord = texcoord;
  })";

static GLuint compile_shader(const GLchar *const shader_source,
                             GLenum shader_type) {
  GLuint shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, &shader_source, nullptr);
  glCompileShader(shader);

  GLint success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    GLint log_length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

    std::string error_log;
    error_log.reserve(log_length);
    glGetShaderInfoLog(shader, log_length, nullptr, &error_log[0]);

    glDeleteShader(shader);

    throw std::runtime_error(error_log.data());
  }
  return shader;
}

cs200::TextureRender::TextureRender()
    : program{0},         //
      texture_buffer{0},  //
      vertex_buffer{0},   //
      texcoord_buffer{0}, //
      face_buffer{0},     //
      vao{0},             //
      utransform{0},      //
      mesh_face_count{0} {

  const GLuint fragment_shader_id =
      compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);
  const GLuint vertex_shader_id =
      compile_shader(vertex_shader_source, GL_VERTEX_SHADER);

  program = glCreateProgram();

  glAttachShader(program, fragment_shader_id);
  glAttachShader(program, vertex_shader_id);

  glLinkProgram(program);
  GLint link_ok = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (link_ok == GL_FALSE) {
    GLint log_length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

    std::string error_log;
    error_log.reserve(log_length);
    glGetProgramInfoLog(program, log_length, &log_length, &error_log[0]);

    glDeleteProgram(program);
    glDeleteShader(fragment_shader_id);
    glDeleteShader(vertex_shader_id);

    throw std::runtime_error(error_log.data());
  }

  glDeleteShader(fragment_shader_id);
  glDeleteShader(vertex_shader_id);

  utransform = glGetUniformLocation(program, "transform");
}

cs200::TextureRender::~TextureRender() {}

void cs200::TextureRender::clearFrame(const glm::vec4 &c) {
  glClearColor(c.r, c.g, c.b, c.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void cs200::TextureRender::loadTexture(unsigned char *rgbdata, int width,
                                       int height) {}

void cs200::TextureRender::setTransform(const glm::mat4 &transform_matrix) {}

void cs200::TextureRender::loadMesh(const TexturedMesh &m) {}

void cs200::TextureRender::unloadTexture() {}

void cs200::TextureRender::unloadMesh() {}

void cs200::TextureRender::displayFaces() {}
