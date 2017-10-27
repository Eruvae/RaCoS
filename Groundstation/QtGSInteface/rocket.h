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
    Q_PROPERTY(float roll READ roll WRITE setRoll NOTIFY rollChanged)
    //Q_PROPERTY(float yaw READ yaw WRITE setYaw NOTIFY yawChanged)
    //Q_PROPERTY(float pitch READ pitch WRITE setPitch NOTIFY pitchChanged)

public:
    explicit Rocket(Qt3DCore::QNode *parent = 0);
    ~Rocket();

    Qt3DRender::QMesh *mesh;
    Qt3DCore::QTransform *transform;
    Qt3DExtras::QDiffuseMapMaterial *material;
    Qt3DRender::QTextureImage *texture;

    float roll() const;
    //float yaw() const;
    //float pitch() const;

public slots:
    void setRoll(float roll);
    //void setYaw(float yaw);
    //void setPitch(float pitch);

signals:
    void rollChanged(float roll);
    //void yawChanged(float yaw);
    //void pitchChanged(float pitch);

private:
    float ani_roll;
    //float ani_yaw;
    //float ani_pitch;

protected:
    void updateTransform();

};

#endif // ROCKET_H
