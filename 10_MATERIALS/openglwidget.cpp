#include "openglwidget.h"
#include <QDebug>
#include <qmath.h>
#include <QTime>
openGLWidget::openGLWidget(QOpenGLWidget *parent)
    :QOpenGLWidget(parent),
    m_vbo(QOpenGLBuffer::VertexBuffer),
    m_ibo(QOpenGLBuffer::IndexBuffer),
    m_texture(QOpenGLTexture::Target2D),
    m_cameraPosition(0.0f,2.0f,3.0f),
    m_cameraUp(0.0,1.0,0.0),
    m_lightColor(1.0,0.8,0.6),
    m_lightPosition(-1.0,3.0,3.0)
{
    QSurfaceFormat format;
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
    m_timerId = startTimer(50);
    m_rotateAngle = 0;
    m_lightAmbit = QVector3D(0.59225,0.59225,0.59225);
    m_lightDiff  = QVector3D(0.50754 ,0.50754, 0.50754);
    m_lightSpec  = QVector3D(0.508273, 0.508273 ,0.508273);
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

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //光照模型中需要加入法线
    float vertex[] = {
        // 前面 (z = 0.5)
        // 顶点坐标 (x, y, z)   纹理坐标 (u, v)
        // 位置              //纹理坐标
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0,0.0,-1.0,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,0.0,0.0,-1.0,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0,0.0,-1.0,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0,0.0,-1.0,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0,0.0,-1.0,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0,0.0,-1.0,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0,0.0,1.0,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,0.0,0.0,1.0,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0,0.0,1.0,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0,0.0,1.0,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,0.0,0.0,1.0,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0,0.0,1.0,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,-1.0,0.0,0.0,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,-1.0,0.0,0.0,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,-1.0,0.0,0.0,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,-1.0,0.0,0.0,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,-1.0,0.0,0.0,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,-1.0,0.0,0.0,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,1.0,0.0,0.0,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,1.0,0.0,0.0,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,1.0,0.0,0.0,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,1.0,0.0,0.0,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0,0.0,0.0,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,1.0,0.0,0.0,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0,-1.0,0.0,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0,-1.0,0.0,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,0.0,-1.0,0.0,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,0.0,-1.0,0.0,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0,-1.0,0.0,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0,-1.0,0.0,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0,1.0,0.0,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0,1.0,0.0,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0,1.0,0.0,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0,1.0,0.0,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,0.0,1.0,0.0,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0,1.0,0.0,
    };


    m_vao.create();
    m_vao.bind();

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertex,sizeof(vertex));


    m_texture.create();
    m_texture.bind();
    m_texture.setData(QImage("./texture/container.png").mirrored());
    m_texture.setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::Repeat);
    m_texture.setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::Repeat);
    m_texture.setMinificationFilter(QOpenGLTexture::Linear);


    compilerShader(m_shaderProgram,"./shaders/vertex.glsl","./shaders/fragment.glsl");
    m_shaderProgram.bind();
    m_shaderProgram.setAttributeBuffer(0,GL_FLOAT,0,3,8*sizeof(GLfloat));
    m_shaderProgram.enableAttributeArray(0);

    m_shaderProgram.setAttributeBuffer(1,GL_FLOAT,3*sizeof(GLfloat),2,8*sizeof(GLfloat));
    m_shaderProgram.enableAttributeArray(1);

    m_shaderProgram.setAttributeBuffer(2,GL_FLOAT,5*sizeof(GLfloat),2,8*sizeof(GLfloat));
    m_shaderProgram.enableAttributeArray(2);
    m_shaderProgram.release();
    m_vao.release();

}
void openGLWidget::paintGL()
{
    glEnable(GL_COLOR_BUFFER_BIT | GL_DEPTH_TEST);

    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    m_shaderProgram.bind();
    m_vao.bind();
    m_texture.bind(0);
    m_shaderProgram.setUniformValue("texture",0);

    m_shaderProgram.setUniformValue("lightColor",m_lightColor);
    m_shaderProgram.setUniformValue("lightPosition",m_lightPosition);

    m_shaderProgram.setUniformValue("materialValue.ambit", m_lightAmbit);
    m_shaderProgram.setUniformValue("materialValue.diff", m_lightDiff);
    m_shaderProgram.setUniformValue("materialValue.spec", m_lightSpec);
    m_shaderProgram.setUniformValue("materialValue.business", GLfloat(0.4 * 128));


    //矩阵初始化为单位阵
    m_modelMatrix.setToIdentity();
    m_viewMatrix.setToIdentity();
    m_projectMatrix.setToIdentity();

    //旋转变换
    m_modelMatrix.rotate(m_rotateAngle,0.0,1.0,0.0);


    m_viewMatrix.lookAt(m_cameraPosition,QVector3D(0.0,0.0,0.0),m_cameraUp);

    m_projectMatrix.perspective(45.0f,1.0*width()/(height()*1.0),0.1,100.0);

    //将矩阵的变换传递给shader

    m_shaderProgram.setUniformValue("model",m_modelMatrix);
    m_shaderProgram.setUniformValue("view",m_viewMatrix);
    m_shaderProgram.setUniformValue("projection",m_projectMatrix);

    glDrawArrays(GL_TRIANGLES,0,36);


    m_vao.release();
    m_texture.release();
    m_shaderProgram.release();
}

void openGLWidget::resizeGL(int width, int height)
{
    glViewport(0,0,width,height);
}

void openGLWidget::timerEvent(QTimerEvent *event)
{
    //通过按键可以看出，是物体的移动模拟出摄像机运动，而并非摄像机自己移动,openGL中本身并无摄像机
    static qreal moveSpeed = 0.02;
    m_rotateAngle++;
    if(m_rotateAngle >= 360){
        m_rotateAngle = 0;
    }
    if(m_keyList.contains(Qt::Key_W)){
        //摄像机需要向前
        m_cameraPosition += m_cameraDirection * moveSpeed;
    }else if(m_keyList.contains(Qt::Key_S)){
        //摄像机需要向后
        m_cameraPosition -= m_cameraDirection * moveSpeed;
    }else if(m_keyList.contains(Qt::Key_A)){
        //摄像机需要向左,需要先计算出左向量
        QVector3D cameraLeft = QVector3D::crossProduct(m_cameraUp,-m_cameraDirection).normalized();
        m_cameraPosition -= cameraLeft * moveSpeed;
    }else if(m_keyList.contains(Qt::Key_D)){
        //摄像机需要向右,左右只是方向上的问题
        QVector3D cameraLeft = QVector3D::crossProduct(m_cameraUp,-m_cameraDirection).normalized();
        m_cameraPosition += cameraLeft * moveSpeed;
    }
    m_materialsChangeFrequence++;


    if(m_materialsChangeFrequence >= 40){
        m_materialsChangeFrequence = 0;
        m_lightAmbit = QVector3D((float)qrand()/RAND_MAX+0.8,(float)qrand()/RAND_MAX+0.6,0.8+(float)qrand()/RAND_MAX);
        m_lightDiff = QVector3D((float)qrand()/RAND_MAX,(float)qrand()/RAND_MAX,(float)qrand()/RAND_MAX);
        m_lightSpec = QVector3D((float)qrand()/RAND_MAX,(float)qrand()/RAND_MAX,(float)qrand()/RAND_MAX);
        update();
        qDebug()<<"it is change "<<m_lightAmbit<<Qt::endl;
    }

    update();
}

void openGLWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W){
        m_keyList.insert(Qt::Key_W);
    }else if(event->key() == Qt::Key_S){
        m_keyList.insert(Qt::Key_S);
    }else if(event->key() == Qt::Key_A){
        m_keyList.insert(Qt::Key_A);
    }else if(event->key() == Qt::Key_D){
        m_keyList.insert(Qt::Key_D);
    }
}

void openGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W){
        m_keyList.remove(Qt::Key_W);
    }else if(event->key() == Qt::Key_S){
        m_keyList.remove(Qt::Key_S);
    }else if(event->key() == Qt::Key_A){
        m_keyList.remove(Qt::Key_A);
    }else if(event->key() == Qt::Key_D){
        m_keyList.remove(Qt::Key_D);
    }
}

