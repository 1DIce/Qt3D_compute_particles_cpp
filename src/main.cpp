//=============================================================================================================
/**
* @file     computeframegraph.h
*
* @brief     ComputeParticles main
*
*/


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================


#include "computeframegraph.h"
#include "computematerial.h"

//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QGuiApplication>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QSphereGeometry>
#include <Qt3DExtras/QFirstPersonCameraController>
#include <QByteArray>
#include <Qt3DCore>
#include <Qt3DRender>
#include <iostream>

//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace Qt3DRender;

//*************************************************************************************************************
//=============================================================================================================
// GLOBAL VARIABLES
//=============================================================================================================

const int PARTICLE_COUNT = 50 * 1024;

//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATION
//=============================================================================================================

QByteArray buildParticleBuffer();

//*************************************************************************************************************
//=============================================================================================================
// MAIN
//=============================================================================================================

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    Qt3DExtras::Qt3DWindow view;

    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

    //init buffer
    Qt3DRender::QBuffer *particleBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
    QByteArray particleByteArray = buildParticleBuffer();
    particleBuffer->setData(particleByteArray);

    //Compute material
    COMPUTESHADER::ComputeMaterial *computeMaterial = new COMPUTESHADER::ComputeMaterial();
    computeMaterial->setVertexBuffer(particleBuffer);

    //Compute framegraph
    COMPUTESHADER::ComputeFramegraph *framegraph = new COMPUTESHADER::ComputeFramegraph();

    //compute entity
    Qt3DCore::QEntity *particleComputeEntity = new Qt3DCore::QEntity(rootEntity);
    QComputeCommand *particlesComputeJob = new QComputeCommand();
    particleComputeEntity->addComponent(particlesComputeJob);
    particleComputeEntity->addComponent(computeMaterial);

    //Attributes of Geometry
    QAttribute *particlePositionDataAttribute = new QAttribute();
    particlePositionDataAttribute->setName(QStringLiteral("particlePosition"));
    particlePositionDataAttribute->setAttributeType(QAttribute::VertexAttribute);
    particlePositionDataAttribute->setVertexBaseType(QAttribute::Float);
    particlePositionDataAttribute->setVertexSize(3);
    particlePositionDataAttribute->setDivisor(1);
    particlePositionDataAttribute->setByteOffset(0);
    particlePositionDataAttribute->setByteStride(12 * (int)sizeof(float));
    particlePositionDataAttribute->setBuffer(particleBuffer);

    QAttribute *particleColorDataAttribute = new QAttribute();
    particleColorDataAttribute->setName(QStringLiteral("particleColor"));
    particleColorDataAttribute->setAttributeType(QAttribute::VertexAttribute);
    particleColorDataAttribute->setVertexBaseType(QAttribute::Float);
    particleColorDataAttribute->setVertexSize(3);
    particleColorDataAttribute->setDivisor(1);
    particleColorDataAttribute->setByteOffset(8 * (int)sizeof(float));
    particleColorDataAttribute->setByteStride(12 * (int)sizeof(float));
    particleColorDataAttribute->setBuffer(particleBuffer);

    //Geometry
    Qt3DExtras::QSphereGeometry *sphereGeometry = new Qt3DExtras::QSphereGeometry();
    sphereGeometry->setRings(10);
    sphereGeometry->setSlices(10);
    sphereGeometry->setRadius(1);
    sphereGeometry->addAttribute(particlePositionDataAttribute);
    sphereGeometry->addAttribute(particleColorDataAttribute);


    //particle render entity
    Qt3DCore::QEntity *particleRenderEntity = new Qt3DCore::QEntity(rootEntity);
    QGeometryRenderer *particleRenderer = new QGeometryRenderer();
    particleRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Triangles);
    particleRenderer->setInstanceCount(PARTICLE_COUNT);
    particleRenderer->setIndexOffset(0);
    particleRenderer->setFirstInstance(0);
    particleRenderer->setGeometry(sphereGeometry);

    particleRenderEntity->addComponent(particleRenderer);
    particleRenderEntity->addComponent(computeMaterial);


    //create camera
    QCamera *pCamera = new QCamera;
    pCamera->setProjectionType(QCameraLens::PerspectiveProjection);
    pCamera->setViewCenter(QVector3D(0, 0, 0));
    pCamera->setPosition(QVector3D(0, 0, -800.0));
    pCamera->setNearPlane(0.1f);
    pCamera->setFarPlane(1000.0f);
    pCamera->setFieldOfView(25.0f);
    pCamera->setAspectRatio(1.33f);
    framegraph->setCamera(pCamera);

    Qt3DExtras::QFirstPersonCameraController *pCamController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
    pCamController->setCamera(pCamera);


    view.setRootEntity(rootEntity);
    view.setActiveFrameGraph(framegraph);

    view.show();

    return app.exec();
}


//*************************************************************************************************************

QByteArray buildParticleBuffer()
{
    const int byteSizeOfParticleData = 12;
    const float factor = 500.0f;
    QByteArray bufferData;
    bufferData.resize(PARTICLE_COUNT * byteSizeOfParticleData * (int)sizeof(float));
    float *rawVertexArray = reinterpret_cast<float *>(bufferData.data());

    for(int i = 0 ; i < PARTICLE_COUNT; i++)
    {
        const int positionIdx = i * byteSizeOfParticleData;
        const int velocityIdx = i * byteSizeOfParticleData + 4;
        const int colorIdx = i * byteSizeOfParticleData + 8;

        for(int j = 0; j < 3; j++)
        {
            rawVertexArray[positionIdx + j] = ((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) - 0.5f) * factor;
            rawVertexArray[velocityIdx + j] = (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) * 2.0f;
            rawVertexArray[colorIdx + j] = 0.75f + std::sin(((i / 1024.0f) + j * 0.333f) * 6.0f) * 0.25f;
        }
        rawVertexArray[positionIdx + 3] = 1.0f;
        rawVertexArray[velocityIdx + 3] = 0.0f;
        rawVertexArray[colorIdx + 3] = 1.0f;
    }
    return bufferData;
}


