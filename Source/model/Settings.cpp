#include "Settings.h"

#include "Util.h"

class Settings::DelegatingOpenGLRenderer : public OpenGLRenderer
{
public:
    void newOpenGLContextCreated()
    {
        renderers.call(&OpenGLRenderer::newOpenGLContextCreated);
    }

    void renderOpenGL()
    {
        jassert (OpenGLHelpers::isContextActive());

        MessageManagerLock mm (Thread::getCurrentThread());
        if (! mm.lockWasGained())
            return;

        renderers.call(&OpenGLRenderer::renderOpenGL);
    }

    void openGLContextClosing()
    {
        renderers.call(&OpenGLRenderer::openGLContextClosing);
    }

    ListenerList<OpenGLRenderer> renderers;
};

Settings::Settings()
  : threadPool(1)
  , configFile(getSettingsDirectory().getChildFile(JUCEApplication::getInstance()->getApplicationName().toLowerCase().replace(" ", "")), createConfigFileOptions())
  , openGLContext()
  , delegatingOpenGLRenderer(new DelegatingOpenGLRenderer())
{
    openGLContext.setRenderer(delegatingOpenGLRenderer);
}

Settings::~Settings()
{
    try
    {
        bool interruptRunningJobs = true;
        int timeOutMilliseconds = 0;
        threadPool.removeAllJobs(interruptRunningJobs, timeOutMilliseconds);
    } catch (...) {}
}

class Settings& Settings::getInstance()
{
    static Settings instance;
    return instance;
}

ThreadPool& Settings::getThreadPool()
{
    return threadPool;
}

OpenGLContext& Settings::getOpenGLContext()
{
    return openGLContext;
}

void Settings::addOpenGLRenderer(OpenGLRenderer* renderer)
{
    delegatingOpenGLRenderer->renderers.add(renderer);
}

void Settings::removeOpenGLRenderer(OpenGLRenderer* renderer)
{
    delegatingOpenGLRenderer->renderers.remove(renderer);
}

void Settings::addJob(ThreadPoolJob *job, bool deleteJobWhenFinished)
{
    threadPool.addJob(job, deleteJobWhenFinished);
}

File Settings::getSettingsDirectory()
{
    File settingsDirectory( File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile(JUCEApplication::getInstance()->getApplicationName()) );
    if (!settingsDirectory.exists()) {
        bool succeeded = settingsDirectory.createDirectory();
        if (!succeeded) {
            settingsDirectory = File::getSpecialLocation(File::userApplicationDataDirectory);
        }
    }
    return settingsDirectory;
}

PropertiesFile::Options Settings::createConfigFileOptions()
{
    PropertiesFile::Options options;
    options.applicationName = JUCEApplication::getInstance()->getApplicationName();
    options.filenameSuffix = "properties";
    options.commonToAllUsers = false;
    options.storageFormat = PropertiesFile::storeAsXML;
    return options;
}

void Settings::saveConfigToFile()
{
    configFile.saveIfNeeded();
}

bool Settings::hasProperty(const char* propertyName)
{
    return configFile.containsKey(propertyName);
}

void Settings::removeProperty(const char* propertyName)
{
    if (configFile.containsKey(propertyName))
    {
        configFile.removeValue(propertyName);

        saveConfigToFile();
    }
}

String Settings::readString(StringRef propertyName,
                                 const String& defaultValue)
{
    if (!configFile.containsKey(propertyName))
    {
        return defaultValue;
    }
    else
    {
        String propertyValue = configFile.getValue(propertyName, defaultValue);
        return propertyValue;
    }
}

void Settings::writeString(const String & propertyName, 
                           const String & value)
{
    configFile.setValue(propertyName, value);

    saveConfigToFile();
}

