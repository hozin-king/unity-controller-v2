#include <iostream>
#include <string>
#include <vector>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace CHAMS_SCANNER {
    void *handle = NULL;
    std::vector<std::string> chams;
    const char *chamsName = "null";
    bool isStarted = false;
    
    struct ChamsConfig {
        const char* shaderName = "_NEPMODS";
        bool enableWallhack;
        bool enableWallhackS;
        bool enableWallhackW;
        bool enableWallhackG;
        bool enableWallhackO;
        bool enableRainbow;
        bool enableRainbow1;
        bool visibSh;
        bool BackFlat;
        bool invis;
        float r = 255.0f;
        float g = 0.0f;
        float b = 0.0f;
        int w = 1;
        int a = 255;

        float red = 255.0f;
        float green = 0.0f;
        float blue =0.0f;
        float mi = 0.0f;
    } config;
    
    GLint (*Real_Dump)(GLuint program, const char *name);
    GLint SaveDump(GLuint program, const char *name) {
        std::vector<std::string>::iterator checkName = find(chams.begin(), chams.end(), name);
        if (checkName == chams.end()) {
            chams.push_back(name);
        }
        return Real_Dump(program, name);
    }
    
    bool lib() {
        handle = 0;
        handle = dlopen("libGLESv2.so", RTLD_LAZY);
        if(!handle) {
            return false;
        }
        return true;
    }
    bool isCurrentShader(const char *shader) {
        GLint currProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
        return Real_Dump(currProgram, shader) != -1;
    }
    #define _DRAW_RGB_ 255,0,0
    void (*old_glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void *indices);
    void new_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices) {
        old_glDrawElements(mode, count, type, indices);
        if (mode != GL_TRIANGLES || count < 10000) return; {
            if (isCurrentShader(config.shaderName)) {
                
                if (config.enableWallhackW) {
                    glDepthRangef(1, 0.5);
                    glBlendColor(GLfloat(config.r/255), GLfloat(config.g/255), GLfloat(config.b/255), 1);
                    glColorMask(1, 1, 1, 1);
                    glEnable(GL_BLEND);
                    glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE, GL_ZERO);
                    glLineWidth(config.w);
                    old_glDrawElements(GL_LINE_LOOP, count, type, indices);
                }
            
                if(config.visibSh) {
                    glDisable(GL_DEPTH_TEST);
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_ONE, GL_CONSTANT_COLOR);
                    glBlendColor(0.0f, 1.0f, 0.0f, 1.0f);
                    old_glDrawElements(GL_TRIANGLES, count, type, indices);
                    glEnable(GL_DEPTH_TEST);
                    glBlendColor(GLfloat(config.r/255), GLfloat(config.g/255), GLfloat(config.b/255), GLfloat(config.a/255));
                    old_glDrawElements(GL_TRIANGLES, count, type, indices); 
                }
                
                if(config.BackFlat) {
                    glDisable(GL_DEPTH_TEST);
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_ZERO, GL_ZERO);
                    old_glDrawElements(GL_TRIANGLES, count, type, indices);
                    glEnable(GL_DEPTH_TEST);
                }
            
                if(config.invis) {
                    glDisable(GL_DEPTH_TEST);
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_ZERO, GL_ONE);
                    old_glDrawElements(GL_TRIANGLES, count, type, indices);
                    glEnable(GL_DEPTH_TEST);
                    glDepthFunc(GL_LESS);
                }
                
                if (config.enableWallhack) {
                    glBlendColor(GLfloat(config.r/255), GLfloat(config.g/255), GLfloat(config.b/255), GLfloat(config.a/255));
                    glColorMask(config.r, config.g, config.b, 255);
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_CONSTANT_ALPHA, GL_CONSTANT_COLOR);
                }
            
                if (config.enableWallhackS) {
                   glDepthRangef(1, 0.5);
                   glEnable(GL_BLEND);
                   glBlendFunc(GL_SRC_COLOR, GL_CONSTANT_COLOR);
                   glBlendEquation(GL_FUNC_ADD);
                   glBlendColor(GLfloat(config.r/255), GLfloat(config.g/255), GLfloat(config.b/255), 1);
                   glDepthFunc(GL_ALWAYS);
                   old_glDrawElements(GL_TRIANGLES, count, type, indices);
                   glColorMask(config.r, config.g, config.b, 255);
                   glBlendFunc(GL_DST_COLOR, GL_ONE);
                   glDepthFunc(GL_LESS);
                   glBlendColor(0.0, 0.0, 0.0, 0.0);
                }
            
                if (config.enableWallhackG) {
                    glEnable(GL_BLEND);
                    glBlendColor(GLfloat(config.r/255), GLfloat(config.g/255), GLfloat(config.b/255), 1);
                    glColorMask(1, 1, 1, 1);
                    glEnable(GL_BLEND);
                    glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE, GL_ZERO);
                    glLineWidth(config.w);
                    glDepthRangef(0.5, 1);
                    old_glDrawElements(GL_LINES, count, type, indices);
                    glBlendColor(1, 1, 1, 1);
                    glDepthRangef(1, 0.5);
                    old_glDrawElements(GL_TRIANGLES, count, type, indices);
                }
                
                if (config.enableWallhackO) {
                    glDepthRangef(1, 0.5);
                    glLineWidth(config.w);
                    glEnable(GL_BLEND);
                    glColorMask(1, 1, 1, 1);
                    glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE, GL_ZERO);
                    glBlendColor(0, 0, 0, 1);
                    old_glDrawElements(GL_TRIANGLES, count, type, indices);
                    glBlendColor(GLfloat(config.r/255), GLfloat(config.g/255), GLfloat(config.b/255), 1);
                    old_glDrawElements(GL_LINES, count, type, indices);
                }
            
                if (config.enableRainbow) {
                    if(true){
                       if (config.red == 255){
                           if (config.blue == 0 ){
                               if (config.green == 255){} else{
                                   config.green = config.green+1;
                               }
                           }
                       }
                       if (config.green == 255){
                           if (config.red == 0){} else{
                               config.red = config.red-1;
                           }
                       }
                       if (config.green == 255) {
                           if (config.red == 0) {
                               if (config.blue==255){} else{
                                   config.blue = config.blue+1;
                               }
                           }
                       }
                       if (config.blue == 255) {
                           if (config.green == 0) {
                               config.mi = 1;
                               config.red = config.red+1;
                           } 
                           else{
                                config.green = config.green-1;
                           }
                       }
                       if (config.mi == 1){
                           if (config.red == 255){
                               if (config.blue == 0){} else{
                                   config.blue = config.blue-1;
                               }
                           }
                       }
                       config.r = config.red;
                       config.g = config.green;
                       config.b = config.blue;
                    }
                    glBlendColor(GLfloat(config.r/255), GLfloat(config.g/255), GLfloat(config.b/255), GLfloat(config.a/255));
                    glColorMask(1, 1, 1, 1);
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_ONE_MINUS_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_ALPHA);
                    
                }
                old_glDrawElements(mode, count, type, indices);
        
                glDepthRangef(0.5, 1);
        
                glColorMask(1, 1, 1, 1);
                glDisable(GL_BLEND);
            }     
        }
    }
    void Dump() {
        void *p_glGetUniformLocation = (void *)dlsym(handle, "glGetUniformLocation");
        const char *dlsym_error = dlerror();
        if(dlsym_error) {
            return;
        } else {
            DobbyHook((void *)p_glGetUniformLocation,  (void *)SaveDump, (void **)&Real_Dump);
        }
    
    }
    void Draw() {
        auto p_glDrawElements = (const void*(*)(...))dlsym(handle, "glDrawElements");
        const char *dlsym_error = dlerror();
        if(dlsym_error){
            LOGE("Cannot load symbol 'glDrawElements': %s", dlsym_error);
            return;
        } else {
            hook((void *)p_glDrawElements, (void *)new_glDrawElements, (void **)&old_glDrawElements);
        }
    }
    
    void Start() {
        if (lib()) {
            if(!isStarted) {
                Dump();
                Draw();
                isStarted = true;
            }
        }
    }
    
}
