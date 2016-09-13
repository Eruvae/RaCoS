#ifndef ROCKET_H
#define ROCKET_H

#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DInput>
#include <Qt3DLogic>
#include <Qt3DExtras>

class Rocket : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(float theta READ theta WRITE setTheta NOTIFY thetaChanged)

public:
    explicit Rocket(Qt3DCore::QNode *parent = 0);
    ~Rocket();

    Qt3DRender::QMesh *mesh;
    Qt3DCore::QTransform *transform;
    Qt3DExtras::QDiffuseMapMaterial *material;
    Qt3DRender::QTextureImage *texture;

    float theta() const;

public slots:
    void setTheta(float theta);

signals:
    void thetaChanged(float theta);

private:
    float ani_theta;

protected:
    void updateTransform();

};

#endif // ROCKET_H
