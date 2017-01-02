//
//  main.c
//  OpenGLSample
//
//  Created by 晴佐久哲士 on 2016/12/03.
//  Copyright (c) 2016年 晴佐久哲士. All rights reserved.
//

#include "shader_utils.h"
#include <math.h>

struct Attributes{
    GLfloat coord2d[2];
    GLfloat v_color[3];
};

Attributes triangle_attributes[] = {
    {{0.0, 0.8}, {1.0, 1.0, 0.0}},
    {{-0.8, -0.8}, {0.0, 0.0, 1.0}},
    {{0.8, -0.8}, {1.0, 0.0, 0.0}}
};

// GLSLプログラム
GLuint program;

//VBO
GLuint vbo_triangle;
GLuint vbo_triangle_colors;

// シェーダに渡す、頂点情報
GLint attribute_coord2d;
GLint attribute_v_color;

//グローバル透明度
GLint uniform_fade;

void idle(void){
    float cur_fade = sinf(glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (2*M_PI) / 5) / 2 + 0.5; // 0->1->0 every 5 seconds
    glUseProgram(program);
    glUniform1f(uniform_fade, cur_fade);
    glutPostRedisplay();
}


void draw(void) {
    /* Clear the background as white */
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(program);

    glEnableVertexAttribArray(attribute_coord2d);
    glEnableVertexAttribArray(attribute_v_color);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);

    /* Describe our vertices array to OpenGL (it can't guess its format automatically) */
    glVertexAttribPointer(
                          attribute_coord2d, // attribute
                          2,                 // number of elements per vertex, here (x,y)
                          GL_FLOAT,          // the type of each element
                          GL_FALSE,          // take our values as-is
                          sizeof(Attributes),                 // no extra data between each position
                          (void*)0  // pointer to the C array
    );
    
    glVertexAttribPointer(
                          attribute_v_color,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Attributes),
                          (GLvoid*)offsetof(Attributes, v_color)
    );

//    glEnableVertexAttribArray(attribute_v_color);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle_colors);
//    glVertexAttribPointer(attribute_v_color,
//                          3,
//                          GL_FLOAT,
//                          GL_FALSE,
//                          0,
//                          (void*)0
//    );
    
    // 透明度の設定
    
    /* Push each element in buffer_vertices to the vertex shader */
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    // 属性について完了したことをOpenGLに通知
    glDisableVertexAttribArray(attribute_coord2d);
    glDisableVertexAttribArray(attribute_v_color);

    /* Display the result */
    glutSwapBuffers();
}

int init_resources(void)
{
    GLuint vs = create_shader("shader.vert", GL_VERTEX_SHADER);
    GLuint fs = create_shader("shader.frag", GL_FRAGMENT_SHADER);
    GLint link_ok = GL_FALSE;
    
    if(vs == 0) return 0;
    if(fs == 0) return 0;
    
    //GLSLプログラムにシェーダをリンク
    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    
    //リンクのステータスチェック
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (link_ok == GL_FALSE) {
        fprintf(stderr, "glLinkProgram:");
        print_log(program);
        return 0;
    }

    
    //VBOの作成
    glGenBuffers(1, &vbo_triangle);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_attributes), triangle_attributes, GL_STATIC_DRAW);

    attribute_coord2d = glGetAttribLocation(program, "coord2d");
    if (attribute_coord2d == -1)
    {
        fprintf(stderr, "Could not bind attribute coord2d\n");
        return 0;
    }
    
    attribute_v_color = glGetAttribLocation(program, "v_color");
    if (attribute_v_color == -1)
    {
        fprintf(stderr, "Could not bind attribute v_color\n");
        return 0;
    }

//
//    GLfloat triangle_vertices[] = {
//        0.0,  0.8,
//        -0.8, -0.8,
//        0.8, -0.8,
//    };
//
//    // バッファ領域の作成
//    glGenBuffers(1, &vbo_triangle);
//    // ArrayBufferとしてVBOを指定
//    glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
//    // ArrayBufferにデータをセット
//    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);
//    
//    // ArrayBufferを非アクティブに
//    //glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//    //頂点シェーダの位置情報を取得
//    const char* attribute_name = "coord2d";
//    attribute_coord2d = glGetAttribLocation(program, attribute_name);
//    if (attribute_coord2d == -1)
//    {
//        fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
//        return 0;
//    }
//    
//    //色情報の作成
//    GLfloat triangle_colors[] = {
//        1.0, 1.0, 0.0,
//        0.0, 0.0, 1.0,
//        1.0, 0.0, 0.0
//    };
//    
//    glGenBuffers(1, &vbo_triangle_colors);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle_colors);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_colors), triangle_colors, GL_STATIC_DRAW);
//    
//    //頂点シェーダの色情報を取得
//    attribute_name = "v_color";
//    attribute_v_color = glGetAttribLocation(program, attribute_name);
//    if (attribute_v_color == -1)
//    {
//        fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
//        return 0;
//    }
    

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    uniform_fade = glGetUniformLocation(program, "fade");
    if(uniform_fade == -1){
        fprintf(stderr, "Could not bind uniform fade");
        return 0;
    }
    
    return 1;
}

void free_resources()
{
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo_triangle);
}

int main(int argc, char * argv[]) {
    // insert code here...
    glutInit(&argc, argv);
    glutInitWindowSize(600,600);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_ALPHA | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("Modern OpenGL Sample");

    if (init_resources() == 1)
    {
        glutDisplayFunc(draw);
        glutIdleFunc(idle);
        glutMainLoop();
    }

    free_resources();
    return 0;
}