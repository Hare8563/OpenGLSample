//
//  shader_utils.h
//  OpenGLSample
//
//  Created by 晴佐久哲士 on 2016/12/31.
//  Copyright (c) 2016年 晴佐久哲士. All rights reserved.
//

#ifndef OpenGLSample_shader_utils_h
#define OpenGLSample_shader_utils_h
#include <GLUT/glut.h>
#include <iostream>

const char* file_read(const char* filename);
void print_log(GLuint object);
GLuint create_shader(const char* filename, GLenum type);

#endif
