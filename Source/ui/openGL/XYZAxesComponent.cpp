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

    translatedViewMatrix.n[0][0] += float(event.getDistanceFromDragStartX()) / getWidth();
    translatedViewMatrix.n[1][1] += -float(event.getDistanceFromDragStartY()) / getHeight();

    //std::cerr << "XYZAxesComponent::mouseDrag; new translation (" << translatedViewMatrix.n[12] << ", " << translatedViewMatrix.n[13] << ")" << std::endl;
    renderingProperties->setViewMatrix(translatedViewMatrix);

    cameraMoved = true;

    openGLContext.triggerRepaint();
}

void XYZAxesComponent::mouseMagnify(const MouseEvent &event, float scaleFactor)
{
    c4::Matrix4D translatedViewMatrix(renderingProperties->getViewMatrix());
    translatedViewMatrix.n[0][2] += scaleFactor - 1;
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

// bool operator==(const c4::Vector3D& lhs, const c4::Vector3D& rhs)
// {
//     return     fabs(lhs.x - rhs.x) < 0.01f
//             && fabs(lhs.y - rhs.y) < 0.01f
//             && fabs(lhs.z - rhs.z) < 0.01f;
// }

// bool operator!=(const c4::Vector3D& lhs, const c4::Vector3D& rhs)
// {
//     return !(lhs == rhs);
// }

void XYZAxesComponent::createAxesAndPositionLabelsForCameraPosition()
{
    c4::Matrix4D viewMatrix( renderingProperties->getViewMatrix() );
    c4::Vector4D origin( viewMatrix.getRow(3) );

    origin.x = roundf( origin.x / 5.0f ) * 5.0f;
    origin.y = roundf( origin.y / 5.0f ) * 5.0f;
    origin.z = roundf( origin.z / 5.0f ) * 5.0f;


    bool nearestXAxisChanged = xAxis == nullptr || xAxis->getBoundingBox().getCenter() != origin;
    if (nearestXAxisChanged)
    {
        xAxis = LineSegmentBuilder::createLineSegment(openGLContext,
                                                       std::tuple<float, float, float>(-10, 0, 0),
                                                       std::tuple<float, float, float>(10, 0, 0),
                                                       Colours::red,
                                                       c4::Matrix4D(c4::Vector3D(0, 0, 0)));
    }

    bool nearestYAxisChanged = yAxis == nullptr || yAxis->getBoundingBox().getCenter() != origin;
    if (nearestYAxisChanged)
    {
        yAxis = LineSegmentBuilder::createLineSegment(openGLContext,
                                                       std::tuple<float, float, float>(0, -10, 0),
                                                       std::tuple<float, float, float>(0, 10, 0),
                                                       Colours::green,
                                                       c4::Matrix4D(c4::Vector3D(0, 0, 0)));
    }

    bool nearestZAxisChanged = zAxis == nullptr || zAxis->getBoundingBox().getCenter() != origin;
    {
        zAxis = LineSegmentBuilder::createLineSegment(openGLContext,
                                                       std::tuple<float, float, float>(0, 0, -100),
                                                       std::tuple<float, float, float>(0, 0, 100),
                                                       Colours::blue,
                                                       c4::Matrix4D(c4::Vector3D(0, 0, 0)));
    }

    if (nearestXAxisChanged || nearestYAxisChanged || nearestZAxisChanged)
    {
        std::cerr << "xAxisChanged <" << nearestXAxisChanged << "> yAxisChanged <" << nearestYAxisChanged << "> zAxisChanged <" << nearestZAxisChanged << ">" << std::endl;
        std::cerr << "origin (" << origin.x << ", " << origin.y << ", " << origin.z << ")" << std::endl;

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

    positionLabels.resize(6, GraphicObject::Ptr());

    float imageSize = 0.5f;

    if (nearestXAxisChanged)
    {
        String minXAxisText(TRANS("($1,0,0)").replace("$1", String(origin.x - 5)));
        positionLabels[0] = TextureBuilder::createTexture(openGLContext,
                                                          TextureBuilder::createTextImage(minXAxisText, Font(12)),
                                                          imageSize,
                                                          c4::Matrix4D(c4::Vector3D(-5, imageSize, 0)));

        String maxXAxisText(TRANS("($1,0,0)").replace("$1", String(origin.x + 5)));
        positionLabels[1] = TextureBuilder::createTexture(openGLContext,
                                                          TextureBuilder::createTextImage(maxXAxisText, Font(12)),
                                                          imageSize,
                                                          c4::Matrix4D(c4::Vector3D(5, imageSize, 0)));
    }

    if (nearestYAxisChanged)
    {
        positionLabels[2] = TextureBuilder::createTexture(openGLContext,
                                                          TextureBuilder::createTextImage(TRANS("(0,-5,0)"), Font(12)),
                                                          imageSize,
                                                          c4::Matrix4D(c4::Vector3D(imageSize, -5, 0)));

        positionLabels[3] = TextureBuilder::createTexture(openGLContext,
                                                          TextureBuilder::createTextImage(TRANS("(0,5,0)"), Font(12)),
                                                          imageSize,
                                                          c4::Matrix4D(c4::Vector3D(imageSize, 5, 0)));
    }

    if (nearestZAxisChanged)
    {
        positionLabels[4] = TextureBuilder::createTexture(openGLContext,
                                                          TextureBuilder::createTextImage(TRANS("(0,0,-5)"), Font(12)),
                                                          imageSize,
                                                          c4::Matrix4D(c4::Vector3D(0, imageSize, -5)));

        positionLabels[5] = TextureBuilder::createTexture(openGLContext,
                                                          TextureBuilder::createTextImage(TRANS("(0,0,5)"), Font(12)),
                                                          imageSize,
                                                          c4::Matrix4D(c4::Vector3D(0, imageSize, 5)));
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
