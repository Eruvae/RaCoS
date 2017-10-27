#include "rocket.h"

Rocket::Rocket(Qt3DCore::QNode *parent)
    : Qt3DCore::QEntity(parent)
    , mesh(new Qt3DRender::QMesh())
    , transform(new Qt3DCore::QTransform())
    , material(new Qt3DExtras::QDiffuseMapMaterial())
    , texture(new Qt3DRender::QTextureImage())
{
    //mesh->setSource(QUrl(QStringLiteral("file:/home/racos/Dropbox/Team-Ordner „RaCoS“/Software/RaCoS/Groundstation/QtGSInteface/assets/rocket/rocket.obj")));
    //mesh->setSource(QUrl(QStringLiteral("file:/home/racos/Dropbox/Team-Ordner „RaCoS“/Software/RaCoS/Groundstation/QtGSInteface/assets/rocket/liftoffconfig.obj")));
    mesh->setSource(QUrl(QStringLiteral("file:/C:/Users/TobiZ/Dropbox/Team-Ordner „RaCoS“/Software/RaCoS/Groundstation/QtGSInteface/assets/rocket/rocket.obj")));

    // OLD_ROCKET
    transform->setScale(1.0f);
    transform->setRotationY(60.0f);
    // OLD_ROCKET_END

    /* NEW_ROCKET
    //transform->setTranslation(QVector3D(0.0f, 0.0f, 0.0f));
    transform->setScale(1.0f);
    transform->setRotationZ(220.0f);
    //transform->setRotationY(90.0f);
    //transform->setRotationX(90.0f);

    // NEW_ROCKET_END */

    //texture->setSource(QUrl(QStringLiteral("file:/home/racos/Dropbox/Team-Ordner „RaCoS“/Software/RaCoS/Groundstation/QtGSInteface/assets/rocket/rxTexCustom.png")));
    //texture->setSource(QUrl(QStringLiteral("file:/home/racos/Dropbox/Team-Ordner „RaCoS“/Software/RaCoS/Groundstation/QtGSInteface/assets/rocket/liftoffconfigTex.png")));
    texture->setSource(QUrl(QStringLiteral("file:/C:/Users/TobiZ/Dropbox/Team-Ordner „RaCoS“/Software/RaCoS/Groundstation/QtGSInteface/assets/rocket/rxTexCustom.png")));
    material->diffuse()->addTextureImage(texture);
    //material->setShininess(10);
    material->setShininess(10.0f);
    material->setSpecular(QColor::fromRgbF(0.75f, 0.75f, 0.75f, 1.0f));
    material->setAmbient(QColor::fromRgbF(0.75f, 0.75f, 0.75f, 1.0f));

    addComponent(mesh);
    addComponent(transform);
    addComponent(material);
}

Rocket::~Rocket()
{
}

void Rocket::updateTransform()
{
    transform->setRotationY(ani_roll/*ani_roll*/);
    //transform->setRotationX(ani_yaw/*ani_pitch*/);
    //transform->setRotationZ(ani_pitch);
}

float Rocket::roll() const
{
    return ani_roll;
}

/*
float Rocket::yaw() const
{
    return ani_yaw;
}

float Rocket::pitch() const
{
    return ani_pitch;
}
*/

void Rocket::setRoll(float roll)
{
    if (ani_roll == roll)
        return;

    ani_roll = roll;
    emit rollChanged(roll);
    updateTransform();
}

/*
void Rocket::setYaw(float yaw)
{
    if (ani_yaw == yaw)
        return;

    ani_yaw = yaw;
    emit rollChanged(yaw);
    updateTransform();
}

void Rocket::setPitch(float pitch)
{
    if (ani_pitch == pitch)
        return;

    ani_pitch = pitch;
    emit rollChanged(pitch);
    updateTransform();
}
*/
