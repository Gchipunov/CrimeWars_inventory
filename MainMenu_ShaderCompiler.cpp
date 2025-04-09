// ShaderCompiler.h
#ifndef SHADER_COMPILER_H
#define SHADER_COMPILER_H

#include <string>
#include "tier1/strtools.h"
#include "filesystem.h"
#include "tier0/dbg.h"

class ShaderCompiler {
public:
    ShaderCompiler();
    ~ShaderCompiler();

    bool compileShaderFromFile(const std::string& filePath, unsigned int& shaderID);
    bool compileShaderFromSource(const std::string& shaderSource, unsigned int& shaderID, GLenum shaderType);
    
    std::string getLastError() const { return lastError; }
    
private:
    bool readShaderFile(const std::string& filePath, std::string& outSource);
    bool checkCompileStatus(unsigned int shaderID);
    
    std::string lastError;
};

#endif // SHADER_COMPILER_H

// ShaderCompiler.cpp
#include "ShaderCompiler.h"
#include <GL/gl.h>
#include <GL/glext.h>

ShaderCompiler::ShaderCompiler() : lastError("") {}

ShaderCompiler::~ShaderCompiler() {}

bool ShaderCompiler::compileShaderFromFile(const std::string& filePath, unsigned int& shaderID) {
    std::string shaderSource;
    if (!readShaderFile(filePath, shaderSource)) {
        return false;
    }

    // Determine shader type from extension
    GLenum shaderType = GL_VERTEX_SHADER;
    if (V_stristr(filePath.c_str(), ".frag") || V_stristr(filePath.c_str(), ".fs")) {
        shaderType = GL_FRAGMENT_SHADER;
    }

    return compileShaderFromSource(shaderSource, shaderID, shaderType);
}

bool ShaderCompiler::compileShaderFromSource(const std::string& shaderSource, unsigned int& shaderID, GLenum shaderType) {
    // Create shader object
    shaderID = glCreateShader(shaderType);
    if (shaderID == 0) {
        lastError = "Failed to create shader object";
        return false;
    }

    // Set shader source
    const char* sourcePtr = shaderSource.c_str();
    glShaderSource(shaderID, 1, &sourcePtr, nullptr);
    
    // Compile shader
    glCompileShader(shaderID);
    
    // Check compilation status
    if (!checkCompileStatus(shaderID)) {
        glDeleteShader(shaderID);
        shaderID = 0;
        return false;
    }
    
    return true;
}

bool ShaderCompiler::readShaderFile(const std::string& filePath, std::string& outSource) {
    FileHandle_t file = filesystem->Open(filePath.c_str(), "r", "GAME");
    if (!file) {
        lastError = "Failed to open shader file: " + filePath;
        return false;
    }

    int fileSize = filesystem->Size(file);
    if (fileSize <= 0) {
        filesystem->Close(file);
        lastError = "Empty or invalid shader file: " + filePath;
        return false;
    }

    char* buffer = new char[fileSize + 1];
    filesystem->Read(buffer, fileSize, file);
    buffer[fileSize] = '\0';
    
    outSource = buffer;
    delete[] buffer;
    filesystem->Close(file);
    
    return true;
}

bool ShaderCompiler::checkCompileStatus(unsigned int shaderID) {
    GLint success = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        GLint logLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
        
        if (logLength > 0) {
            char* errorLog = new char[logLength];
            glGetShaderInfoLog(shaderID, logLength, nullptr, errorLog);
            lastError = std::string(errorLog);
            delete[] errorLog;
        } else {
            lastError = "Unknown shader compilation error";
        }
        return false;
    }
    
    return true;
}

// Example integration into main menu (in your main menu class)
void CMainMenu::AddShaderCompilerPanel() {
    ShaderCompiler* compiler = new ShaderCompiler();
    
    // Example usage
    unsigned int vertexShaderID = 0;
    if (compiler->compileShaderFromFile("shaders/test.vert", vertexShaderID)) {
        Msg("Successfully compiled vertex shader\n");
    } else {
        Warning("Vertex shader compilation failed: %s\n", compiler->getLastError().c_str());
    }
    
    unsigned int fragmentShaderID = 0;
    if (compiler->compileShaderFromFile("shaders/test.frag", fragmentShaderID)) {
        Msg("Successfully compiled fragment shader\n");
    } else {
        Warning("Fragment shader compilation failed: %s\n", compiler->getLastError().c_str());
    }
    
    // Create program and link shaders if compilation succeeded
    if (vertexShaderID && fragmentShaderID) {
        unsigned int programID = glCreateProgram();
        glAttachShader(programID, vertexShaderID);
        glAttachShader(programID, fragmentShaderID);
        glLinkProgram(programID);
        
        // Clean up individual shaders
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
    }
    
    delete compiler;
}
