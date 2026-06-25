#pragma once

namespace shader {
    constexpr char const* positionTextureColor_vert =
R"(attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

#ifdef GL_ES
varying highp vec4 v_fragmentColor;
varying highp vec2 v_texCoord;
#else
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
#endif

void main() {
    gl_Position = CC_MVPMatrix * a_position;
    v_fragmentColor = a_color;
    v_texCoord = a_texCoord;
})";

    constexpr char const* positionTextureColor_frag =
R"(varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform sampler2D CC_Texture0;

void main() {
    gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
})";

    constexpr char const* positionTextureColorAlphaTest_frag =
R"(varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform sampler2D CC_Texture0;
uniform float CC_alpha_value;

void main() {
    vec4 texColor = texture2D(CC_Texture0, v_texCoord);
    if (texColor.a <= CC_alpha_value) discard;
    gl_FragColor = texColor * v_fragmentColor;
})";

    constexpr char const* positionColor_vert =
R"(attribute vec4 a_position;
attribute vec4 a_color;
#ifdef GL_ES
varying highp vec4 v_fragmentColor;
#else
varying vec4 v_fragmentColor;
#endif

void main() {
    gl_Position = CC_MVPMatrix * a_position;
    v_fragmentColor = a_color;
})";

    constexpr char const* positionColor_frag =
R"(varying vec4 v_fragmentColor;

void main() {
    gl_FragColor = v_fragmentColor;
})";

    constexpr char const* positionTexture_vert =
R"(attribute vec4 a_position;
attribute vec2 a_texCoord;
#ifdef GL_ES
varying highp vec2 v_texCoord;
#else
varying vec2 v_texCoord;
#endif

void main() {
    gl_Position = CC_MVPMatrix * a_position;
    v_texCoord = a_texCoord;
})";

    constexpr char const* positionTexture_frag =
R"(varying vec2 v_texCoord;
uniform sampler2D CC_Texture0;

void main() {
    gl_FragColor = texture2D(CC_Texture0, v_texCoord);
})";

    constexpr char const* positionTexture_uColor_vert =
R"(attribute vec4 a_position;
attribute vec2 a_texCoord;
#ifdef GL_ES
varying highp vec2 v_texCoord;
#else
varying vec2 v_texCoord;
#endif

void main() {
    gl_Position = CC_MVPMatrix * a_position;
    v_texCoord = a_texCoord;
})";

    constexpr char const* positionTexture_uColor_frag =
R"(uniform vec4 u_color;
varying vec2 v_texCoord;
uniform sampler2D CC_Texture0;

void main() {
    gl_FragColor = texture2D(CC_Texture0, v_texCoord) * u_color;
})";

    constexpr char const* positionTextureA8Color_vert =
R"(attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;
#ifdef GL_ES
varying highp vec4 v_fragmentColor;
varying highp vec2 v_texCoord;
#else
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
#endif

void main() {
    gl_Position = CC_MVPMatrix * a_position;
    v_fragmentColor = a_color;
    v_texCoord = a_texCoord;
})";

    constexpr char const* positionTextureA8Color_frag =
R"(varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform sampler2D CC_Texture0;

void main() {
    gl_FragColor = vec4(v_fragmentColor.rgb, v_fragmentColor.a * texture2D(CC_Texture0, v_texCoord).a);
})";

    constexpr char const* position_uColor_vert =
R"(attribute vec4 a_position;
uniform	vec4 u_color;
uniform float u_pointSize;
#ifdef GL_ES
varying highp vec4 v_fragmentColor;
#else
varying vec4 v_fragmentColor;
#endif

void main() {
    gl_Position = CC_MVPMatrix * a_position;
    gl_PointSize = u_pointSize;
    v_fragmentColor = u_color;
})";

    constexpr char const* position_uColor_frag =
R"(varying vec4 v_fragmentColor;

void main() {
    gl_FragColor = v_fragmentColor;
})";

    constexpr char const* positionColorLengthTexture_vert =
R"(#ifdef GL_ES
attribute highp vec4 a_position;
attribute highp vec2 a_texcoord;
attribute highp vec4 a_color;
varying highp vec4 v_color;
varying highp vec2 v_texcoord;
#else
attribute vec4 a_position;
attribute vec2 a_texcoord;
attribute vec4 a_color;
varying vec4 v_color;
varying vec2 v_texcoord;
#endif

void main() {
    v_color = vec4(a_color.rgb * a_color.a, a_color.a);
    v_texcoord = a_texcoord;
    gl_Position = CC_MVPMatrix * a_position;
})";

    constexpr char const* positionColorLengthTexture_frag =
R"(#ifdef GL_ES
varying highp vec4 v_color;
varying highp vec2 v_texcoord;
#else
varying vec4 v_color;
varying vec2 v_texcoord;
#endif

void main() {
    gl_FragColor = v_color*smoothstep(0.0, length(fwidth(v_texcoord)), 1.0 - length(v_texcoord));
})";

    constexpr char const* exSwitchMask_frag =
R"(varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform sampler2D u_texture;
uniform sampler2D u_mask;

void main() {
    vec4 texColor = texture2D(u_texture, v_texCoord);
    vec4 maskColor = texture2D(u_mask, v_texCoord);
    vec4 finalColor = vec4(texColor.r, texColor.g, texColor.b, maskColor.a * texColor.a);
    gl_FragColor = v_fragmentColor * finalColor;
})";
}