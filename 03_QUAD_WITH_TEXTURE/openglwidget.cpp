#include "openglwidget.h"
#include <QDebug>
openGLWidget::openGLWidget(QOpenGLWidget *parent)
    :QOpenGLWidget(parent),
    m_vbo(QOpenGLBuffer::VertexBuffer),
    m_ibo(QOpenGLBuffer::IndexBuffer),
    m_texture(QOpenGLTexture::Target2D)
{
    QSurfaceFormat format;
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);

}

openGLWidget::~openGLWidget()
{
    m_vao.destroy();
    m_vbo.destroy();
    m_texture.destroy();
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

    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);


    m_vao.release();
    m_texture.release();
    m_shaderProgram.release();
}

void openGLWidget::resizeGL(int width, int height)
{
    glViewport(0,0,width,height);
}
