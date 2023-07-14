#include "openglwidget.h"
#include <QDebug>
openGLWidget::openGLWidget(QOpenGLWidget *parent)
    :QOpenGLWidget(parent),
    m_vbo(QOpenGLBuffer::VertexBuffer),
    m_ibo(QOpenGLBuffer::IndexBuffer),
    m_texture(QOpenGLTexture::Target2D)
{
    m_rotateAngle = 0;

    m_timerId = startTimer(50);
}

openGLWidget::~openGLWidget()
{
    m_vao.destroy();
    m_vbo.destroy();
    m_texture.destroy();
    if(m_timerId > 0){
        killTimer(m_timerId);
    }
}

void openGLWidget::compilerShader(QOpenGLShaderProgram &_shader, const QString &_vertex, const QString _fragment)
{

    bool res(false);
    res = _shader.addShaderFromSourceFile(QOpenGLShader::Vertex,_vertex);
    if(!res){
        qDebug()<<"vertex shader compiler error"<<_shader.log()<<Qt::endl;
        return ;
    }
    res = _shader.addShaderFromSourceFile(QOpenGLShader::Fragment,_fragment);
    if(!res){
        qDebug()<<"fragment shader compiler error"<<_shader.log()<<Qt::endl;
        return ;
    }
    res = _shader.link();
    if(!res){
        qDebug()<<"shader link error"<<_shader.log()<<Qt::endl;
        return ;
    }
}


void openGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glEnable(GL_COLOR_BUFFER_BIT);
    GLfloat vertex[]={
         //vertex           //uv
      -0.5f, -0.5f, 0.0f,      0.0f, 0.0f,
      -0.5f,  0.5f, 0.0f,      0.0f, 1.0f,
       0.5f,  0.5f, 0.0f,      1.0f, 1.0f,
       0.5f, -0.5f, 0.0f,      1.0f, 0.0f
    };
    GLuint index[] = {
        0,1,2,
        2,3,0
    };
    m_vao.create();
    m_vao.bind();

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertex,sizeof(vertex));

    m_ibo.create();
    m_ibo.bind();
    m_ibo.allocate(index,sizeof(index));

    m_texture.create();
    m_texture.bind();
    m_texture.setData(QImage("./texture/pic.png").mirrored());
    m_texture.setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::Repeat);
    m_texture.setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::Repeat);
    m_texture.setMinificationFilter(QOpenGLTexture::Linear);

    compilerShader(m_shaderProgram,"./shaders/vertex.glsl","./shaders/fragment.glsl");
    m_shaderProgram.bind();
    m_shaderProgram.setAttributeBuffer(0,GL_FLOAT,0,3,5*sizeof(GLfloat));
    m_shaderProgram.enableAttributeArray(0);

    m_shaderProgram.setAttributeBuffer(1,GL_FLOAT,3*sizeof(GLfloat),2,5*sizeof(GLfloat));
    m_shaderProgram.enableAttributeArray(1);
    m_shaderProgram.release();
    m_vao.release();

}
void openGLWidget::paintGL()
{
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glEnable(GL_COLOR_BUFFER_BIT);

    m_shaderProgram.bind();
    m_vao.bind();
    m_texture.bind(0);
    m_shaderProgram.setUniformValue("texture",0);


    //矩阵初始化为单位阵
    m_modelMatrix.setToIdentity();
    m_viewMatrix.setToIdentity();
    m_projectMatrix.setToIdentity();



    //缩放变换
    m_modelMatrix.scale(0.5);

    //平移变换
    m_modelMatrix.translate(-0.8,0.0,0.0);

    //旋转变换
    m_modelMatrix.rotate(m_rotateAngle,0.0,1.0,0.0);

    //将矩阵的变换传递给shader

    m_shaderProgram.setUniformValue("model",m_modelMatrix);
    m_shaderProgram.setUniformValue("view",m_viewMatrix);
    m_shaderProgram.setUniformValue("projection",m_projectMatrix);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

    //通过平移生成的图像，由于使用的是同一个模型矩阵，所以变化会叠加，如果需要变化相对独立，需要在每次使用模型矩阵之前，将其置成单位阵
    //m_modelMatrix.setToIdentity();
    m_modelMatrix.translate(1.2,0.0,0.0);
    m_modelMatrix.rotate(m_rotateAngle,1.2,0.0,0.0);
    m_shaderProgram.setUniformValue("model",m_modelMatrix);
    m_shaderProgram.setUniformValue("view",m_viewMatrix);
    m_shaderProgram.setUniformValue("projection",m_projectMatrix);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

    m_vao.release();
    m_texture.release();
    m_shaderProgram.release();
}

void openGLWidget::resizeGL(int width, int height)
{
    glViewport(0,0,width,height);
}

void openGLWidget::timerEvent(QTimerEvent *event){
    m_rotateAngle+=3;
    if(m_rotateAngle >= 360){
        m_rotateAngle = 0;
    }
    update();
}
