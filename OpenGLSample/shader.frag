varying lowp vec3 f_color;
uniform lowp float fade;  //どの頂点でも共通の値(透明度)
void main(void)
{
    gl_FragColor[0] = f_color.x;
    gl_FragColor[1] = f_color.y;
    gl_FragColor[2] = f_color.z;
    gl_FragColor[3] = fade;
}