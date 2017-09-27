//=============================================================================================================
/**
* @file     computematerial.h
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
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

#include <Qt3DCore>
#include <Qt3DRender/QMaterial>


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

namespace Qt3DRender {
    class QEffect;
    class QParameter;
    class QShaderProgram;
    class QFilterKey;
    class QTechnique;
    class QRenderPass;
    class QBuffer;
}

namespace QtCore {
    class QByteArray;
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
* This class is special material for compute shader use.
*
* @brief Compute shader material.
*/

class ComputeMaterial : public Qt3DRender::QMaterial
{

public:
    //=========================================================================================================
    /**
    * Constructs a ComputeMaterial object.
    */
    explicit ComputeMaterial(Qt3DCore::QNode *parent = 0);
    
    //=========================================================================================================
    /**
     * Set the vertex buffer that is used with the name Particles in shaders
     *
     * @param inBuffer
     */
    void setVertexBuffer(Qt3DRender::QBuffer *inBuffer);


protected:

private:

    //=========================================================================================================
    /**
     * Init the ComputeMaterial class
     */
    void init();

    QByteArray buildShaderStorage();

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
