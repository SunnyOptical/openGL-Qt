# openGL-Qt


## Project:01_STATIC_TRIANGLE
> 使用现代OpenGL绘制一个静态的三角形.

**VBO:** VBO全称Vertex Buffer Object,顶点缓冲对象，是一段在GPU中开辟的缓冲区，用来存放和顶点相关的数据，比如顶点坐标，UV坐标,颜色信息，法线等等。

**VAO:** VAO全称Vertex Array Object,顶点数组对象，存放保存顶点缓冲对象的指针，和其顶点数据的分布关系，便于解析顶点数据的内存模型。

### 效果
![](./MDResource/01_STATIC_TRIANGLE/01_STATIC_TRIANGLE.png)

## Project02:02_TRIANGLE_WITH_ELEMENT
> 使用VAO，VBO,EBO绘制一个四边形

**EBO:** EBO也叫IBO(Index Buffer Object),指使用索引号来标识需要绘制的顶点，使用EBO的好处是，几乎不会定义重复的点，假设需要绘制一个立方体，如果使用GL_QUADS
绘制时，会存在某个点被多次定义，而使用IBO/EBO的方式就不会。

**EBO部分定义**

```c++
GLfloat vertex[]={
      -0.5f,-0.5f,0.0f,
       0.0f,0.5f,0.0f,
       0.5f,-0.5f,0.0f,
       0.0f,-1.0f,0.0f
    };
GLuint index[] = {
    0,1,2,
    2,0,3};
```

### 效果
![](./MDResource/02_TRIANGLE_WITH_ELEMENT/02_TRIANGLE_WITH_ELEMENT.png)