#include "XYZAxesComponent.h"

#include "Settings.h"
#include "LineSegmentBuilder.h"
#include "TextureBuilder.h"

XYZAxesComponent::XYZAxesComponent()
  : raiiOpenGLRenderer(this)
  , openGLContext(Settings::getInstance().getOpenGLContext())
  , initialized(false)
  , cameraMoved(false)
{

}

XYZAxesComponent::~XYZAxesComponent()
{
    if (openGLContext.makeActive())
    {
        openGLContextClosing();
    }
}

void XYZAxesComponent::mouseDown(const MouseEvent& event)
{
    viewMatrixAtMouseDown = renderingProperties->getViewMatrix();
}

void XYZAxesComponent::mouseDrag(const MouseEvent& event)
{
    c4::Matrix4D translatedViewMatrix(viewMatrixAtMouseDown);

    translatedViewMatrix.n[0][3] += float(event.getDistanceFromDragStartX()) / getWidth();
    translatedViewMatrix.n[1][3] += -float(event.getDistanceFromDragStartY()) / getHeight();

    //std::cerr << "XYZAxesComponent::mouseDrag; new translation (" << translatedViewMatrix.n[12] << ", " << translatedViewMatrix.n[13] << ")" << std::endl;
    renderingProperties->setViewMatrix(translatedViewMatrix);

    cameraMoved = true;

    openGLContext.triggerRepaint();
}

void XYZAxesComponent::mouseMagnify(const MouseEvent &event, float scaleFactor)
{
    c4::Matrix4D translatedViewMatrix(renderingProperties->getViewMatrix());
    translatedViewMatrix.n[2][3] += scaleFactor - 1;
    //std::cerr << "XYZAxesComponent::mouseDrag; scaleFactor (" << scaleFactor << ") new translation (" << translatedViewMatrix.n[14] << ")" << std::endl;
    renderingProperties->setViewMatrix(translatedViewMatrix);

    cameraMoved = true;

    openGLContext.triggerRepaint();
}

void XYZAxesComponent::newOpenGLContextCreated()
{
    renderingProperties = RenderingProperties::Ptr(new RenderingProperties(getWidth(), getHeight()));

    createAxesAndPositionLabelsForCameraPosition();
}

bool didModelTranslationChangeSignificantly(const c4::Vector4D& currentTranslation,
                                            const c4::Vector4D& newTranslation)
{
    return fabs(currentTranslation.x - newTranslation.x) > 0.01f
            || fabs(currentTranslation.y - newTranslation.y) > 0.01f
            || fabs(currentTranslation.z - newTranslation.z) > 0.01f;
}



void XYZAxesComponent::createAxesAndPositionLabelsForCameraPosition()
{
    c4::Vector4D idealCameraPositionRelativeToModel(0, 0, -15, 1);
    c4::Vector4D currentCameraPositionRelativeToModel( renderingProperties->getViewMatrix().getRow(3) );
    c4::Vector4D phaseAlignedModelTranslation = currentCameraPositionRelativeToModel - idealCameraPositionRelativeToModel;

    phaseAlignedModelTranslation.x = -roundf( phaseAlignedModelTranslation.x / 5.0f ) * 5.0f;
    phaseAlignedModelTranslation.y = -roundf( phaseAlignedModelTranslation.y / 5.0f ) * 5.0f;
    phaseAlignedModelTranslation.z = -roundf( phaseAlignedModelTranslation.z / 5.0f ) * 5.0f;
    phaseAlignedModelTranslation.w = 1;

    bool nearestXAxisChanged = false;
    if (xAxis == nullptr)
    {
        xAxis = LineSegmentBuilder::createLineSegment(openGLContext,
                                                      std::tuple<float, float, float>(-10, 0, 0),
                                                      std::tuple<float, float, float>(10, 0, 0),
                                                      Colours::red,
                                                      c4::Matrix4D(c4::Vector3D(0, 0, 0)));
        nearestXAxisChanged = true;
    }
    else if ( didModelTranslationChangeSignificantly(phaseAlignedModelTranslation, 
                                                     xAxis->getModelTransformation().getRow(3)) )
    {
        xAxis->setModelTransformation( c4::Matrix4D( phaseAlignedModelTranslation ) );
        nearestXAxisChanged = true;
    }

    bool nearestYAxisChanged = false;
    if (yAxis == nullptr)
    {
        yAxis = LineSegmentBuilder::createLineSegment(openGLContext,
                                                       std::tuple<float, float, float>(0, -10, 0),
                                                       std::tuple<float, float, float>(0, 10, 0),
                                                       Colours::green,
                                                       c4::Matrix4D(c4::Vector3D(0, 0, 0)));
        nearestYAxisChanged = true;
    }
    else if ( didModelTranslationChangeSignificantly(phaseAlignedModelTranslation, 
                                                     yAxis->getModelTransformation().getRow(3)) )
    {
        yAxis->setModelTransformation( c4::Matrix4D( phaseAlignedModelTranslation ) );
        nearestYAxisChanged = true;
    }

    bool nearestZAxisChanged = false; 
    if (zAxis == nullptr)
    {
        zAxis = LineSegmentBuilder::createLineSegment(openGLContext,
                                                       std::tuple<float, float, float>(0, 0, -100),
                                                       std::tuple<float, float, float>(0, 0, 100),
                                                       Colours::blue,
                                                       c4::Matrix4D(c4::Vector3D(0, 0, 0)));
        nearestZAxisChanged = true;
    }
    else if ( didModelTranslationChangeSignificantly(phaseAlignedModelTranslation, 
                                                     zAxis->getModelTransformation().getRow(3)) )
    {
        zAxis->setModelTransformation( c4::Matrix4D( phaseAlignedModelTranslation ) );
        nearestZAxisChanged = true;
    }

    /*
    if (nearestXAxisChanged || nearestYAxisChanged || nearestZAxisChanged)
    {
        std::cerr << "xAxisChanged <" << nearestXAxisChanged << "> yAxisChanged <" << nearestYAxisChanged << "> zAxisChanged <" << nearestZAxisChanged << ">" << std::endl;
        std::cerr << "phaseAlignedModelTranslation (" << phaseAlignedModelTranslation.x 
                                                      << ", " << phaseAlignedModelTranslation.y << ", "
                                                      << phaseAlignedModelTranslation.z << ")" << std::endl;

        if (nearestXAxisChanged)
        {
            c4::Vector3D xAxisCenter(xAxis->getBoundingBox().getCenter());
            std::cerr << "x axis (" << xAxisCenter.x << ", " << xAxisCenter.y << ", " << xAxisCenter.z << ")" << std::endl;
        }

        if (nearestYAxisChanged)
        {
            c4::Vector3D yAxisCenter(yAxis->getBoundingBox().getCenter());
            std::cerr << "y axis (" << yAxisCenter.x << ", " << yAxisCenter.y << ", " << yAxisCenter.z << ")" << std::endl;
        }

        if (nearestZAxisChanged)
        {
            c4::Vector3D zAxisCenter(zAxis->getBoundingBox().getCenter());
            std::cerr << "z axis (" << zAxisCenter.x << ", " << zAxisCenter.y << ", " << zAxisCenter.z << ")" << std::endl;
        }
    }
    */

    positionLabels.resize(6, GraphicObject::Ptr());

    float imageSize = 0.5f;

    if (nearestXAxisChanged)
    {
        String minXAxisText(TRANS("($1,$2,$3)").replace("$1", String(phaseAlignedModelTranslation.x - 5))
                                               .replace("$2", String(phaseAlignedModelTranslation.y))
                                               .replace("$3", String(phaseAlignedModelTranslation.z)));
        positionLabels[0] = TextureBuilder::createTexture(openGLContext,
                                                          TextureBuilder::createTextImage(minXAxisText, Font(12)),
                                                          imageSize,
                                                          c4::Matrix4D(c4::Vector3D(-5, imageSize, 0) + phaseAlignedModelTranslation));

        String maxXAxisText(TRANS("($1,$2,$3)").replace("$1", String(phaseAlignedModelTranslation.x + 5))
                                               .replace("$2", String(phaseAlignedModelTranslation.y))
                                               .replace("$3", String(phaseAlignedModelTranslation.z)));
        positionLabels[1] = TextureBuilder::createTexture(openGLContext,
                                                          TextureBuilder::createTextImage(maxXAxisText, Font(12)),
                                                          imageSize,
                                                          c4::Matrix4D(c4::Vector3D(5, imageSize, 0) + phaseAlignedModelTranslation));
    }

    if (nearestYAxisChanged)
    {
        String minYAxisText(TRANS("($1,$2,$3)").replace("$1", String(phaseAlignedModelTranslation.x))
                                               .replace("$2", String(phaseAlignedModelTranslation.y - 5))
                                               .replace("$3", String(phaseAlignedModelTranslation.z)));
        positionLabels[2] = TextureBuilder::createTexture(openGLContext,
                                                          TextureBuilder::createTextImage(minYAxisText, Font(12)),
                                                          imageSize,
                                                          c4::Matrix4D(c4::Vector3D(imageSize, -5, 0) + phaseAlignedModelTranslation));

        String maxYAxisText(TRANS("($1,$2,$3)").replace("$1", String(phaseAlignedModelTranslation.x))
                                               .replace("$2", String(phaseAlignedModelTranslation.y + 5))
                                               .replace("$3", String(phaseAlignedModelTranslation.z)));
        positionLabels[3] = TextureBuilder::createTexture(openGLContext,
                                                          TextureBuilder::createTextImage(maxYAxisText, Font(12)),
                                                          imageSize,
                                                          c4::Matrix4D(c4::Vector3D(imageSize, 5, 0) + phaseAlignedModelTranslation));
    }

    if (nearestZAxisChanged)
    {
        String minZAxisText(TRANS("($1,$2,$3)").replace("$1", String(phaseAlignedModelTranslation.x))
                                               .replace("$2", String(phaseAlignedModelTranslation.y))
                                               .replace("$3", String(phaseAlignedModelTranslation.z - 5)));
        positionLabels[4] = TextureBuilder::createTexture(openGLContext,
                                                          TextureBuilder::createTextImage(minZAxisText, Font(12)),
                                                          imageSize,
                                                          c4::Matrix4D(c4::Vector3D(0, imageSize, -5) + phaseAlignedModelTranslation));

        String maxZAxisText(TRANS("($1,$2,$3)").replace("$1", String(phaseAlignedModelTranslation.x))
                                               .replace("$2", String(phaseAlignedModelTranslation.y))
                                               .replace("$3", String(phaseAlignedModelTranslation.z + 5)));
        positionLabels[5] = TextureBuilder::createTexture(openGLContext,
                                                          TextureBuilder::createTextImage(maxZAxisText, Font(12)),
                                                          imageSize,
                                                          c4::Matrix4D(c4::Vector3D(0, imageSize, 5) + phaseAlignedModelTranslation));
    }
}

void XYZAxesComponent::renderOpenGL()
{
    if (!initialized)
    {
        newOpenGLContextCreated();
        initialized = true;
    }

    if (cameraMoved)
    {
        createAxesAndPositionLabelsForCameraPosition();
        cameraMoved = false;
    }

    OpenGLHelpers::clear(Colours::white);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LESS);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, getWidth(), getHeight());

    c4::Matrix4D viewMatrix(renderingProperties->getViewMatrix());
    c4::Matrix4D projectionMatrix(renderingProperties->getProjectionMatrix());

    xAxis->render(openGLContext, viewMatrix, projectionMatrix);
    yAxis->render(openGLContext, viewMatrix, projectionMatrix);
    zAxis->render(openGLContext, viewMatrix, projectionMatrix);

    int numberOfLabels = positionLabels.size();
    for (int indexOfLabel = 0; indexOfLabel < numberOfLabels; indexOfLabel++)
    {
        positionLabels[indexOfLabel]->render(openGLContext, viewMatrix, projectionMatrix);
    }
}

void XYZAxesComponent::openGLContextClosing()
{
    if (xAxis != nullptr)
    {
        xAxis->shutdown();
        xAxis = GraphicObject::Ptr();
    }

    if (yAxis != nullptr)
    {
        yAxis->shutdown();
        yAxis = GraphicObject::Ptr();
    }

    if (zAxis != nullptr)
    {
        zAxis->shutdown();
        zAxis = GraphicObject::Ptr();
    }

    positionLabels.clear();
}
