//=============================================================================================================
/**
* @file     computeframegraph.h
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*
* @brief     ComputeFramegraph class declaration.
*
*/

#ifndef COMPUTESHADER_COMPUTEFRAMEGRAPH_H
#define COMPUTESHADER_COMPUTEFRAMEGRAPH_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QSharedPointer>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

namespace Qt3DRender {
    class QViewport;
    class QRenderSurfaceSelector;
    class QClearBuffers;
    class QNoDraw;
    class QDispatchCompute;
    class QTechniqueFilter;
    class QCameraSelector;
    class QCamera;
    class QFilterKey;
    class QMemoryBarrier;
}

//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE COMPUTESHADER
//=============================================================================================================

namespace COMPUTESHADER {


//*************************************************************************************************************
//=============================================================================================================
// COMPUTESHADER FORWARD DECLARATIONS
//=============================================================================================================


//=============================================================================================================
/**
* create a new Framegraph with viewPort as root.
*
* @brief Brief description of this class.
*/

class ComputeFramegraph : public Qt3DRender::QViewport
{

public:
    typedef QSharedPointer<ComputeFramegraph> SPtr;            /**< Shared pointer type for ComputeFramegraph. */
    typedef QSharedPointer<const ComputeFramegraph> ConstSPtr; /**< Const shared pointer type for ComputeFramegraph. */

    //=========================================================================================================
    /**
    * Constructs a ComputeFramegraph object.
    */
    explicit ComputeFramegraph(Qt3DCore::QNode *parent = 0);
    
//    void setDrawKey(Qt3DRender::QFilterKey *pFilterKey);
    
//    void setComputeKey(Qt3DRender::QFilterKey *pFilterKey);
    
    void setWorkGroups(const int x, const int y, const int z);
    
    void setCamera(Qt3DRender::QCamera *pCamera);
    

protected:

private:
    void init();
    
    QPointer<Qt3DRender::QRenderSurfaceSelector> m_pSurfaceSelector;

    QPointer<Qt3DRender::QClearBuffers> m_pClearBuffers;

    QPointer<Qt3DRender::QNoDraw> m_pNoDraw;

    QPointer<Qt3DRender::QDispatchCompute> m_pDispatchCompute;

    QPointer<Qt3DRender::QFilterKey> m_pComputeKey;
    QPointer<Qt3DRender::QFilterKey> m_pDrawKey;

    QPointer<Qt3DRender::QTechniqueFilter> m_pComputeFilter;
    QPointer<Qt3DRender::QTechniqueFilter> m_pDrawFilter;

    QPointer<Qt3DRender::QCameraSelector> m_pCameraSelector;

    QPointer<Qt3DRender::QMemoryBarrier> m_pMemoryBarrier;
};


//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================


} // namespace COMPUTESHADER

#endif // COMPUTESHADER_COMPUTEFRAMEGRAPH_H
