#include "rocket.h"

Rocket::Rocket(Qt3DCore::QNode *parent)
    : Qt3DCore::QEntity(parent)
    , mesh(new Qt3DRender::QMesh())
    , transform(new Qt3DCore::QTransform())
    , material(new Qt3DExtras::QDiffuseMapMaterial())
    , texture(new Qt3DRender::QTextureImage())
{
    mesh->setSource(QUrl(QStringLiteral("file:C:/Users/TobiZ/RaCoS/Groundstation/QtGSInteface/assets/rocket/RX_Structure_Vehicle.obj")));
    transform->setScale(0.0025f);
    transform->setRotationZ(-90.0f);
    texture->setSource(QUrl(QStringLiteral("file:C:/Users/TobiZ/RaCoS/Groundstation/QtGSInteface/assets/rocket/mat.png")));
    material->diffuse()->addTextureImage(texture);

    addComponent(mesh);
    addComponent(transform);
    addComponent(material);
}

Rocket::~Rocket()
{
}

void Rocket::updateTransform()
{
    transform->setRotationY(ani_theta);
}

float Rocket::theta() const
{
    return ani_theta;
}

void Rocket::setTheta(float theta)
{
    if (ani_theta == theta)
        return;

    ani_theta = theta;
    emit thetaChanged(theta);
    updateTransform();
}
