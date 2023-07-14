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

};

#endif // OPENGLWIDGET_H
