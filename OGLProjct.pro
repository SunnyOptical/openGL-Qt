TEMPLATE = subdirs
SUBDIRS += 01_STATIC_TRIANGLE \ #vao和vbo绘制三角形
           02_TRIANGLE_WITH_ELEMENT \ #vao vbo ibo结合绘制三角形
           03_QUAD_WITH_TEXTURE \ #绘制一个正方形，并为其添加纹理
           04_LINEAR_TRANSFORM \ #物体的线性变换
           05_3D_CUBE \ #使用ibo绘制一个3D立方体
           06_3D_CUBE_WITH_TEXTURE \ #绘制一个带纹理贴图的3D立方体
           07_3D_WITH_CAMERA \ #摄像机与坐标系统,通过按键操作摄像机的位置,不改变其方向
           08_CAMERA_WITH_YAW_PITCH \ #支持俯仰角，航向角变化的摄像机系统
           09_PHONE_LIGHT \ #冯氏光照模型
           10_MATERIALS \ #材质系统:定义不同材质体现对光的不同反射与吸收状态
