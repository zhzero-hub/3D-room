/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2020-09-15 13:28:32
 * @LastEditors: sueRimn
 * @LastEditTime: 2020-09-15 13:46:30
 */
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

#define min(x,y) ((x) < (y) ? (x) : (y))
#define max(x,y) ((x) < (y) ? (y) : (x))
 
#define HeroHeight 0.1f           //玩家视点到脚的高度
 
#define GravityAcceler -4.0f
 
#define BoundaryGap 0.001f          //碰撞间距
#define JumpInitialSpeed 3.0f    //起跳初速度
#define JumpFactor 0.02f          //跳起速度系数
#define GravityFactor 0.02f       //下落速度系数

const GLfloat PI= 3.14159265358979323846f;
const float rightAngle = 90.0f;

int gamemode = 0;
//0: 使用camera进行浏览
//1: 使用ball进行浏览

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

float floorVertices[] = {
    // ---- 位置 ----    ---- 法向 ----     --- 纹理 ---
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 15.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 15.0f, 15.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 15.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 15.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 15.0f
};

float wallVertices[3][48] = {
{
    -0.3f, -0.3f, -0.3f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.3f, -0.3f, -0.3f,  0.0f,  0.0f, -1.0f,  15.0f, 0.0f,
     0.3f,  0.3f, -0.3f,  0.0f,  0.0f, -1.0f,  15.0f, 15.0f,
     0.3f,  0.3f, -0.3f,  0.0f,  0.0f, -1.0f,  15.0f, 15.0f,
    -0.3f,  0.3f, -0.3f,  0.0f,  0.0f, -1.0f,  0.0f, 15.0f,
    -0.3f, -0.3f, -0.3f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f
},
{
    -0.3f, -0.3f,  0.3f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.3f, -0.3f,  0.3f,  0.0f,  0.0f, 1.0f,   15.0f, 0.0f,
     0.3f,  0.3f,  0.3f,  0.0f,  0.0f, 1.0f,   15.0f, 15.0f,
     0.3f,  0.3f,  0.3f,  0.0f,  0.0f, 1.0f,   15.0f, 15.0f,
    -0.3f,  0.3f,  0.3f,  0.0f,  0.0f, 1.0f,   0.0f, 15.0f,
    -0.3f, -0.3f,  0.3f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f
},
{
    -0.3f,  0.3f,  0.3f, -1.0f,  0.0f,  0.0f,  15.0f, 0.0f,
    -0.3f,  0.3f, -0.3f, -1.0f,  0.0f,  0.0f,  15.0f, 15.0f,
    -0.3f, -0.3f, -0.3f, -1.0f,  0.0f,  0.0f,  0.0f, 15.0f,
    -0.3f, -0.3f, -0.3f, -1.0f,  0.0f,  0.0f,  0.0f, 15.0f,
    -0.3f, -0.3f,  0.3f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.3f,  0.3f,  0.3f, -1.0f,  0.0f,  0.0f,  15.0f, 0.0f
}
};

float cubeVertices[] = {
    // positions          // normals           // texture coords
    -0.1f, -0.7f, -0.1f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.1f, -0.7f, -0.1f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.1f,  0.7f, -0.1f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.1f,  0.7f, -0.1f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.1f,  0.7f, -0.1f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.1f, -0.7f, -0.1f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.1f, -0.7f,  0.1f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.1f, -0.7f,  0.1f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.1f,  0.7f,  0.1f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.1f,  0.7f,  0.1f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.1f,  0.7f,  0.1f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.1f, -0.7f,  0.1f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.1f,  0.7f,  0.1f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.1f,  0.7f, -0.1f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.1f, -0.7f, -0.1f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.1f, -0.7f, -0.1f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.1f, -0.7f,  0.1f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.1f,  0.7f,  0.1f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.1f,  0.7f,  0.1f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.1f,  0.7f, -0.1f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.1f, -0.7f, -0.1f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.1f, -0.7f, -0.1f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.1f, -0.7f,  0.1f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.1f,  0.7f,  0.1f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.1f, -0.7f, -0.1f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.1f, -0.7f, -0.1f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.1f, -0.7f,  0.1f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.1f, -0.7f,  0.1f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.1f, -0.7f,  0.1f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.1f, -0.7f, -0.1f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.1f,  0.7f, -0.1f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.1f,  0.7f, -0.1f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.1f,  0.7f,  0.1f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.1f,  0.7f,  0.1f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.1f,  0.7f,  0.1f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.1f,  0.7f, -0.1f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

float deskVertices[] = {
    -0.4f, -0.03f, -0.8f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.4f, -0.03f, -0.8f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.4f,  0.03f, -0.8f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.4f,  0.03f, -0.8f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.4f,  0.03f, -0.8f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.4f, -0.03f, -0.8f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.4f, -0.03f,  0.8f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.4f, -0.03f,  0.8f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.4f,  0.03f,  0.8f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.4f,  0.03f,  0.8f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.4f,  0.03f,  0.8f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.4f, -0.03f,  0.8f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.4f,  0.03f,  0.8f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.4f,  0.03f, -0.8f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.4f, -0.03f, -0.8f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.4f, -0.03f, -0.8f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.4f, -0.03f,  0.8f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.4f,  0.03f,  0.8f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.4f,  0.03f,  0.8f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.4f,  0.03f, -0.8f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.4f, -0.03f, -0.8f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.4f, -0.03f, -0.8f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.4f, -0.03f,  0.8f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.4f,  0.03f,  0.8f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.4f, -0.03f, -0.8f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.4f, -0.03f, -0.8f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.4f, -0.03f,  0.8f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.4f, -0.03f,  0.8f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.4f, -0.03f,  0.8f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.4f, -0.03f, -0.8f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.4f,  0.03f, -0.8f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.4f,  0.03f, -0.8f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.4f,  0.03f,  0.8f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.4f,  0.03f,  0.8f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.4f,  0.03f,  0.8f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.4f,  0.03f, -0.8f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

float bedCubeVertices[] = {
    -0.3f, -0.4f, -0.3f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.3f, -0.4f, -0.3f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.3f,  0.4f, -0.3f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.3f,  0.4f, -0.3f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.3f,  0.4f, -0.3f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.3f, -0.4f, -0.3f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.3f, -0.4f,  0.3f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.3f, -0.4f,  0.3f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.3f,  0.4f,  0.3f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.3f,  0.4f,  0.3f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.3f,  0.4f,  0.3f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.3f, -0.4f,  0.3f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.3f,  0.4f,  0.3f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.3f,  0.4f, -0.3f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.3f, -0.4f, -0.3f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.3f, -0.4f, -0.3f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.3f, -0.4f,  0.3f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.3f,  0.4f,  0.3f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.3f,  0.4f,  0.3f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.3f,  0.4f, -0.3f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.3f, -0.4f, -0.3f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.3f, -0.4f, -0.3f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.3f, -0.4f,  0.3f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.3f,  0.4f,  0.3f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.3f, -0.4f, -0.3f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.3f, -0.4f, -0.3f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.3f, -0.4f,  0.3f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.3f, -0.4f,  0.3f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.3f, -0.4f,  0.3f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.3f, -0.4f, -0.3f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.3f,  0.4f, -0.3f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.3f,  0.4f, -0.3f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.3f,  0.4f,  0.3f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.3f,  0.4f,  0.3f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.3f,  0.4f,  0.3f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.3f,  0.4f, -0.3f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

float bedVertices[] = {
    -0.8f, -0.03f, -0.3f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.8f, -0.03f, -0.3f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.8f,  0.03f, -0.3f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.8f,  0.03f, -0.3f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.8f,  0.03f, -0.3f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.8f, -0.03f, -0.3f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.8f, -0.03f,  0.3f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.8f, -0.03f,  0.3f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.8f,  0.03f,  0.3f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.8f,  0.03f,  0.3f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.8f,  0.03f,  0.3f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.8f, -0.03f,  0.3f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.8f,  0.03f,  0.3f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.8f,  0.03f, -0.3f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.8f, -0.03f, -0.3f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.8f, -0.03f, -0.3f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.8f, -0.03f,  0.3f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.8f,  0.03f,  0.3f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

     0.8f,  0.03f,  0.3f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.8f,  0.03f, -0.3f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.8f, -0.03f, -0.3f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.8f, -0.03f, -0.3f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.8f, -0.03f,  0.3f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.8f,  0.03f,  0.3f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

    -0.8f, -0.03f, -0.3f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
     0.8f, -0.03f, -0.3f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.8f, -0.03f,  0.3f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.8f, -0.03f,  0.3f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    -0.8f, -0.03f,  0.3f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
    -0.8f, -0.03f, -0.3f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,

    -0.8f,  0.03f, -0.3f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     0.8f,  0.03f, -0.3f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.8f,  0.03f,  0.3f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.8f,  0.03f,  0.3f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    -0.8f,  0.03f,  0.3f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
    -0.8f,  0.03f, -0.3f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
};

float lightVertices[] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };

float sofaCubeVertices[] = {
    -0.2f, -0.15f, -0.8f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.2f, -0.15f, -0.8f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.2f,  0.15f, -0.8f,  0.0f,  0.0f, -1.0f,  1.0f, 4.0f,
     0.2f,  0.15f, -0.8f,  0.0f,  0.0f, -1.0f,  1.0f, 4.0f,
    -0.2f,  0.15f, -0.8f,  0.0f,  0.0f, -1.0f,  0.0f, 4.0f,
    -0.2f, -0.15f, -0.8f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.2f, -0.15f,  0.8f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.2f, -0.15f,  0.8f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.2f,  0.15f,  0.8f,  0.0f,  0.0f, 1.0f,   1.0f, 4.0f,
     0.2f,  0.15f,  0.8f,  0.0f,  0.0f, 1.0f,   1.0f, 4.0f,
    -0.2f,  0.15f,  0.8f,  0.0f,  0.0f, 1.0f,   0.0f, 4.0f,
    -0.2f, -0.15f,  0.8f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.2f,  0.15f,  0.8f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.2f,  0.15f, -0.8f, -1.0f,  0.0f,  0.0f,  1.0f, 4.0f,
    -0.2f, -0.15f, -0.8f, -1.0f,  0.0f,  0.0f,  0.0f, 4.0f,
    -0.2f, -0.15f, -0.8f, -1.0f,  0.0f,  0.0f,  0.0f, 4.0f,
    -0.2f, -0.15f,  0.8f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.2f,  0.15f,  0.8f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.2f,  0.15f,  0.8f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.2f,  0.15f, -0.8f,  1.0f,  0.0f,  0.0f,  1.0f, 4.0f,
     0.2f, -0.15f, -0.8f,  1.0f,  0.0f,  0.0f,  0.0f, 4.0f,
     0.2f, -0.15f, -0.8f,  1.0f,  0.0f,  0.0f,  0.0f, 4.0f,
     0.2f, -0.15f,  0.8f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.2f,  0.15f,  0.8f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.2f, -0.15f, -0.8f,  0.0f, -1.0f,  0.0f,  0.0f, 4.0f,
     0.2f, -0.15f, -0.8f,  0.0f, -1.0f,  0.0f,  1.0f, 4.0f,
     0.2f, -0.15f,  0.8f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.2f, -0.15f,  0.8f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.2f, -0.15f,  0.8f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.2f, -0.15f, -0.8f,  0.0f, -1.0f,  0.0f,  0.0f, 4.0f,

    -0.2f,  0.15f, -0.8f,  0.0f,  1.0f,  0.0f,  0.0f, 4.0f,
     0.2f,  0.15f, -0.8f,  0.0f,  1.0f,  0.0f,  1.0f, 4.0f,
     0.2f,  0.15f,  0.8f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.2f,  0.15f,  0.8f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.2f,  0.15f,  0.8f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.2f,  0.15f, -0.8f,  0.0f,  1.0f,  0.0f,  0.0f, 4.0f
};

float armRestVertices[] = {
    -0.2f, -0.1f, -0.1f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.2f, -0.1f, -0.1f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.2f,  0.1f, -0.1f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.2f,  0.1f, -0.1f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.2f,  0.1f, -0.1f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.2f, -0.1f, -0.1f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.2f, -0.1f,  0.1f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.2f, -0.1f,  0.1f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.2f,  0.1f,  0.1f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.2f,  0.1f,  0.1f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.2f,  0.1f,  0.1f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.2f, -0.1f,  0.1f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.2f,  0.1f,  0.1f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.2f,  0.1f, -0.1f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.2f, -0.1f, -0.1f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.2f, -0.1f, -0.1f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.2f, -0.1f,  0.1f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.2f,  0.1f,  0.1f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.2f,  0.1f,  0.1f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.2f,  0.1f, -0.1f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.2f, -0.1f, -0.1f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.2f, -0.1f, -0.1f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.2f, -0.1f,  0.1f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.2f,  0.1f,  0.1f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.2f, -0.1f, -0.1f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.2f, -0.1f, -0.1f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.2f, -0.1f,  0.1f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.2f, -0.1f,  0.1f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.2f, -0.1f,  0.1f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.2f, -0.1f, -0.1f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.2f,  0.1f, -0.1f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.2f,  0.1f, -0.1f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.2f,  0.1f,  0.1f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.2f,  0.1f,  0.1f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.2f,  0.1f,  0.1f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.2f,  0.1f, -0.1f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

float sofaVertices[] = {
    -0.05f, -0.4f, -0.8f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.05f, -0.4f, -0.8f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.05f,  0.4f, -0.8f,  0.0f,  0.0f, -1.0f,  1.0f, 2.0f,
     0.05f,  0.4f, -0.8f,  0.0f,  0.0f, -1.0f,  1.0f, 2.0f,
    -0.05f,  0.4f, -0.8f,  0.0f,  0.0f, -1.0f,  0.0f, 2.0f,
    -0.05f, -0.4f, -0.8f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.05f, -0.4f,  0.8f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.05f, -0.4f,  0.8f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.05f,  0.4f,  0.8f,  0.0f,  0.0f, 1.0f,   1.0f, 2.0f,
     0.05f,  0.4f,  0.8f,  0.0f,  0.0f, 1.0f,   1.0f, 2.0f,
    -0.05f,  0.4f,  0.8f,  0.0f,  0.0f, 1.0f,   0.0f, 2.0f,
    -0.05f, -0.4f,  0.8f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.05f,  0.4f,  0.8f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.05f,  0.4f, -0.8f, -1.0f,  0.0f,  0.0f,  1.0f, 2.0f,
    -0.05f, -0.4f, -0.8f, -1.0f,  0.0f,  0.0f,  0.0f, 2.0f,
    -0.05f, -0.4f, -0.8f, -1.0f,  0.0f,  0.0f,  0.0f, 2.0f,
    -0.05f, -0.4f,  0.8f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.05f,  0.4f,  0.8f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.05f,  0.4f,  0.8f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.05f,  0.4f, -0.8f,  1.0f,  0.0f,  0.0f,  1.0f, 2.0f,
     0.05f, -0.4f, -0.8f,  1.0f,  0.0f,  0.0f,  0.0f, 2.0f,
     0.05f, -0.4f, -0.8f,  1.0f,  0.0f,  0.0f,  0.0f, 2.0f,
     0.05f, -0.4f,  0.8f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.05f,  0.4f,  0.8f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.05f, -0.4f, -0.8f,  0.0f, -1.0f,  0.0f,  0.0f, 2.0f,
     0.05f, -0.4f, -0.8f,  0.0f, -1.0f,  0.0f,  1.0f, 2.0f,
     0.05f, -0.4f,  0.8f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.05f, -0.4f,  0.8f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.05f, -0.4f,  0.8f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.05f, -0.4f, -0.8f,  0.0f, -1.0f,  0.0f,  0.0f, 2.0f,

    -0.05f,  0.4f, -0.8f,  0.0f,  1.0f,  0.0f,  0.0f, 2.0f,
     0.05f,  0.4f, -0.8f,  0.0f,  1.0f,  0.0f,  1.0f, 2.0f,
     0.05f,  0.4f,  0.8f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.05f,  0.4f,  0.8f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.05f,  0.4f,  0.8f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.05f,  0.4f, -0.8f,  0.0f,  1.0f,  0.0f,  0.0f, 2.0f
};

float closetVertices[] = {
    -0.3f, -0.5f, -0.8f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.3f, -0.5f, -0.8f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.3f,  0.5f, -0.8f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.3f,  0.5f, -0.8f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.3f,  0.5f, -0.8f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.3f, -0.5f, -0.8f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.3f, -0.5f,  0.8f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.3f, -0.5f,  0.8f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.3f,  0.5f,  0.8f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.3f,  0.5f,  0.8f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.3f,  0.5f,  0.8f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.3f, -0.5f,  0.8f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.3f,  0.5f,  0.8f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.3f,  0.5f, -0.8f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.3f, -0.5f, -0.8f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.3f, -0.5f, -0.8f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.3f, -0.5f,  0.8f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.3f,  0.5f,  0.8f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.3f,  0.5f,  0.8f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.3f,  0.5f, -0.8f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.3f, -0.5f, -0.8f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.3f, -0.5f, -0.8f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.3f, -0.5f,  0.8f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.3f,  0.5f,  0.8f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.3f, -0.5f, -0.8f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.3f, -0.5f, -0.8f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.3f, -0.5f,  0.8f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.3f, -0.5f,  0.8f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.3f, -0.5f,  0.8f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.3f, -0.5f, -0.8f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.3f,  0.5f, -0.8f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.3f,  0.5f, -0.8f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.3f,  0.5f,  0.8f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.3f,  0.5f,  0.8f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.3f,  0.5f,  0.8f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.3f,  0.5f, -0.8f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

float closetWallVertices[] = {
     0.3f,  0.5f,  0.8f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.3f,  0.5f, -0.8f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.3f, -0.5f, -0.8f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.3f, -0.5f, -0.8f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.3f, -0.5f,  0.8f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.3f,  0.5f,  0.8f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
};

float chairVertices[] = {
    -0.4f, -0.15f, -0.4f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.4f, -0.15f, -0.4f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.4f,  0.15f, -0.4f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.4f,  0.15f, -0.4f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.4f,  0.15f, -0.4f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.4f, -0.15f, -0.4f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.4f, -0.15f,  0.4f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.4f, -0.15f,  0.4f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.4f,  0.15f,  0.4f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.4f,  0.15f,  0.4f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.4f,  0.15f,  0.4f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.4f, -0.15f,  0.4f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.4f,  0.15f,  0.4f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.4f,  0.15f, -0.4f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.4f, -0.15f, -0.4f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.4f, -0.15f, -0.4f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.4f, -0.15f,  0.4f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.4f,  0.15f,  0.4f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.4f,  0.15f,  0.4f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.4f,  0.15f, -0.4f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.4f, -0.15f, -0.4f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.4f, -0.15f, -0.4f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.4f, -0.15f,  0.4f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.4f,  0.15f,  0.4f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.4f, -0.15f, -0.4f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.4f, -0.15f, -0.4f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.4f, -0.15f,  0.4f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.4f, -0.15f,  0.4f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.4f, -0.15f,  0.4f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.4f, -0.15f, -0.4f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.4f,  0.15f, -0.4f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.4f,  0.15f, -0.4f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.4f,  0.15f,  0.4f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.4f,  0.15f,  0.4f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.4f,  0.15f,  0.4f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.4f,  0.15f, -0.4f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

float chairCubeVertices[] = {
    -0.1f, -0.5f, -0.1f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.1f, -0.5f, -0.1f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.1f,  0.5f, -0.1f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.1f,  0.5f, -0.1f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.1f,  0.5f, -0.1f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.1f, -0.5f, -0.1f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.1f, -0.5f,  0.1f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.1f, -0.5f,  0.1f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.1f,  0.5f,  0.1f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.1f,  0.5f,  0.1f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.1f,  0.5f,  0.1f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.1f, -0.5f,  0.1f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.1f,  0.5f,  0.1f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.1f,  0.5f, -0.1f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.1f, -0.5f, -0.1f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.1f, -0.5f, -0.1f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.1f, -0.5f,  0.1f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.1f,  0.5f,  0.1f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.1f,  0.5f,  0.1f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.1f,  0.5f, -0.1f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.1f, -0.5f, -0.1f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.1f, -0.5f, -0.1f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.1f, -0.5f,  0.1f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.1f,  0.5f,  0.1f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.1f, -0.5f, -0.1f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.1f, -0.5f, -0.1f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.1f, -0.5f,  0.1f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.1f, -0.5f,  0.1f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.1f, -0.5f,  0.1f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.1f, -0.5f, -0.1f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.1f,  0.5f, -0.1f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.1f,  0.5f, -0.1f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.1f,  0.5f,  0.1f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.1f,  0.5f,  0.1f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.1f,  0.5f,  0.1f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.1f,  0.5f, -0.1f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

float pictureVertices[] = {
    -0.8f, -0.6f, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.8f, -0.6f, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.8f,  0.6f, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.8f,  0.6f, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.8f,  0.6f, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.8f, -0.6f, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f
};

float doorWallVertices1[] = {
     0.3f,  0.3f,  0.2f,  1.0f,  0.0f,  0.0f,  15.0f, 0.0f,
     0.3f,  0.3f, -0.2f,  1.0f,  0.0f,  0.0f,  15.0f, 10.0f,
     0.3f, -0.3f, -0.2f,  1.0f,  0.0f,  0.0f,  0.0f, 10.0f,
     0.3f, -0.3f, -0.2f,  1.0f,  0.0f,  0.0f,  0.0f, 10.0f,
     0.3f, -0.3f,  0.2f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.3f,  0.3f,  0.2f,  1.0f,  0.0f,  0.0f,  15.0f, 0.0f
};

float doorWallVertices2[] = {
    0.3f,  0.3f,  0.05f,  1.0f,  0.0f,  0.0f,  15.0f, 0.0f,
    0.3f,  0.3f, -0.05f,  1.0f,  0.0f,  0.0f,  15.0f, 2.5f,
    0.3f, -0.3f, -0.05f,  1.0f,  0.0f,  0.0f,  0.0f, 2.5f,
    0.3f, -0.3f, -0.05f,  1.0f,  0.0f,  0.0f,  0.0f, 2.5f,
    0.3f, -0.3f,  0.05f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    0.3f,  0.3f,  0.05f,  1.0f,  0.0f,  0.0f,  15.0f, 0.0f
};

float doorWallVertices3[] = {
    0.3f,  0.2f,  0.05f,  1.0f,  0.0f,  0.0f,  10.0f, 0.0f,
    0.3f,  0.2f, -0.05f,  1.0f,  0.0f,  0.0f,  10.0f, 2.5f,
    0.3f, -0.2f, -0.05f,  1.0f,  0.0f,  0.0f,  0.0f, 2.5f,
    0.3f, -0.2f, -0.05f,  1.0f,  0.0f,  0.0f,  0.0f, 2.5f,
    0.3f, -0.2f,  0.05f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    0.3f,  0.2f,  0.05f,  1.0f,  0.0f,  0.0f,  10.0f, 0.0f
};

float doorVertices[] = {
    0.3f,  0.1f,  0.05f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    0.3f,  0.1f, -0.05f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    0.3f, -0.1f, -0.05f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    0.3f, -0.1f, -0.05f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    0.3f, -0.1f,  0.05f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    0.3f,  0.1f,  0.05f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f
};

float roadVertices[] = {
    0.6f,  0.2f,  0.05f,  1.0f,  0.0f,  0.0f,  15.0f, 0.0f,
    0.6f,  0.2f, -0.05f,  1.0f,  0.0f,  0.0f,  15.0f, 1.0f,
    -0.6f, 0.2f, -0.05f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.6f, 0.2f, -0.05f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.6f, 0.2f,  0.05f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    0.6f,  0.2f,  0.05f,  1.0f,  0.0f,  0.0f,  15.0f, 0.0f
};

// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3( 1.5f,  2.5f,  1.5f),
    glm::vec3( 1.5f,  2.5f, -1.5f),
    glm::vec3(-1.5f,  2.5f, -1.5f),
    glm::vec3(-1.5f,  2.5f,  1.5f)
};

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

bool lock = true;
bool xz = true;

class Shader
{
public:
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(string type)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        const char *vShaderCode;
        const char *fShaderCode;
        if(type == "shader")
        {
            vShaderCode = "#version 330 core\n" 
                "layout (location = 0) in vec3 aPos;\n" 
                "layout (location = 1) in vec2 aTexCoord;\n" 
                "layout (location = 2) in vec3 aNormal;\n" 
                "\n" 
                "out vec2 TexCoord;\n" 
                "out vec3 FragPos;\n" 
                "out vec3 Normal;\n" 
                "\n" 
                "uniform mat4 model;\n" 
                "uniform mat4 view;\n" 
                "uniform mat4 projection;\n" 
                "\n" 
                "void main()\n" 
                "{\n" 
                "\tFragPos = vec3(model * vec4(aPos, 1.0));\n" 
                "    Normal = mat3(transpose(inverse(model))) * aNormal;  \n" 
                "\tgl_Position = projection * view * model * vec4(aPos, 1.0f);\n" 
                "\tTexCoord = vec2(aTexCoord.x, aTexCoord.y);\n" 
                "}\0";

            fShaderCode = "#version 330 core\n" 
                "out vec4 FragColor;\n" 
                "\n" 
                "struct Material {\n" 
                "    sampler2D diffuse;\n" 
                "    sampler2D specular;\n" 
                "    float shininess;\n" 
                "}; \n" 
                "\n" 
                "struct DirLight {\n" 
                "    vec3 direction;\n" 
                "\t\n" 
                "    vec3 ambient;\n" 
                "    vec3 diffuse;\n" 
                "    vec3 specular;\n" 
                "};\n" 
                "\n" 
                "struct PointLight {\n" 
                "    vec3 position;\n" 
                "    \n" 
                "    float constant;\n" 
                "    float linear;\n" 
                "    float quadratic;\n" 
                "\t\n" 
                "    vec3 ambient;\n" 
                "    vec3 diffuse;\n" 
                "    vec3 specular;\n" 
                "};\n" 
                "\n" 
                "struct SpotLight {\n" 
                "    vec3 position;\n" 
                "    vec3 direction;\n" 
                "    float cutOff;\n" 
                "    float outerCutOff;\n" 
                "  \n" 
                "    float constant;\n" 
                "    float linear;\n" 
                "    float quadratic;\n" 
                "  \n" 
                "    vec3 ambient;\n" 
                "    vec3 diffuse;\n" 
                "    vec3 specular;\n" 
                "\n" 
                "    bool lightSwitch;\n" 
                "};\n" 
                "\n" 
                "#define NR_POINT_LIGHTS 4\n" 
                "\n" 
                "in vec3 FragPos;\n" 
                "in vec3 Normal;\n" 
                "in vec2 TexCoord;\n" 
                "\n" 
                "uniform vec3 viewPos;\n" 
                "uniform DirLight dirLight;\n" 
                "uniform PointLight pointLights[NR_POINT_LIGHTS];\n" 
                "uniform SpotLight spotLight;\n" 
                "uniform Material material;\n" 
                "\n" 
                "// function prototypes\n" 
                "vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);\n" 
                "vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n" 
                "vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n" 
                "\n" 
                "void main()\n" 
                "{    \n" 
                "    // properties\n" 
                "    vec3 norm = normalize(Normal);\n" 
                "    vec3 viewDir = normalize(viewPos - FragPos);\n" 
                "    \n" 
                "    // == =====================================================\n" 
                "    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight\n" 
                "    // For each phase, a calculate function is defined that calculates the corresponding color\n" 
                "    // per lamp. In the main() function we take all the calculated colors and sum them up for\n" 
                "    // this fragment's final color.\n" 
                "    // == =====================================================\n" 
                "    // phase 1: directional lighting\n" 
                "    vec3 result = CalcDirLight(dirLight, norm, viewDir);\n" 
                "    // phase 2: point lights\n" 
                "    for(int i = 0; i < NR_POINT_LIGHTS; i++)\n" 
                "        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    \n" 
                "    // phase 3: spot light\n" 
                "    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    \n" 
                "    \n" 
                "    FragColor = vec4(result, 1.0);\n" 
                "}\n" 
                "\n" 
                "// calculates the color when using a directional light.\n" 
                "vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)\n" 
                "{\n" 
                "    vec3 lightDir = normalize(-light.direction);\n" 
                "    // diffuse shading\n" 
                "    float diff = max(dot(normal, lightDir), 0.0);\n" 
                "    // specular shading\n" 
                "    vec3 reflectDir = reflect(-lightDir, normal);\n" 
                "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n" 
                "    // combine results\n" 
                "    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));\n" 
                "    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));\n" 
                "    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));\n" 
                "    return (ambient + diffuse + specular);\n" 
                "}\n" 
                "\n" 
                "// calculates the color when using a point light.\n" 
                "vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)\n" 
                "{\n" 
                "    vec3 lightDir = normalize(light.position - fragPos);\n" 
                "    // diffuse shading\n" 
                "    float diff = max(dot(normal, lightDir), 0.0);\n" 
                "    // specular shading\n" 
                "    vec3 reflectDir = reflect(-lightDir, normal);\n" 
                "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n" 
                "    // attenuation\n" 
                "    float distance = length(light.position - fragPos);\n" 
                "    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    \n" 
                "    // combine results\n" 
                "    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));\n" 
                "    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));\n" 
                "    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));\n" 
                "    ambient *= attenuation;\n" 
                "    diffuse *= attenuation;\n" 
                "    specular *= attenuation;\n" 
                "    return (ambient + diffuse + specular);\n" 
                "}\n" 
                "\n" 
                "// calculates the color when using a spot light.\n" 
                "vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)\n" 
                "{   \n" 
                "    vec3 lightDir = normalize(light.position - fragPos);\n" 
                "    // diffuse shading\n" 
                "    float diff = max(dot(normal, lightDir), 0.0);\n" 
                "    // specular shading\n" 
                "    vec3 reflectDir = reflect(-lightDir, normal);\n" 
                "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n" 
                "    // attenuation\n" 
                "    float distance = length(light.position - fragPos);\n" 
                "    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    \n" 
                "    // spotlight intensity\n" 
                "    float theta = dot(lightDir, normalize(-light.direction)); \n" 
                "    float epsilon = light.cutOff - light.outerCutOff;\n" 
                "    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);\n" 
                "    // combine results\n" 
                "    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));\n" 
                "    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));\n" 
                "    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));\n" 
                "    ambient *= attenuation * intensity;\n" 
                "    diffuse *= attenuation * intensity;\n" 
                "    specular *= attenuation * intensity;\n" 
                "    return (ambient + diffuse + specular);\n" 
                "}\0";
        }
        else if(type == "light")
        {
            vShaderCode = "#version 330 core\n" 
                "layout (location = 0) in vec3 aPos;\n" 
                "layout (location = 1) in vec2 aTexCoord;\n" 
                "\n" 
                "out vec2 TexCoord;\n" 
                "\n" 
                "uniform mat4 model;\n" 
                "uniform mat4 view;\n" 
                "uniform mat4 projection;\n" 
                "\n" 
                "void main()\n" 
                "{\n" 
                "\tgl_Position = projection * view * model * vec4(aPos, 1.0f);\n" 
                "\tTexCoord = vec2(aTexCoord.x, aTexCoord.y);\n" 
                "}\0";

            fShaderCode = "#version 330 core\n" 
                "out vec4 FragColor;\n" 
                "\n" 
                "uniform bool Switch;\n" 
                "\n" 
                "void main()\n" 
                "{\n" 
                "    if(Switch)\n" 
                "        FragColor = vec4(0.1); // set alle 4 vector values to 1.0\n" 
                "    else\n" 
                "        FragColor = vec4(1.0);\n" 
                "}\0";
        }
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use() 
    { 
        glUseProgram(ID); 
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) 
    { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
            }
        }
    }
};




// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    float jumpTime;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void updatePosition(Camera *ball , Camera_Movement dir)
    {
        glm::vec3 dis = this->Position - ball->Position;
        if(dir == LEFT)
        {
            this->Position.y = ball->Position.y + dis.y;
            this->Position.x = ball->Position.x - Front.x * 2.0f;
            this->Position.z = ball->Position.z - Front.z * 2.0f;
        }
        else if(dir == RIGHT)
        {
            this->Position.y = ball->Position.y + dis.y;
            this->Position.x = ball->Position.x - Front.x * 2.0f;
            this->Position.z = ball->Position.z - Front.z * 2.0f;
        }
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime, string mode)
    {
        float velocity = MovementSpeed * deltaTime;
        float tempY = Position.y;
        if(mode == "ball")
        {
            if (direction == FORWARD)
                Position += Front * velocity;
            if (direction == BACKWARD)
                Position -= Front * velocity;
            if (direction == LEFT)
                Position -= Right * velocity;
            if (direction == RIGHT)
                Position += Right * velocity;
            if (direction == UP)
                Position.y += velocity;
            if (direction == DOWN)
                Position.y -= velocity;
        }
        else if(mode == "camera")
        {
            if (direction == FORWARD)
                Position += Front * velocity;
            if (direction == BACKWARD)
                Position -= Front * velocity;
            if (direction == LEFT)
            {
                if(gamemode)
                {
                    Yaw   -= rightAngle;
                    updateCameraVectors();
                }
                else
                {
                    Position -= Right * velocity;
                }
                
            }
            if (direction == RIGHT)
            {
                if(gamemode)
                {
                    Yaw   += rightAngle;
                    updateCameraVectors();
                }
                else
                {
                    Position += Right * velocity;
                }
            }
            if (direction == UP)
                Position.y += velocity;
            if (direction == DOWN)
                Position.y -= velocity;
            if(gamemode)
                Position.y = tempY;
        }



        /*if (xz)
            Position.y = tempY;*/
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, bool lock, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
            
        updateCameraVectors();
       
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f; 
    }

    void updateCameraVectors(float _yaw , float _pitch)
    {
        Yaw = _yaw;
        Pitch = _pitch;
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        front.y = sin(glm::radians(_pitch));
        front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};

class PhysicsEngine {
public:
	PhysicsEngine();
	~PhysicsEngine();
 
	//设置空间内部边缘
	void setSceneInnerBoundary(float x1, float y1, float z1, float x2, float y2, float z2);

	bool isJumping;

	void jumpAndUpdateVelocity();    //按下space跳跃时调用
	//每帧绘制的时候更新摄像机垂直方向移动
	void updateCameraVertMovement(glm::vec3 & cameraPos);
 
private:
 
	glm::vec3 velocity;        //垂直方向速度
	glm::vec3 gravity;         //重力加速度
	glm::vec3 accelerUp;       //方向向上的加速度
 
	glm::vec4 outerBoundary;
	vector<glm::vec3> innerBoundaryMin;    //碰撞器小的x/y/z坐标
	vector<glm::vec3> innerBoundaryMax;    //碰撞器大的x/y/z坐标
};

PhysicsEngine::PhysicsEngine() {
    velocity = glm::vec3(0.f, 0.f, 0.f);
    gravity = glm::vec3(0.f, GravityAcceler, 0.f);
    accelerUp = glm::vec3(0.f, 0.f, 0.f);

    isJumping = false;
}

PhysicsEngine::~PhysicsEngine() {
}

void PhysicsEngine::setSceneInnerBoundary(float x1, float y1, float z1, float x2, float y2, float z2) {
    glm::vec3 key(x1 - BoundaryGap, y1 - BoundaryGap, z1 - BoundaryGap);
    glm::vec3 value(x2 + BoundaryGap, y2 + BoundaryGap, z2 + BoundaryGap);

    innerBoundaryMin.push_back(key);
    innerBoundaryMax.push_back(value);
}


void PhysicsEngine::jumpAndUpdateVelocity() {
	//cout << "jump" << endl;
	velocity += glm::vec3(0.f, JumpInitialSpeed, 0.f);
	accelerUp.y = 0.f;
}

//判断在xz平面，相机位置是否位于碰撞体内部
bool insideTheCollider(glm::vec3 _cameraPos, glm::vec3 _innerMin, glm::vec3 _innerMax) {
	float camX = _cameraPos.x;
	float camZ = _cameraPos.z;
	float minX = _innerMin.x;
	float minZ = _innerMin.z;
	float maxX = _innerMax.x;
	float maxZ = _innerMax.z;

	if (minX <= camX && camX <= maxX && minZ <= camZ && camZ <= maxZ)
		return true;
	else
		return false;
}

void PhysicsEngine::updateCameraVertMovement(glm::vec3 & cameraPos) {
	glm::vec3 acceleration = gravity + accelerUp;
	velocity += acceleration * GravityFactor;
	cameraPos += velocity * JumpFactor;
	//targetPos += velocity * JumpFactor;

	//if (abs(velocity.y) < 0.1f)
	//	cout << "#### cameraPos.y " << cameraPos.y << endl;

	//检测所有碰撞体
    unsigned int size = innerBoundaryMin.size();
	for (unsigned int i = 0; i < size; i++) {
		//如果在XZ平面进入碰撞体所在区域
		if (insideTheCollider(cameraPos, innerBoundaryMin[i], innerBoundaryMax[i])) {
			if (cameraPos.y - HeroHeight <= innerBoundaryMax[i][1]
				&& cameraPos.y >= innerBoundaryMax[i][1]) {              //脚接触到碰撞体顶部
				//cout << "touch the top of collider" << endl;
				isJumping = false;
				accelerUp.y = -GravityAcceler;
				velocity.y = 0.f;
				cameraPos.y = innerBoundaryMax[i][1] + HeroHeight;
				break;
			}

			if (cameraPos.y >= innerBoundaryMin[i][1] &&
				cameraPos.y - HeroHeight <= innerBoundaryMin[i][1]) {    //头接触到碰撞体底部
				//cout << "touch the bottom of collider" << endl;
				velocity.y = 0.f;
				cameraPos.y = innerBoundaryMin[i][1];
				break;
			}
		}
		else {
            //cout << "test" << endl;
			accelerUp.y = 0.f;
		}
	}
}

// camera
Camera camera(glm::vec3(0.0f, -1.0f, 2.0f));
Camera ball(glm::vec3(0.0f, -2.5f, 0.0f));
PhysicsEngine physicsEngine;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float angle = 0.0f;


glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window , Shader *shader);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const * path);
void processSingleInput(GLFWwindow *window);
glm::mat4 RotateArbitraryLine(glm::vec3 v1, glm::vec3 v2, float theta);

bool doubleClick = false;
double lastClick = 0.0f;
bool Switch = false;
bool rotate1 = false;
bool rotate2 = false;
bool smooth = false;
bool split = false;
bool split_r = false;
bool restore = false;
bool doorLock = false;
bool doorOpen = false;
bool doorisOpen = false;
bool isSplit = false;

float jumpTime = 0.3f;
float jumpClick = 0.0f;

bool jumpMode = false;

int main()
{
    // -------------------------- 初始化 --------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//核心模式

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", nullptr, nullptr);
    // 创建窗口
    if (window == nullptr)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);


    // 窗口调整大小的时候调用
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD: 用来管理OpenGL的函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }
    // ---------------------------------------------------------------
    glEnable(GL_DEPTH_TEST);


    // -------------------------- 绘制图形 --------------------------
    //生成一个VBO对象
    unsigned int VBO , VAO , lightVAO , lightVBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // 0. 复制顶点数组到缓冲中供OpenGL使用
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1 , &lightVBO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER , lightVBO);
    glBufferData(GL_ARRAY_BUFFER , sizeof(lightVertices) , lightVertices , GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int floorVBO, floorVAO;
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);

    glBindVertexArray(floorVAO);

    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int wallVBO[3] , wallVAO[3];
    glGenVertexArrays(3, wallVAO);
    glGenBuffers(3, wallVBO);

    for(int i = 0;i < 3;i ++)
    {
        glBindVertexArray(wallVAO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, wallVBO[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertices[i]), wallVertices[i], GL_STATIC_DRAW);
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    unsigned int cubeVBO , cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int deskVBO , deskVAO;
    glGenVertexArrays(1, &deskVAO);
    glGenBuffers(1, &deskVBO);

    glBindVertexArray(deskVAO);
    glBindBuffer(GL_ARRAY_BUFFER, deskVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(deskVertices), deskVertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int bedVBO , bedVAO;
    glGenVertexArrays(1, &bedVAO);
    glGenBuffers(1, &bedVBO);

    glBindVertexArray(bedVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bedVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bedVertices), bedVertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int bedCubeVBO , bedCubeVAO;
    glGenVertexArrays(1, &bedCubeVAO);
    glGenBuffers(1, &bedCubeVBO);

    glBindVertexArray(bedCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bedCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bedCubeVertices), bedCubeVertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int sofaCubeVBO , sofaCubeVAO;
    glGenVertexArrays(1, &sofaCubeVAO);
    glGenBuffers(1, &sofaCubeVBO);

    glBindVertexArray(sofaCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sofaCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sofaCubeVertices), sofaCubeVertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int armRestVBO , armRestVAO;
    glGenVertexArrays(1, &armRestVAO);
    glGenBuffers(1, &armRestVBO);

    glBindVertexArray(armRestVAO);
    glBindBuffer(GL_ARRAY_BUFFER, armRestVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(armRestVertices), armRestVertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int sofaVBO , sofaVAO;
    glGenVertexArrays(1, &sofaVAO);
    glGenBuffers(1, &sofaVBO);

    glBindVertexArray(sofaVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sofaVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sofaVertices), sofaVertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int closetVBO , closetVAO;
    glGenVertexArrays(1, &closetVAO);
    glGenBuffers(1, &closetVBO);
    glBindVertexArray(closetVAO);
    glBindBuffer(GL_ARRAY_BUFFER, closetVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(closetVertices), closetVertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    

    unsigned int closetWallVBO , closetWallVAO;
    glGenVertexArrays(1, &closetWallVAO);
    glGenBuffers(1, &closetWallVBO);
    glBindVertexArray(closetWallVAO);
    glBindBuffer(GL_ARRAY_BUFFER, closetWallVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(closetWallVertices), closetWallVertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int chairVBO , chairVAO;
    glGenVertexArrays(1, &chairVAO);
    glGenBuffers(1, &chairVBO);
    glBindVertexArray(chairVAO);
    glBindBuffer(GL_ARRAY_BUFFER, chairVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(chairVertices), chairVertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int chairCubeVBO , chairCubeVAO;
    glGenVertexArrays(1, &chairCubeVAO);
    glGenBuffers(1, &chairCubeVBO);

    glBindVertexArray(chairCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, chairCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(chairCubeVertices), chairCubeVertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int pictureVBO , pictureVAO;
    glGenVertexArrays(1, &pictureVAO);
    glGenBuffers(1, &pictureVBO);
    glBindVertexArray(pictureVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pictureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pictureVertices), pictureVertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int doorWallVBO1 , doorWallVAO1;
    glGenVertexArrays(1, &doorWallVAO1);
    glGenBuffers(1, &doorWallVBO1);

    glBindVertexArray(doorWallVAO1);
    glBindBuffer(GL_ARRAY_BUFFER, doorWallVBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(doorWallVertices1), doorWallVertices1, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int doorWallVBO2 , doorWallVAO2;
    glGenVertexArrays(1, &doorWallVAO2);
    glGenBuffers(1, &doorWallVBO2);

    glBindVertexArray(doorWallVAO2);
    glBindBuffer(GL_ARRAY_BUFFER, doorWallVBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(doorWallVertices2), doorWallVertices2, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int doorWallVBO3 , doorWallVAO3;
    glGenVertexArrays(1, &doorWallVAO3);
    glGenBuffers(1, &doorWallVBO3);

    glBindVertexArray(doorWallVAO3);
    glBindBuffer(GL_ARRAY_BUFFER, doorWallVBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(doorWallVertices3), doorWallVertices3, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int doorVBO , doorVAO;
    glGenVertexArrays(1, &doorVAO);
    glGenBuffers(1, &doorVBO);

    glBindVertexArray(doorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, doorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(doorVertices), doorVertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int roadVBO , roadVAO;
    glGenVertexArrays(1, &roadVAO);
    glGenBuffers(1, &roadVBO);

    glBindVertexArray(roadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, roadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(roadVertices), roadVertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    const int Y_SEGMENTS = 50;
    const int X_SEGMENTS = 50;

    vector<float> sphereVertices;
	vector<int> sphereIndices;
    for (int y=0;y<=Y_SEGMENTS;y++)
	{
		for (int x=0;x<=X_SEGMENTS;x++)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			sphereVertices.push_back(xPos);
			sphereVertices.push_back(yPos);
			sphereVertices.push_back(zPos);
            sphereVertices.push_back(xPos);
			sphereVertices.push_back(yPos);
			sphereVertices.push_back(zPos);
            sphereVertices.push_back(xSegment);
			sphereVertices.push_back(ySegment);
		}
	}

	//生成球的Indices
	for (int i=0;i<Y_SEGMENTS;i++)
	{
		for (int j=0;j<X_SEGMENTS;j++)
		{
			sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j+1);
			sphereIndices.push_back(i* (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
			sphereIndices.push_back(i * (X_SEGMENTS + 1) + j + 1);
		}
	}
    
    unsigned int ballVBO, ballVAO;
	glGenVertexArrays(1, &ballVAO);
	glGenBuffers(1, &ballVBO);
	//生成并绑定球体的VAO和VBO
	glBindVertexArray(ballVAO);
	glBindBuffer(GL_ARRAY_BUFFER, ballVBO);
	//将顶点数据绑定至当前默认的缓冲中
	glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);

	GLuint element_buffer_object;//EBO
	glGenBuffers(1, &element_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int), &sphereIndices[0], GL_STATIC_DRAW);

	//设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // ---------------------------------------------------------------
    Shader ourShader("shader"); // you can name your shader files however you like
    Shader lightShader("light");

    // -------------------------- 纹理 --------------------------
    unsigned int diffuseMap = loadTexture("container2.png");
    unsigned int specularMap = loadTexture("container2_specular.png");
    unsigned int floorMap = loadTexture("floor.jpg");
    unsigned int wallMap = loadTexture("wall1.jpg");
    unsigned int cubeMap = loadTexture("cube.jpg");
    unsigned int deskMap = loadTexture("desk.jpg");
    unsigned int bedMap = loadTexture("bed.png");
    unsigned int sofaMap = loadTexture("sofa.jpg");
    unsigned int sofaMap2 = loadTexture("sofa2.jpg");
    unsigned int sofaMap3 = loadTexture("sofa3.jpg");
    unsigned int closetMap = loadTexture("closet.jpg");
    unsigned int closetMap2 = loadTexture("closet2.jpg");
    unsigned int pictureMap = loadTexture("wallPicture.png");
    unsigned int wallMap2 = loadTexture("wall2.jpg");
    unsigned int doorMap = loadTexture("door.png");
    unsigned int grassMap = loadTexture("grass.jpg");
    unsigned int roadMap = loadTexture("road.jpg");
    unsigned int ballMap = loadTexture("container.png");
    
    // ---------------------------------------------------------------

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    // bind specular map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, floorMap);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, wallMap);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, cubeMap);

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, deskMap);

    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, bedMap);

    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, sofaMap);

    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_2D, sofaMap2);

    glActiveTexture(GL_TEXTURE9);
    glBindTexture(GL_TEXTURE_2D, sofaMap3);

    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, closetMap);

    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_2D, closetMap2);

    glActiveTexture(GL_TEXTURE12);
    glBindTexture(GL_TEXTURE_2D, pictureMap);

    glActiveTexture(GL_TEXTURE13);
    glBindTexture(GL_TEXTURE_2D, wallMap2);

    glActiveTexture(GL_TEXTURE14);
    glBindTexture(GL_TEXTURE_2D, doorMap);

    glActiveTexture(GL_TEXTURE15);
    glBindTexture(GL_TEXTURE_2D, grassMap);

    glActiveTexture(GL_TEXTURE16);
    glBindTexture(GL_TEXTURE_2D, roadMap);

    glActiveTexture(GL_TEXTURE17);
    glBindTexture(GL_TEXTURE_2D, ballMap);

    float angle = 0.0f;
    float rotateAngle1 = 0.0f , rotateAngle2 = 0.0f;
    float smoothDistance = 0.0f;
    float splitDistance[3] = {} , splitDistance_r[3] = {};


    //physicsEngine.setSceneOuterBoundary(-30.0f , -30.0f , 30.0f , 30.0f);
    physicsEngine.setSceneInnerBoundary(-30.0f , -2.82f , -30.0f , 30.0f , -2.81f , 30.0f);
    physicsEngine.setSceneInnerBoundary(-0.3f , -2.26f , -1.3f , 0.3f , -1.76f , -0.8f);
    //physicsEngine.setSceneInnerBoundary(-0.3f , -2.26f , -1.3f , 0.3f , -2.16f , -0.8f);
    physicsEngine.setSceneInnerBoundary(-2.86f , -1.75f , -2.96f , -2.14f , -1.45f , -1.04f);
    physicsEngine.setSceneInnerBoundary(2.4f , -2.2f , -2.7f , 3.0f , -2.0f , -1.4f);
    physicsEngine.setSceneInnerBoundary(1.9f , -2.7f , -2.3f , 2.3f , -2.5f , -1.85f);
    physicsEngine.setSceneInnerBoundary(1.9f , -2.7f , -2.3f , 2.1f , -2.3f , -1.85f);
    physicsEngine.setSceneInnerBoundary(-1.0f , -2.6f , 1.8f , 0.3f , -2.6f , 3.0f);
    physicsEngine.setSceneInnerBoundary(-2.2f , -2.9f , 1.3f , -1.8f , -2.5f , 2.6f);
    physicsEngine.setSceneInnerBoundary(-2.2f , -2.9f , 1.3f , -1.8f , -2.2f , 1.5f);
    physicsEngine.setSceneInnerBoundary(-2.2f , -2.9f , 2.4f , -1.8f , -2.2f , 2.6f);
    physicsEngine.setSceneInnerBoundary(-2.3f , -2.9f , 1.3f , -2.2f , -2.0f , 2.6f);

    // -------------------------- 渲染循环 --------------------------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 输入
        processInput(window , &ourShader);

        // 渲染指令
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //physicsEngine.outCollisionTest(camera.Position , target.Position);
        //physicsEngine.inCollisionTest(camera.Position , target.Position);
        if(xz)
        {
            if(gamemode == 0)
                physicsEngine.updateCameraVertMovement(camera.Position);
            else
                physicsEngine.updateCameraVertMovement(ball.Position);

        }


        // be sure to activate shader when setting uniforms/drawing objects
        ourShader.use();
        ourShader.setBool("lightSwitch" , true);
        ourShader.setVec3("viewPos", camera.Position);
        ourShader.setFloat("material.shininess", 32.0f);

        if(!Switch)
        {
            // directional light
            ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
            ourShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
            ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
            ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
            for(int i = 0;i <= 3;i ++)
            {
                string base = "pointLights";
                base += "[" + std::to_string(i) + "].";
                string position = base + "position" ,
                ambient = base + "ambient" ,
                diffuse = base + "diffuse" ,
                specular = base + "specular" ,
                constant = base + "constant" ,
                linear = base + "linear" ,
                quadratic = base + "quadratic";
                ourShader.setVec3(position , pointLightPositions[i]);
                ourShader.setVec3(ambient , 0.05f, 0.05f, 0.05f);
                ourShader.setVec3(diffuse , 0.8f, 0.8f, 0.8f);
                ourShader.setVec3(specular , 1.0f, 1.0f, 1.0f);
                ourShader.setFloat(constant, 1.0f);
                ourShader.setFloat(linear, 0.09);
                ourShader.setFloat(quadratic, 0.032);
            }
        }
        else
        {
            ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
            ourShader.setVec3("dirLight.ambient", 0.02f, 0.02f, 0.02f);
            ourShader.setVec3("dirLight.diffuse", 0.04f, 0.04f, 0.04f);
            ourShader.setVec3("dirLight.specular", 0.05f, 0.05f, 0.05f);
            for(int i = 0;i <= 3;i ++)
            {
                string base = "pointLights";
                base += "[" + std::to_string(i) + "].";
                string position = base + "position" ,
                ambient = base + "ambient" ,
                diffuse = base + "diffuse" ,
                specular = base + "specular" ,
                constant = base + "constant" ,
                linear = base + "linear" ,
                quadratic = base + "quadratic";
                ourShader.setVec3(position , pointLightPositions[i]);
                ourShader.setVec3(ambient , 0.1f, 0.1f, 0.1f);
                ourShader.setVec3(diffuse , 0.8f, 0.8f, 0.8f);
                ourShader.setVec3(specular , 1.0f, 1.0f, 1.0f);
                ourShader.setFloat(constant, 10.0f);
                ourShader.setFloat(linear, 0.09);
                ourShader.setFloat(quadratic, 0.032);
            }
        }
        ourShader.setVec3("spotLight.position", camera.Position);
        ourShader.setVec3("spotLight.direction", camera.Front);
        ourShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        ourShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        ourShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("spotLight.constant", 1.0f);
        ourShader.setFloat("spotLight.linear", 0.09);
        ourShader.setFloat("spotLight.quadratic", 0.032);
        // spotLight
        if(Switch)
        {
            ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
            ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
        }
        else
        {
            ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(0.0f)));
            ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(0.0f)));
        }


        // view/projection transformations
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f , -2.8f , 0.0f));
        model = glm::translate(model, ball.Position);
        float ballAngle = (ball.Position.x * ball.Position.x + ball.Position.y * ball.Position.y + ball.Position.z * ball.Position.z);
        model = glm::rotate(model, (float)glfwGetTime() * 3.0f, ball.Position);
        //model = RotateArbitraryLine(ball.Position - ball.Right , ball.Position + ball.Right , glfwGetTime());
        model = glm::scale(model, glm::vec3(0.1f)); // Make it a smaller cube
        ourShader.setMat4("model", model);
        ourShader.setInt("material.diffuse" , 17);
        ourShader.setInt("material.specular", 17);
		glBindVertexArray(ballVAO);
		glDrawElements(GL_TRIANGLES, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);

        // render containers
        // calculate the model matrix for each object and pass it to shader before drawing
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f , -2.5f , -1.0f));
        model = glm::scale(model, glm::vec3(0.5f));
        if(rotate1)
        {
            if(rotateAngle1 <= 0.78539815f)
            {
                rotateAngle1 += deltaTime * 1.0f;
            }
        }
        if(rotate2)
        {
            rotateAngle2 += deltaTime * 2.0f;
        }
        if(!restore)
        {
            model = glm::rotate(model, rotateAngle1, glm::vec3(1.0f, 1.0f, 1.0f));
            model = glm::rotate(model, rotateAngle2, glm::vec3(1.0f, 1.0f, -1.0f));
        }
        else
        {
            rotateAngle1 = 0.0f;
            rotateAngle2 = 0.0f;
        }
        
        ourShader.setMat4("model", model);
        ourShader.setInt("material.diffuse" , 0);
        ourShader.setInt("material.specular", 1);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        
        // render container
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(6.0f));

        ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);
        ourShader.setInt("material.diffuse" , 2);
        ourShader.setInt("material.specular", 2);

        glBindVertexArray(floorVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        model = glm::translate(model, glm::vec3(0.0f, 4.4999f, 0.0f));
        model = glm::scale(model, glm::vec3(10.0f));

        ourShader.setMat4("model", model);
        ourShader.setInt("material.diffuse" , 15);
        ourShader.setInt("material.specular", 15);

        glBindVertexArray(floorVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 6.0f, 0.0f));
        model = glm::scale(model, glm::vec3(6.0f));

        ourShader.setMat4("model", model);
        ourShader.setInt("material.diffuse" , 13);
        ourShader.setInt("material.specular", 13);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        ourShader.setInt("material.diffuse" , 3);
        ourShader.setInt("material.specular", 3);
        for(int i = 0;i < 3;i ++)
        {
            // render container
            model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(10.0f));

            ourShader.setMat4("model", model);
            ourShader.setMat4("view", view);
            ourShader.setMat4("projection", projection);
            
            glBindVertexArray(wallVAO[i]);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        model = glm::translate(model , glm::vec3(0.0f , 0.0f , -0.1f));
        ourShader.setMat4("model", model);
        glBindVertexArray(doorWallVAO1);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        model = glm::translate(model , glm::vec3(0.0f , 0.0f , 0.35f));
        ourShader.setMat4("model", model);
        glBindVertexArray(doorWallVAO2);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        model = glm::translate(model , glm::vec3(0.0f , 0.1f , -0.1f));
        ourShader.setMat4("model", model);
        glBindVertexArray(doorWallVAO3);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        if(doorOpen)
            angle += deltaTime * 1.5f;
        if(doorLock)
            angle -= deltaTime * 1.5f;
        if(angle > 2.0f || angle < 0.0f)
        {
            if(doorLock)
            {
                doorLock = false;
                doorisOpen = false;
                angle = 0.0f;
            }
            if(doorOpen)
            {
                doorOpen = false;
                doorisOpen = true;
                angle = 2.0f;
            }  
        }

        model = glm::mat4(1.0f);
        model = RotateArbitraryLine(glm::vec3(3.0f , -2.5f , 1.0f) , glm::vec3(3.0f , 0.0f , 1.0f) , -angle);
        model = glm::translate(model , glm::vec3(0.0f , -2.0f , 1.5f));
        model = glm::scale(model, glm::vec3(10.0f));
        ourShader.setMat4("model", model);
        ourShader.setInt("material.diffuse" , 14);
        ourShader.setInt("material.specular", 14);
        glBindVertexArray(doorVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        model = glm::mat4(1.0f);
        model = glm::translate(model , glm::vec3(9.0f , -5.0f , 1.5f));
        model = glm::scale(model, glm::vec3(10.0f));
        ourShader.setMat4("model", model);
        ourShader.setInt("material.diffuse" , 16);
        ourShader.setInt("material.specular", 16);
        glBindVertexArray(roadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        for(int i = 0;i < 3;i ++)
        {
            if(split)
            {
                splitDistance[i] += 0.8f * deltaTime * i;
                if(splitDistance[i] > 1.2f * i)
                {
                    splitDistance[i] = 1.2f * i;
                    isSplit = true;
                }
            }
            else if(split_r)
            {
                splitDistance[i] -= 0.8f * deltaTime * i;
                if(splitDistance[i] < 0.0f)
                {
                    splitDistance[i] = 0.0f;
                    isSplit = false;
                }
            }
            if(restore)
            {
                splitDistance[i] = 0.0f;
                isSplit = false;
            }
            model = glm::mat4(1.0f);
            model = glm::translate(model , glm::vec3(2.6f , -2.72f , -2.5f + splitDistance[i]));
            model = glm::scale(model, glm::vec3(0.4f));
            ourShader.setMat4("model" , model);
            ourShader.setMat4("view", view);
            ourShader.setMat4("projection", projection);
            ourShader.setInt("material.diffuse" , 4);
            ourShader.setInt("material.specular", 4);

            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            model = glm::translate(model , glm::vec3(0.8f , 0.0f , 0.0f));
            ourShader.setMat4("model" , model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            model = glm::translate(model , glm::vec3(0.0f , 0.0f , 2.0f));
            ourShader.setMat4("model" , model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            model = glm::translate(model , glm::vec3(-0.8f , 0.0f , 0.0f));
            ourShader.setMat4("model" , model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            model = glm::translate(model , glm::vec3(0.275f , 0.775f , -1.0f));
            model = glm::scale(model, glm::vec3(1.8f));
            ourShader.setMat4("model" , model);
            ourShader.setInt("material.diffuse" , 5);
            ourShader.setInt("material.specular", 5);

            glBindVertexArray(deskVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            model = glm::mat4(1.0f);
            model = glm::translate(model , glm::vec3(2.0f , -2.6f , -2.1f + splitDistance[i]));
            
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f));
            ourShader.setMat4("model" , model);
            ourShader.setInt("material.diffuse" , 8);
            ourShader.setInt("material.specular", 8);

            glBindVertexArray(chairVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::translate(model , glm::vec3(0.0f , -0.25f , 0.25f));
            ourShader.setMat4("model" , model);

            glDrawArrays(GL_TRIANGLES, 0, 36);

            model = glm::translate(model , glm::vec3(-0.36f , -0.35f , -0.3f));
            model = glm::scale(model, glm::vec3(0.4f));
            ourShader.setMat4("model" , model);
            ourShader.setInt("material.diffuse" , 5);
            ourShader.setInt("material.specular", 5);

            glBindVertexArray(chairCubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            model = glm::translate(model , glm::vec3(1.8f , 0.0f , 0.0f));
            ourShader.setMat4("model" , model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            model = glm::translate(model , glm::vec3(0.0f , 0.0f , 1.5f));
            ourShader.setMat4("model" , model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            model = glm::translate(model , glm::vec3(-1.8f , 0.0f , 0.0f));
            ourShader.setMat4("model" , model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        for(int i = 0;i <= 1;i ++)
        {
            model = glm::mat4(1.0f);
            if(smooth)
                smoothDistance += deltaTime * 1.5f;
            else if(restore)
                smoothDistance = 0.0f;
            float dis = sin(smoothDistance);
            model = glm::translate(model , glm::vec3(-0.15f - 0.545f * i + dis , -2.955f , 2.0f));
            model = glm::scale(model, glm::vec3(0.1f));
            ourShader.setMat4("model" , model);
            ourShader.setMat4("view", view);
            ourShader.setMat4("projection", projection);
            ourShader.setInt("material.diffuse" , 5);
            ourShader.setInt("material.specular", 5);

            glBindVertexArray(bedCubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            model = glm::translate(model , glm::vec3(4.0f , 0.0f , 0.0f));
            ourShader.setMat4("model" , model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            model = glm::translate(model , glm::vec3(0.0f , 0.0f , 9.0f));
            ourShader.setMat4("model" , model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            model = glm::translate(model , glm::vec3(-4.0f , 0.0f , 0.0f));
            ourShader.setMat4("model" , model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            model = glm::translate(model , glm::vec3(2.0f , 0.61f , -4.5f));
            model = glm::scale(model, glm::vec3(6.8f));
            ourShader.setMat4("model" , model);
            ourShader.setInt("material.diffuse" , 6);
            ourShader.setInt("material.specular", 6);

            glBindVertexArray(deskVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        model = glm::mat4(1.0f);
        model = glm::translate(model , glm::vec3(-2.0f , -2.8799f , 2.0f));
        model = glm::scale(model, glm::vec3(0.8f));
        ourShader.setMat4("model" , model);
        ourShader.setInt("material.diffuse" , 7);
        ourShader.setInt("material.specular", 7);

        glBindVertexArray(sofaCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::translate(model , glm::vec3(0.0f , 0.25f , 0.7f));
        model = glm::scale(model, glm::vec3(1.0f));
        ourShader.setMat4("model" , model);
        ourShader.setInt("material.diffuse" , 8);
        ourShader.setInt("material.specular", 8);

        glBindVertexArray(armRestVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::translate(model , glm::vec3(0.0f , 0.0f , -1.4f));
        ourShader.setMat4("model" , model);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::translate(model , glm::vec3(-0.25f , 0.0f , 0.7f));
        ourShader.setMat4("model" , model);
        ourShader.setInt("material.diffuse" , 9);
        ourShader.setInt("material.specular", 9);

        glBindVertexArray(sofaVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::translate(model , glm::vec3(-2.5f , -2.399f , -2.0f));
        model = glm::scale(model, glm::vec3(1.2f));
        ourShader.setMat4("model" , model);
        ourShader.setInt("material.diffuse" , 10);
        ourShader.setInt("material.specular", 10);

        glBindVertexArray(closetVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::translate(model , glm::vec3(0.001f , 0.0f , 0.0f));
        model = glm::scale(model, glm::vec3(1.0f));
        ourShader.setMat4("model" , model);
        ourShader.setInt("material.diffuse" , 11);
        ourShader.setInt("material.specular", 11);

        glBindVertexArray(closetWallVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.999f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f));

        ourShader.setMat4("model", model);
        ourShader.setInt("material.diffuse" , 12);
        ourShader.setInt("material.specular", 12);

        glBindVertexArray(pictureVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // also draw the lamp object
        lightShader.use();
        lightShader.setBool("Switch" , Switch);
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);

        glBindVertexArray(lightVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            lightShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // 检查并调用事件，交换缓冲
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 删除着色器对象
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1 , &lightVBO);
    glDeleteVertexArrays(1, &floorVAO);
    glDeleteBuffers(1 , &floorVBO);
    glDeleteVertexArrays(1, &bedCubeVAO);
    glDeleteBuffers(1 , &bedCubeVBO);
    glDeleteVertexArrays(1, &deskVAO);
    glDeleteBuffers(1 , &deskVBO);
    for(int i = 0;i < 4;i ++)
    {
        glDeleteVertexArrays(1, &wallVAO[i]);
        glDeleteBuffers(1 , &wallVBO[i]);
    }
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1 , &cubeVBO);
    glDeleteVertexArrays(1, &bedVAO);
    glDeleteBuffers(1 , &bedVBO);
    glDeleteVertexArrays(1, &sofaCubeVAO);
    glDeleteBuffers(1 , &sofaCubeVBO);
    glDeleteVertexArrays(1, &armRestVAO);
    glDeleteBuffers(1 , &armRestVBO);
    glDeleteVertexArrays(1, &sofaVAO);
    glDeleteBuffers(1 , &sofaVBO);
    glDeleteVertexArrays(1, &closetVAO);
    glDeleteBuffers(1 , &closetVBO);
    glDeleteVertexArrays(1, &closetWallVAO);
    glDeleteBuffers(1 , &closetWallVBO);
    glDeleteVertexArrays(1, &chairVAO);
    glDeleteBuffers(1 , &chairVBO);
    glDeleteVertexArrays(1, &chairCubeVAO);
    glDeleteBuffers(1 , &chairCubeVBO);
    glDeleteVertexArrays(1, &pictureVAO);
    glDeleteBuffers(1 , &pictureVBO);

    // 释放/删除之前的分配的所有资源
    glfwTerminate();
    return 0;
    // ---------------------------------------------------------------
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window , Shader *ourShader)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime, "camera");
        if(gamemode)
            ball.ProcessKeyboard(FORWARD, deltaTime, "ball");
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime, "camera");
        if(gamemode)
            ball.ProcessKeyboard(BACKWARD, deltaTime, "ball");
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !gamemode)
    {
        camera.ProcessKeyboard(LEFT, deltaTime, "camera");
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !gamemode)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime, "camera");
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, 2.5f * deltaTime, "camera");
        if(gamemode)
            ball.ProcessKeyboard(FORWARD, 2.5f * deltaTime, "ball");
    }
    if (glfwGetKey(window , GLFW_KEY_SPACE) == GLFW_PRESS && !xz)
    {
        camera.ProcessKeyboard(UP , deltaTime, "camera");
        if(gamemode)
            ball.ProcessKeyboard(UP , deltaTime, "ball");
    }
    if (glfwGetKey(window , GLFW_KEY_V) == GLFW_PRESS && !xz)
    {
        camera.ProcessKeyboard(DOWN , deltaTime, "camera");
        if(gamemode)
            ball.ProcessKeyboard(DOWN , deltaTime, "ball");
    }

    if (glfwGetKey(window , GLFW_KEY_R) == GLFW_PRESS)
    {
        camera.Position = glm::vec3(0.0f, 0.0f, 0.0f);
        ball.Position = glm::vec3(0.0f, -2.5f, 0.0f);
    }
    if (glfwGetKey(window , GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotate1 = false;
        rotate2 = false;
        smooth = false;
        split = false;
        split_r = false;
        doorLock = true;
        doorOpen = false;
    }
    if (glfwGetKey(window , GLFW_KEY_U) == GLFW_PRESS)
    {
        restore = true;
        rotate1 = false;
        rotate2 = false;
        smooth = false;
        split = false;
        split_r = false;
        doorLock = true;
        doorOpen = false;
    }
    if (glfwGetKey(window , GLFW_KEY_1) == GLFW_PRESS)
    {
        rotate1 = true;
        restore = false;
    }
    if (glfwGetKey(window , GLFW_KEY_2) == GLFW_PRESS)
    {
        rotate2 = true;
        restore = false;
    }
    if (glfwGetKey(window , GLFW_KEY_3) == GLFW_PRESS)
    {
        smooth = true;
        restore = false;
    }
    if (glfwGetKey(window , GLFW_KEY_4) == GLFW_PRESS)
    {
        split = true;
        split_r = false;
        if(doorisOpen)
        {
            doorisOpen = false;
            doorLock = true;
            doorOpen = false;
        }
        restore = false;
    }
    if (glfwGetKey(window , GLFW_KEY_5) == GLFW_PRESS)
    {
        split = false;
        split_r = true;
        restore = false;
    }
    if (glfwGetKey(window , GLFW_KEY_I) == GLFW_PRESS)
    {
        cout << "camera: " << camera.Position.x << ' ' << camera.Position.y << ' ' << camera.Position.z << endl;
        cout << "ball: " << ball.Position.x << ' ' << ball.Position.y << ' ' << ball.Position.z << endl;
    }
    //physicsEngine.outCollisionTest(camera.Position, camera.Position);
	//physicsEngine.inCollisionTest(camera.Position, camera.Position);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (doubleClick && key == GLFW_KEY_SPACE && action == GLFW_PRESS && glfwGetTime() - lastClick < 0.2f)
    {
        xz = !xz;
        doubleClick = false;
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        lastClick = glfwGetTime();
        doubleClick = true;
        if(xz)
        {
            if (!physicsEngine.isJumping)
            {
                physicsEngine.jumpAndUpdateVelocity();
            }
            physicsEngine.isJumping = true;
        }
    }
    if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
    {
        lock = !lock;
        if(!lock)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        Switch = !Switch;
    }
    if (key == GLFW_KEY_G && action == GLFW_PRESS)
    {
        if(doorisOpen)
        {
            doorLock = true;
            if(doorOpen)
                doorOpen = false;
            restore = false;
        }
        else
        {
            if(!split)
                doorOpen = true;
            if(doorLock)
                doorLock = false;
            restore = false;
        }
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS && gamemode)
    {
        camera.ProcessKeyboard(LEFT, deltaTime, "camera");
        camera.updatePosition(&ball , LEFT);
        ball.updateCameraVectors(camera.Yaw , camera.Pitch);
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS && gamemode)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime, "camera");
        camera.updatePosition(&ball , RIGHT);
        ball.updateCameraVectors(camera.Yaw , camera.Pitch);
    }
    if(key == GLFW_KEY_0 && action == GLFW_PRESS)
    {
        if(gamemode)
        {
            gamemode = 0;
        }
        else
        {
            gamemode = 1;
        }
        
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if(!lock)
    {
        camera.ProcessMouseMovement(xoffset, yoffset , lock);
    }
        

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        cout << "Texture failed to load at path: " << path << endl;
        stbi_image_free(data);
    }

    return textureID;
}

glm::mat4 RotateArbitraryLine(glm::vec3 v1, glm::vec3 v2, float theta)
{
    glm::mat4 rmatrix;
    float a = v1.x;
    float b = v1.y;
    float c = v1.z;

    glm::vec3 p1 = v2 - v1;
    glm::vec3 p = glm::normalize(p1);

    float u = p.x;
    float v = p.y;
    float w = p.z;

    float uu = u * u;
    float uv = u * v;
    float uw = u * w;
    float vv = v * v;
    float vw = v * w;
    float ww = w * w;
    float au = a * u;
    float av = a * v;
    float aw = a * w;
    float bu = b * u;
    float bv = b * v;
    float bw = b * w;
    float cu = c * u;
    float cv = c * v;
    float cw = c * w;

    float costheta = glm::cos(theta);
    float sintheta = glm::sin(theta);

    rmatrix[0][0] = uu + (vv + ww) * costheta;
    rmatrix[0][1] = uv * (1 - costheta) + w * sintheta;
    rmatrix[0][2] = uw * (1 - costheta) - v * sintheta;
    rmatrix[0][3] = 0;

    rmatrix[1][0] = uv * (1 - costheta) - w * sintheta;
    rmatrix[1][1] = vv + (uu + ww) * costheta;
    rmatrix[1][2] = vw * (1 - costheta) + u * sintheta;
    rmatrix[1][3] = 0;

    rmatrix[2][0] = uw * (1 - costheta) + v * sintheta;
    rmatrix[2][1] = vw * (1 - costheta) - u * sintheta;
    rmatrix[2][2] = ww + (uu + vv) * costheta;
    rmatrix[2][3] = 0;

    rmatrix[3][0] = (a * (vv + ww) - u * (bv + cw)) * (1 - costheta) + (bw - cv) * sintheta;
    rmatrix[3][1] = (b * (uu + ww) - v * (au + cw)) * (1 - costheta) + (cu - aw) * sintheta;
    rmatrix[3][2] = (c * (uu + vv) - w * (au + bv)) * (1 - costheta) + (av - bu) * sintheta;
    rmatrix[3][3] = 1;

    return rmatrix;
}