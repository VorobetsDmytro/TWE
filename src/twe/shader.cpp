#include "shader.hpp"

Shader::Shader(const char* vertPath, const char* fragPath) {
    std::string vertBody = File::getBody(vertPath);
    std::string fragBody = File::getBody(fragPath);
    const char* vertex = vertBody.c_str();
    const char* fragment = fragBody.c_str();
    GLuint vertexShader = create(vertex, GL_VERTEX_SHADER);
    GLuint fragmentShader = create(fragment, GL_FRAGMENT_SHADER);
    _id = glCreateProgram();
    GLuint shaders[2] = {
        vertexShader,
        fragmentShader
    };
    linkShaders(shaders, 2);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    _material = std::make_shared<Material>();
    initMaterial();
}

Shader::Shader(const Shader& shader) {
    this->_id = shader._id;
    this->_material = shader._material;
}

Shader::~Shader() {
    clean();
}

GLuint Shader::create(const char* fileContent, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &fileContent, nullptr);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(!status) {
        GLchar errorInfo[512];
        glGetShaderInfoLog(shader, sizeof(errorInfo), NULL, errorInfo);
        std::cout << "Error compilation a shader.\n" << errorInfo << std::endl;
    };
    return shader;
}

void Shader::linkShaders(GLuint* shaders, GLuint size) {
    for(int i = 0; i < size; ++i)
        glAttachShader(_id, shaders[i]);
    glLinkProgram(_id);
    GLint status;
    glGetProgramiv(_id, GL_COMPILE_STATUS, &status);
    if(!status) {
        GLchar errorInfo[512];
        glGetProgramInfoLog(_id, sizeof(errorInfo), NULL, errorInfo);
        std::cout << "Error link a shader.\n" << errorInfo << std::endl;
    };
}

void Shader::use() {
    glUseProgram(_id);
}

void Shader::clean() {
    glDeleteProgram(_id);
}

void Shader::setUniform(const char* name, const glm::vec3& vec) {
    use();
    GLuint loc = glGetUniformLocation(getId(), name);
    glUniform3f(loc, vec.x, vec.y, vec.z);
}

void Shader::setUniform(const char* name, GLfloat value) {
    use();
    GLuint loc = glGetUniformLocation(getId(), name);
    glUniform1f(loc, value);
}

void Shader::setUniform(const char* name, GLuint value) {
    use();
    GLuint loc = glGetUniformLocation(getId(), name);
    glUniform1ui(loc, value);
}

void Shader::setUniform(const char* name, GLint value) {
    use();
    GLuint loc = glGetUniformLocation(getId(), name);
    glUniform1i(loc, value);
}

void Shader::setUniform(const char* name, GLboolean value) {
    use();
    GLuint loc = glGetUniformLocation(getId(), name);
    glUniform1i(loc, value);    
}

void Shader::initMaterial() {
    Material* material = _material.get();
    setUniform("material.objColor", material->objColor);
    setUniform("material.ambient", material->ambient);
    setUniform("material.diffuse", material->diffuse);
    setUniform("material.specular", material->specular);
    setUniform("material.shininess", material->shininess);
}

void Shader::setMaterial(const Material& material) {
    *_material.get() = material;
    initMaterial();
}

GLuint Shader::getId() const noexcept { return _id; }

Material& Shader::getMaterial() const noexcept { return *_material.get(); }

const char* TRANS_MAT_OPTIONS[3] = {
    "model",
    "view",
    "projection"
};

const char* SHADER_PATHS[5] = {
    "../../src/shaders/default.vert",
    "../../src/shaders/default.frag",
    "../../src/shaders/texture.frag",
    "../../src/shaders/light.vert",
    "../../src/shaders/light.frag"
};