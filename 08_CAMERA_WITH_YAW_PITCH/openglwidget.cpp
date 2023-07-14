#include "openglwidget.h"
#include <QDebug>
#include <qmath.h>
openGLWidget::openGLWidget(QOpenGLWidget *parent)
    :QOpenGLWidget(parent),
    m_vbo(QOpenGLBuffer::VertexBuffer),
    m_ibo(QOpenGLBuffer::IndexBuffer),
    m_texture(QOpenGLTexture::Target2D),
    m_cameraPosition(0.0f,2.0f,3.0f),
    m_cameraUp(0.0,1.0,0.0),
    m_pitch(-0.650796),
    m_yaw(-82.98)
{
    QSurfaceFormat format;
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
    m_timerId = startTimer(50);
    m_rotateAngle = 0;
    m_sensitivity = 0.01;
    //openGL的摄像机中，pitch(俯仰角)是xz平面与y之间的夹角，yaw(航向角)是x与z轴之间的夹角
    //此时方向向量不再是从摄像机位置指向远点

    //x轴：由于只计算方向，不关心大小，所以都拿单位向量计算，x = cos(m_pitch)*cos(m_yaw)

    m_cameraDirection = QVector3D(cos(m_pitch)*cos(m_yaw),sin(m_pitch),cos(m_pitch)*sin(m_yaw));
    //本身应该设置鼠标跟踪，鼠标移动改变摄像机的俯仰角和航向角，由于是测试，使用鼠标只有在窗口中按下后才开始捕获处理
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

    float vertex[] = {
        // 前面 (z = 0.5)
        // 顶点坐标 (x, y, z)   纹理坐标 (u, v)
        // 位置              //纹理坐标
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
    m_shaderProgram.setAttributeBuffer(0,GL_FLOAT,0,3,5*sizeof(GLfloat));
    m_shaderProgram.enableAttributeArray(0);

    m_shaderProgram.setAttributeBuffer(1,GL_FLOAT,3*sizeof(GLfloat),2,5*sizeof(GLfloat));
    m_shaderProgram.enableAttributeArray(1);
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


    //矩阵初始化为单位阵
    m_modelMatrix.setToIdentity();
    m_viewMatrix.setToIdentity();
    m_projectMatrix.setToIdentity();

    //旋转变换
    //m_modelMatrix.rotate(m_rotateAngle,0.0,1.0,0.0);


    m_viewMatrix.lookAt(m_cameraPosition,m_cameraPosition+m_cameraDirection,m_cameraUp);

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
    Q_UNUSED(event)
    //通过按键可以看出，是物体的移动模拟出摄像机运动，而并非摄像机自己移动,openGL中本身并无摄像机
    qreal moveSpeed = 0.02;
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
        //摄像机需要向右
        QVector3D cameraLeft = QVector3D::crossProduct(m_cameraUp,-m_cameraDirection).normalized();
        m_cameraPosition += cameraLeft * moveSpeed;
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

void openGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    float offsetX = event->x() - m_lastMousePosition.x();
    float offsetY = m_lastMousePosition.y() - event->y();//注意坐标方向
    offsetX *= m_sensitivity;
    offsetY *= m_sensitivity;
    //更新lastpos
    m_lastMousePosition = event->pos();
    //更新欧拉角
    m_yaw += offsetX;
    m_pitch += offsetY;
    if(m_pitch >= M_PI / 2.0){
        m_pitch = M_PI / 2.0 - 0.1;
    }
    else if(m_pitch <= -M_PI / 2.0){
        m_pitch = -M_PI / 2.0 + 0.1;
    }
    //更新方向向量
    m_cameraDirection = QVector3D(cos(m_pitch)*cos(m_yaw),sin(m_pitch),cos(m_pitch)*sin(m_yaw));
    m_cameraDirection.normalize();
    update();
    qDebug()<<"m_pitch"<<m_pitch<<"m_yaw"<<m_yaw<<Qt::endl;
}
