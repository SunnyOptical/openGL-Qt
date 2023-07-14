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
private:
    QOpenGLVertexArrayObject            m_vao;
    QOpenGLBuffer                       m_vbo;
    QOpenGLShaderProgram                m_shaderProgram;

};

#endif // OPENGLWIDGET_H
