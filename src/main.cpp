#include <Geode/Geode.hpp>
#include <Geode/modify/CCShaderCache.hpp>

#include "shaders.hpp"

using namespace geode::prelude;

enum {
    kCCShaderType_PositionTextureColor,
    kCCShaderType_PositionTextureColorAlphaTest,
    kCCShaderType_PositionColor,
    kCCShaderType_PositionTexture,
    kCCShaderType_PositionTexture_uColor,
    kCCShaderType_PositionTextureA8Color,
    kCCShaderType_Position_uColor,
    kCCShaderType_PositionLengthTextureColor,
    kCCShaderType_ControlSwitch,
};

struct ShaderTypeInfo { int type; char const* name; };
static constexpr std::array<ShaderTypeInfo, 9> shaderTypes{{
    {kCCShaderType_PositionTextureColor, kCCShader_PositionTextureColor},
    {kCCShaderType_PositionTextureColorAlphaTest, kCCShader_PositionTextureColorAlphaTest},
    {kCCShaderType_PositionColor, kCCShader_PositionColor},
    {kCCShaderType_PositionTexture, kCCShader_PositionTexture},
    {kCCShaderType_PositionTexture_uColor, kCCShader_PositionTexture_uColor},
    {kCCShaderType_PositionTextureA8Color, kCCShader_PositionTextureA8Color},
    {kCCShaderType_Position_uColor, kCCShader_Position_uColor},
    {kCCShaderType_PositionLengthTextureColor, kCCShader_PositionLengthTexureColor},
    {kCCShaderType_ControlSwitch, kCCShader_ControlSwitch},
}};

class $modify(CCShaderCache) {
    static Result<GLuint> compileShader(GLenum type, char const* source) {
        GLuint shader = glCreateShader(type);
        GLchar const* sources[] = {
    #ifdef GEODE_IS_MOBILE
            "#extension GL_OES_standard_derivatives : enable\n"
            "precision highp float;\n"
            "precision highp int;",
    #endif
            "uniform mat4 CC_PMatrix;\n"
            "uniform mat4 CC_MVMatrix;\n"
            "uniform mat4 CC_MVPMatrix;\n"
            "uniform vec4 CC_Time;\n"
            "uniform vec4 CC_SinTime;\n"
            "uniform vec4 CC_CosTime;\n"
            "uniform vec4 CC_Random01;\n",
            source
        };

        glShaderSource(shader, std::size(sources), sources, nullptr);
        glCompileShader(shader);

        GLint compiled = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

            if (infoLen > 1) {
                std::string infoLog(infoLen, '\0');
                glGetShaderInfoLog(shader, infoLen, nullptr, infoLog.data());
                glDeleteShader(shader);
                switch (type) {
                    case GL_VERTEX_SHADER:
                        return Err("Vertex shader compilation error: {}", infoLog);
                    case GL_FRAGMENT_SHADER:
                        return Err("Fragment shader compilation error: {}", infoLog);
                    default:
                        return Err("Shader compilation error: {}", infoLog);
                }
            }

            glDeleteShader(shader);
            return Err("Shader compilation failed with unknown error");
        }

        return Ok(shader);
    }

    static bool loadProgram(CCGLProgram* p, char const* vertShader, char const* fragShader) {
        auto vert = compileShader(GL_VERTEX_SHADER, vertShader);
        if (!vert) {
            log::error("{}", vert.unwrapErr());
            return false;
        }

        auto frag = compileShader(GL_FRAGMENT_SHADER, fragShader);
        if (!frag) {
            log::error("{}", frag.unwrapErr());
            glDeleteShader(vert.unwrap());
            return false;
        }

        p->m_uFragShader = frag.unwrap();
        p->m_uVertShader = vert.unwrap();

        GLuint program = glCreateProgram();
        glAttachShader(program, vert.unwrap());
        glAttachShader(program, frag.unwrap());

        p->m_uProgram = program;
        p->m_pHashForUniforms = nullptr;

        return true;
    }

    static void loadShader(CCGLProgram* p, int type) {
        switch (type) {
            case kCCShaderType_PositionTextureColor:
                loadProgram(p, shader::positionTextureColor_vert, shader::positionTextureColor_frag);
                p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
                p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
                p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
                break;
            case kCCShaderType_PositionTextureColorAlphaTest:
                loadProgram(p, shader::positionTextureColor_vert, shader::positionTextureColorAlphaTest_frag);
                p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
                p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
                p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
                break;
            case kCCShaderType_PositionColor:
                loadProgram(p, shader::positionColor_vert, shader::positionColor_frag);
                p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
                p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
                break;
            case kCCShaderType_PositionTexture:
                loadProgram(p, shader::positionTexture_vert, shader::positionTexture_frag);
                p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
                p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
                break;
            case kCCShaderType_PositionTexture_uColor:
                loadProgram(p, shader::positionTexture_uColor_vert, shader::positionTexture_uColor_frag);
                p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
                p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
                break;
            case kCCShaderType_PositionTextureA8Color:
                loadProgram(p, shader::positionTextureA8Color_vert, shader::positionTextureA8Color_frag);
                p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
                p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
                p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
                break;
            case kCCShaderType_Position_uColor:
                loadProgram(p, shader::position_uColor_vert, shader::position_uColor_frag);
                p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
                break;
            case kCCShaderType_PositionLengthTextureColor:
                loadProgram(p, shader::positionColorLengthTexture_vert, shader::positionColorLengthTexture_frag);
                p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
                p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
                p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
                break;
            case kCCShaderType_ControlSwitch:
                loadProgram(p, shader::positionTextureColor_vert, shader::exSwitchMask_frag);
                p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
                p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
                p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
                break;

            default:
                log::error("Unknown shader type: {}", type);
                return;
        }

        p->link();
        p->updateUniforms();
    }

    void loadDefaultShaders() {
        for (auto [t, n] : shaderTypes) {
            auto* p = new CCGLProgram();
            loadShader(p, t);
            m_pPrograms->setObject(p, n);
            p->release();
        }
    }

#ifndef GEODE_IS_IOS
    void reloadDefaultShaders() {
        for (auto [t, n] : shaderTypes) {
            auto* p = this->programForKey(n);
            p->reset();
            loadShader(p, t);
        }
    }
#endif
};