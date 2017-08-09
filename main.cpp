


#include <QGuiApplication>

#include <Qt3DExtras/Qt3DWindow>

#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QSphereGeometry>
#include <Qt3DExtras/QFirstPersonCameraController>

#include <QByteArray>
#include <Qt3DCore>
#include <Qt3DRender>
#include <QRectF>
#include <iostream>
#include <cmath>
#include <cstdlib>

const int PARTICLE_COUNT = 50 * 1024;

using namespace Qt3DRender;

QByteArray buildParticleBuffer();

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    Qt3DExtras::Qt3DWindow view;

    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();
    /**Custom material */
    QMaterial *computeMaterial = new QMaterial();

    //@TO-DO init this buffer
    Qt3DRender::QBuffer *particleBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
    QByteArray particleByteArray = buildParticleBuffer();
    particleBuffer->setData(particleByteArray);

    //@TO-DO make those parameter changeable with a gui
    float particleStep = 0.4f;
    float finalCollisionFactor = 0.2f;
    //add Parameters used in the shaders
    QParameter *particleStepParameter = new QParameter();
    particleStepParameter->setName(QStringLiteral("particleStep"));
    particleStepParameter->setValue(particleStep);
    QParameter *collisionParameter = new QParameter();
    collisionParameter->setName(QStringLiteral("finalCollisionFactor"));
    collisionParameter->setValue(finalCollisionFactor);
    computeMaterial->addParameter(particleStepParameter);
    computeMaterial->addParameter(collisionParameter);

    //Shaders
    QShaderProgram *computeShader = new QShaderProgram();
    computeShader->setComputeShaderCode(QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/particles.comp"))));

    QShaderProgram *drawShader = new QShaderProgram();
    drawShader->setVertexShaderCode(QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/particles.vert"))));
    drawShader->setFragmentShaderCode(QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/particles.frag"))));

    //compute Technique
    //Renderpass
    QRenderPass *computeRenderpass = new QRenderPass();
    computeRenderpass->setShaderProgram(computeShader);
    QParameter *particlesParameter = new QParameter();
    particlesParameter->setName(QStringLiteral("Particles"));
    // We set the buffer as the parameter data
    QVariant variant;
    variant.setValue(particleBuffer);
    particlesParameter->setValue(variant);
    computeRenderpass->addParameter(particlesParameter);

    //Filter key
    QFilterKey *computeKey = new QFilterKey();
    computeKey->setName(QStringLiteral("type"));
    computeKey->setValue(QStringLiteral("compute"));

    //compute Technique
    QTechnique *computeTechnique = new QTechnique();
    //Set OpenGL version
    computeTechnique->graphicsApiFilter()->setApi(QGraphicsApiFilter::OpenGL);
    computeTechnique->graphicsApiFilter()->setMajorVersion(4);
    computeTechnique->graphicsApiFilter()->setMinorVersion(3);
    computeTechnique->graphicsApiFilter()->setProfile(QGraphicsApiFilter::CoreProfile);

    computeTechnique->addRenderPass(computeRenderpass);
    computeTechnique->addFilterKey(computeKey);

    //draw technique
    QRenderPass *drawRenderPass = new QRenderPass();
    drawRenderPass->setShaderProgram(drawShader);
    // We assume the mesh to be drawn will also receive
    // Vertex buffers attributes that will be used to position

    //Filter key
    QFilterKey *drawKey = new QFilterKey();
    drawKey->setName(QStringLiteral("type"));
    drawKey->setValue(QStringLiteral("draw"));

    //draw technique
    QTechnique *drawTechnique = new QTechnique();
    //Set OpenGL version
    drawTechnique->graphicsApiFilter()->setApi(QGraphicsApiFilter::OpenGL);
    drawTechnique->graphicsApiFilter()->setMajorVersion(4);
    drawTechnique->graphicsApiFilter()->setMinorVersion(3);
    drawTechnique->graphicsApiFilter()->setProfile(QGraphicsApiFilter::CoreProfile);

    drawTechnique->addRenderPass(drawRenderPass);
    drawTechnique->addFilterKey(drawKey);

    //Effect
    ///Link shader and uniforms
    ///Neither techniques or parameters can be directly attached
    QEffect *computeEffect = new QEffect();
    computeEffect->addTechnique(computeTechnique);
    computeEffect->addTechnique(drawTechnique);

    computeMaterial->setEffect(computeEffect);

    
    /**Compute Framegraph */
    //create a new Framegraph with viewPort as root
    QViewport *viewPort = new QViewport();
    viewPort->setNormalizedRect(QRectF(0.0f, 0.0f, 1.0f, 1.0f));

    //surfaceSelector child of viewPort
    QRenderSurfaceSelector *surfaceSelector = new QRenderSurfaceSelector(viewPort);

    //clear buffer child of surface selector
    Qt3DRender::QClearBuffers *clearBuffer = new Qt3DRender::QClearBuffers(surfaceSelector);
    clearBuffer->setBuffers(Qt3DRender::QClearBuffers::ColorDepthBuffer);
    clearBuffer->setClearColor(Qt::black);
    
    //no draw child of clear buffer
    //this prevents the renderer from rendering any primitive.
    QNoDraw *noDraw = new QNoDraw(clearBuffer);
    
    //compute pass
    //dispatch compute child of surface selector
    QDispatchCompute *dispatchCompute = new QDispatchCompute(surfaceSelector);
    dispatchCompute->setWorkGroupX(50);
    dispatchCompute->setWorkGroupY(1);
    dispatchCompute->setWorkGroupZ(1);
    
    //filter for the compute technique
    QTechniqueFilter *computeFilter = new QTechniqueFilter(dispatchCompute);
    computeFilter->addMatch(computeKey);

    // Draw particles from buffer computed in the Compute Pass
    QCameraSelector *cameraSelector = new QCameraSelector(surfaceSelector);
    Qt3DRender::QCamera *camera = new QCamera();
    camera->setProjectionType(QCameraLens::PerspectiveProjection);
    camera->setViewCenter(QVector3D(0, 0, 0));
    camera->setPosition(QVector3D(0, 0, -800.0));
    camera->setNearPlane(0.1f);
    camera->setFarPlane(1000.0f);
    camera->setFieldOfView(25.0f);
    camera->setAspectRatio(1.33f);

    cameraSelector->setCamera(camera);
    
    //filter for draw technique
    QTechniqueFilter *drawFilter = new QTechniqueFilter(cameraSelector);
    drawFilter->addMatch(drawKey);
    
    //Memory Barrier
    //required to properly synchronize drawing and compute commands on the GPU.
    QMemoryBarrier *memBarrier = new QMemoryBarrier(drawFilter);
    memBarrier->setWaitOperations(Qt3DRender::QMemoryBarrier::VertexAttributeArray);





    Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
    camController->setCamera(camera);


    //compute entity
    Qt3DCore::QEntity *particleComputeEntity = new Qt3DCore::QEntity(rootEntity);
    QComputeCommand *particlesComputeJob = new QComputeCommand();
    particlesComputeJob->setWorkGroupX(50);
    particlesComputeJob->setWorkGroupY(1);
    particlesComputeJob->setWorkGroupZ(1);
    //set workgroups here again?

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


    view.setRootEntity(rootEntity);
    view.setActiveFrameGraph(viewPort);

    // explicitly set RenderingPolicy to AlwaysRender, as changes in the
    // scene won't be reflected in actual Qt scene-graph changes (due to
    // GPU compute calls)
    view.renderSettings()->setRenderPolicy(Qt3DRender::QRenderSettings::Always);

    view.show();

    return app.exec();
}

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
