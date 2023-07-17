#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H
#include <QOpenGLWidget>
#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLShader>
#include <QTimerEvent>
#include <QSet>
#include <QKeyEvent>
#include <QEvent>
class openGLWidget:public QOpenGLWidget,protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    openGLWidget(QOpenGLWidget *parent = nullptr);
    ~openGLWidget();
    void compilerShader(QOpenGLShaderProgram& _shader,const QString& _vertex,const QString fragment);
protected:
    void paintGL() override;
    void initializeGL() override;
    void resizeGL(int width,int height) override;
    void timerEvent(QTimerEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
private:
    QOpenGLVertexArrayObject            m_vao;
    QOpenGLBuffer                       m_vbo;
    QOpenGLBuffer                       m_ibo;
    QOpenGLShaderProgram                m_shaderProgram;
    QOpenGLTexture                      m_texture;

    QMatrix4x4                          m_modelMatrix;
    QMatrix4x4                          m_viewMatrix;
    QMatrix4x4                          m_projectMatrix;

    qint32                              m_timerId;
    quint32                             m_rotateAngle;
    //摄像机
    QVector3D                           m_cameraPosition;
    QVector3D                           m_cameraDirection;
    QVector3D                           m_cameraUp;
    //光照

    QVector3D                           m_lightColor;
    QVector3D                           m_lightPosition;
    QSet<Qt::Key>                       m_keyList;

    qreal                               m_yaw;      //航向角
    qreal                               m_pitch;    //俯仰角

};

#endif // OPENGLWIDGET_H
