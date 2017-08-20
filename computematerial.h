//=============================================================================================================
/**
* @file     computematerial.h
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*
*
* @brief     ComputeMaterial class declaration.
*
*/

#ifndef COMPUTESHADER_COMPUTEMATERIAL_H
#define COMPUTESHADER_COMPUTEMATERIAL_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QSharedPointer>
#include <Qt3DCore>
#include <Qt3DRender>
#include <QTimer>
#include <QByteArray>




//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

//namespace Qt3DRender {
//    class QMaterial;
//    class QEffect;
//    class QParameter;
//    class QShaderProgram;
//    class QFilterKey;
//    class QTechnique;
//    class QRenderPass;
//    class QBuffer;
//}


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
* Description of what this class is intended to do (in detail).
*
* @brief Brief description of this class.
*/

class ComputeMaterial : public Qt3DRender::QMaterial
{

public:
    typedef QSharedPointer<ComputeMaterial> SPtr;            /**< Shared pointer type for ComputeMaterial. */
    typedef QSharedPointer<const ComputeMaterial> ConstSPtr; /**< Const shared pointer type for ComputeMaterial. */

    //=========================================================================================================
    /**
    * Constructs a ComputeMaterial object.
    */
    explicit ComputeMaterial(Qt3DCore::QNode *parent = 0);
    
    /**
     * Set the vertex buffer that is used with the name Particles in shaders
     *
     * @param inBuffer
     */
    void setVertexBuffer(Qt3DRender::QBuffer *inBuffer);

    void updateSinUniform();

protected:

private:
    //=========================================================================================================
    /**
     * Init the ComputeMaterial class
     */

    void init();

    QByteArray buildShaderStorage();
    void updateShaderStorage();

    float m_fParticleStep;
    float m_fFinalCollisonFactor;

    QPointer<Qt3DRender::QEffect> m_pEffect;

    QPointer<Qt3DRender::QParameter> m_pParticleStepParameter;
    QPointer<Qt3DRender::QParameter> m_pCollisionParameter;

    //Compute Part
    QPointer<Qt3DRender::QShaderProgram> m_pComputeShader;
    QPointer<Qt3DRender::QRenderPass> m_pComputeRenderPass;
    QPointer<Qt3DRender::QParameter> m_pParticlesParameter;
    QPointer<Qt3DRender::QFilterKey> m_pComputeFilterKey;
    QPointer<Qt3DRender::QTechnique> m_pComputeTechnique;

    //test
    QPointer<Qt3DRender::QBuffer> m_pShaderStorage;
    QPointer<Qt3DRender::QParameter> m_pStorageParameter;
    QPointer<Qt3DRender::QParameter> m_pSinParameter;
    QPointer<QTimer> m_pTimer;
    //

    //Draw Part
    QPointer<Qt3DRender::QShaderProgram> m_pDrawShader;
    QPointer<Qt3DRender::QRenderPass> m_pDrawRenderPass;
    QPointer<Qt3DRender::QFilterKey> m_pDrawFilterKey;
    QPointer<Qt3DRender::QTechnique> m_pDrawTechnique;

};


//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================


} // namespace COMPUTESHADER

#endif // COMPUTESHADER_COMPUTEMATERIAL_H
