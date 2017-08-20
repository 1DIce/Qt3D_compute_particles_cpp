//=============================================================================================================
/**
* @file     computematerial.cpp
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*
* @brief    ComputeMaterial class definition.
*
*/


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "computematerial.h"
#include <iostream>


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================
#include <Qt3DRender>

//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace COMPUTESHADER;
using namespace Qt3DRender;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE GLOBAL METHODS
//=============================================================================================================

const int PARTICLE_COUNT = 1024 * 50;

//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

ComputeMaterial::ComputeMaterial(Qt3DCore::QNode *parent)
: QMaterial(parent)
, m_fFinalCollisonFactor(0.2f)
, m_fParticleStep(0.4f)
, m_pEffect(new QEffect)
, m_pParticleStepParameter(new QParameter(QStringLiteral("particleStep"), m_fParticleStep))
, m_pCollisionParameter(new QParameter(QStringLiteral("finalCollisionFactor"), m_fFinalCollisonFactor))
, m_pParticlesParameter(new QParameter)
, m_pComputeShader(new QShaderProgram)
, m_pComputeRenderPass(new QRenderPass)
, m_pComputeFilterKey(new QFilterKey)
, m_pComputeTechnique(new QTechnique)
, m_pDrawShader(new QShaderProgram)
, m_pDrawRenderPass(new QRenderPass)
, m_pDrawFilterKey(new QFilterKey)
, m_pDrawTechnique(new QTechnique)
, m_pSinParameter(new QParameter)
, m_pTimer(new QTimer(this))
, m_pStorageParameter(new QParameter)
, m_pShaderStorage(new Qt3DRender::QBuffer(Qt3DRender::QBuffer::ShaderStorageBuffer))
{
    this->init();
}


//*************************************************************************************************************

void ComputeMaterial::setVertexBuffer(Qt3DRender::QBuffer *inBuffer)
{
    m_pParticlesParameter->setName(QStringLiteral("Particles"));
    
    //Set the buffer as parameter data
    QVariant tempVariant;
    tempVariant.setValue(inBuffer);
    m_pParticlesParameter->setValue(tempVariant);
    m_pComputeRenderPass->addParameter(m_pParticlesParameter);
}


void ComputeMaterial::updateSinUniform()
{

    const float pi = std::acos(-1);
    static float t = 0.0f;
    const float sinT = std::sin(t);
    m_pSinParameter->setValue(sinT);
    t += pi / 160.0f;

}


//*************************************************************************************************************

void ComputeMaterial::init()
{

    //TEST
    ///Synchro tests

    //init shader storage buffer
    m_pShaderStorage->setUsage(Qt3DRender::QBuffer::StreamRead);
    m_pShaderStorage->setData(buildShaderStorage());

    m_pStorageParameter->setName(QStringLiteral("ColorStorage"));

    m_pStorageParameter->setValue(QVariant::fromValue(m_pShaderStorage.data()));

    connect(m_pTimer, &QTimer::timeout,this, &ComputeMaterial::updateShaderStorage);
    m_pTimer->start(10);

//    m_pSinParameter->setName(QStringLiteral("sinUniform"));
//    m_pSinParameter->setValue(0.0f);

//    connect(m_pTimer, &QTimer::timeout,this, &ComputeMaterial::updateSinUniform);
//    m_pTimer->start(50);
    ////////

    //Compute part
    //Set shader
    m_pComputeShader->setComputeShaderCode(QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/particles.csh"))));

    m_pComputeRenderPass->setShaderProgram(m_pComputeShader);

    //Set OpenGL version
    m_pComputeTechnique->graphicsApiFilter()->setApi(QGraphicsApiFilter::OpenGL);
    m_pComputeTechnique->graphicsApiFilter()->setMajorVersion(4);
    m_pComputeTechnique->graphicsApiFilter()->setMinorVersion(3);
    m_pComputeTechnique->graphicsApiFilter()->setProfile(QGraphicsApiFilter::CoreProfile);

    //Set filter Keys
    m_pComputeFilterKey->setName(QStringLiteral("type"));
    m_pComputeFilterKey->setValue(QStringLiteral("compute"));

    //Add to technique
    m_pComputeTechnique->addFilterKey(m_pComputeFilterKey);
    m_pComputeTechnique->addRenderPass(m_pComputeRenderPass);

    //Draw part
    //Set shader
    m_pDrawShader->setVertexShaderCode(QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/particles.vert"))));
    m_pDrawShader->setFragmentShaderCode(QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/particles.frag"))));

    m_pDrawRenderPass->setShaderProgram(m_pDrawShader);

    //Set OpenGL version
    m_pDrawTechnique->graphicsApiFilter()->setApi(QGraphicsApiFilter::OpenGL);
    m_pDrawTechnique->graphicsApiFilter()->setMajorVersion(4);
    m_pDrawTechnique->graphicsApiFilter()->setMinorVersion(3);
    m_pDrawTechnique->graphicsApiFilter()->setProfile(QGraphicsApiFilter::CoreProfile);

    //Set filter Keys
    m_pDrawFilterKey->setName(QStringLiteral("type"));
    m_pDrawFilterKey->setValue(QStringLiteral("draw"));

    //Add to technique
    m_pDrawTechnique->addFilterKey(m_pDrawFilterKey);
    m_pDrawTechnique->addRenderPass(m_pDrawRenderPass);

    //TEST
    m_pComputeRenderPass->addParameter((m_pStorageParameter));
    //m_pComputeRenderPass->addParameter(m_pSinParameter);
    //

    //Effect
    //Link shader and uniforms
    m_pEffect->addTechnique(m_pComputeTechnique);
    m_pEffect->addTechnique(m_pDrawTechnique);

    //Add to material
    this->addParameter(m_pParticleStepParameter);
    this->addParameter(m_pCollisionParameter);

    this->setEffect(m_pEffect);
}

QByteArray ComputeMaterial::buildShaderStorage()
{

        const int dataSize = 4;

        QByteArray bufferData;
        bufferData.resize(PARTICLE_COUNT * dataSize * (int)sizeof(float));
        float *rawVertexArray = reinterpret_cast<float *>(bufferData.data());

        for(int i = 0 ; i < PARTICLE_COUNT; i++)
        {
            const int colorIdx = i * dataSize;

            for(int j = 0; j < 3; j++)
            {       
                rawVertexArray[colorIdx + j] = 1.0;
            }
            rawVertexArray[colorIdx + 3] = 1.0f;
        }
        return bufferData;

}

void ComputeMaterial::updateShaderStorage()
{
    const int dataSize = 4;

    QByteArray bufferData;
    bufferData.resize(PARTICLE_COUNT * dataSize * (int)sizeof(float));
    float *rawVertexArray = reinterpret_cast<float *>(bufferData.data());

    const float pi = std::acos(-1);
    static float t = 0.0f;

    for(int i = 0 ; i < PARTICLE_COUNT; i++)
    {
        const int colorIdx = i * dataSize;

        for(int j = 0; j < 3; j++)
        {
            qint64 time = QDateTime::currentMSecsSinceEpoch();
            const float sinT = std::sin(time);
            t += pi / 16.0f;

            rawVertexArray[colorIdx + j] = 1.0f;
        }

    }

    m_pShaderStorage->setData(bufferData);

    m_pStorageParameter->setValue(QVariant::fromValue(m_pShaderStorage.data()));
}


//*************************************************************************************************************
