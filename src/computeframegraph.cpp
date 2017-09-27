//=============================================================================================================
/**
* @file     computeframegraph.cpp
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*
* @brief    ComputeFramegraph class definition.
*
*/


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "computeframegraph.h"

//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <Qt3DRender/QRenderSurfaceSelector>
#include <Qt3DRender/QClearBuffers>
#include <Qt3DRender/QNoDraw>
#include <Qt3DRender/QDispatchCompute>
#include <Qt3DRender/QTechniqueFilter>
#include <Qt3DRender/QCameraSelector>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QFilterKey>
#include <Qt3DRender/QMemoryBarrier>
#include <Qt3DRender/QViewport>
#include <Qt3DCore/QNode>

//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace COMPUTESHADER;
using namespace Qt3DRender;

//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

ComputeFramegraph::ComputeFramegraph(Qt3DCore::QNode *parent)
    : QViewport(parent)
    , m_pSurfaceSelector(new QRenderSurfaceSelector(this))
    , m_pClearBuffers(new QClearBuffers(m_pSurfaceSelector))
    , m_pNoDraw(new QNoDraw(m_pClearBuffers))
    , m_pDispatchCompute(new QDispatchCompute(m_pSurfaceSelector))
    , m_pComputeFilter(new QTechniqueFilter(m_pDispatchCompute))
    , m_pCameraSelector(new QCameraSelector(m_pSurfaceSelector))
    , m_pDrawFilter(new QTechniqueFilter(m_pCameraSelector))
    , m_pMemoryBarrier(new QMemoryBarrier(m_pDrawFilter))
    , m_pDrawKey(new QFilterKey)
    , m_pComputeKey(new QFilterKey)
{
    init();
}

//*************************************************************************************************************


void ComputeFramegraph::setWorkGroups(const int x, const int y, const int z)
{
    m_pDispatchCompute->setWorkGroupX(x);
    m_pDispatchCompute->setWorkGroupY(y);
    m_pDispatchCompute->setWorkGroupZ(z);
}

//*************************************************************************************************************


void ComputeFramegraph::setCamera(QCamera *pCamera)
{
    m_pCameraSelector->setCamera(pCamera);
}

//*************************************************************************************************************


void ComputeFramegraph::init()
{
    this->setNormalizedRect(QRectF(0.0f, 0.0f, 1.0f, 1.0f));

    //Set ClearBuffers
    m_pClearBuffers->setBuffers(QClearBuffers::ColorDepthBuffer);
    m_pClearBuffers->setClearColor(Qt::black);

    //Set Workgroup size
    m_pDispatchCompute->setWorkGroupX(50);
    m_pDispatchCompute->setWorkGroupY(1);
    m_pDispatchCompute->setWorkGroupZ(1);

    //Set FilterKeys
    m_pComputeKey->setName(QStringLiteral("type"));
    m_pComputeKey->setValue(QStringLiteral("compute"));

    m_pDrawKey->setName(QStringLiteral("type"));
    m_pDrawKey->setValue(QStringLiteral("draw"));

    //Add Matches
    m_pComputeFilter->addMatch(m_pComputeKey);
    m_pDrawFilter->addMatch(m_pDrawKey);


    //Set Memory Barrier
    m_pMemoryBarrier->setWaitOperations(QMemoryBarrier::VertexAttributeArray);

    //Set Camera
//    m_pCamera->setProjectionType(QCameraLens::PerspectiveProjection);
//    m_pCamera->setViewCenter(QVector3D(0, 0, 0));
//    m_pCamera->setPosition(QVector3D(0, 0, -800.0));
//    m_pCamera->setNearPlane(0.1f);
//    m_pCamera->setFarPlane(1000.0f);
//    m_pCamera->setFieldOfView(25.0f);
//    m_pCamera->setAspectRatio(1.33f);
//    m_pCameraSelector->setCamera(m_pCamera);

}


//*************************************************************************************************************
