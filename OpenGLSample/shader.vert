attribute vec2 coord2d; // 頂点ごとに変化する情報(位置)
attribute vec3 v_color; // 頂点ごとに変化する情報(色)

varying lowp vec3 f_color; // 頂点 -> フラグメントに渡すための変数

void main(void)
{
    gl_Position = vec4(coord2d, 0.0, 1.0);
    f_color = v_color;
}