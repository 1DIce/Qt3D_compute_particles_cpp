//=============================================================================================================
/**
* @file     computeframegraph.cpp
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     Month, Year
*
* @section  LICENSE
*
* Copyright (C) Year, Lars Debor and Matti Hamalainen. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that
* the following conditions are met:
*     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
*       following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
*       the following disclaimer in the documentation and/or other materials provided with the distribution.
*     * Neither the name of MNE-CPP authors nor the names of its contributors may be used
*       to endorse or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
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
// INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <Qt3DRender>
#include <Qt3DCore/QNode>

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

}


//*************************************************************************************************************
