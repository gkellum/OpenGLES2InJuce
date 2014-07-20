#ifndef __Settings_H__
#define __Settings_H__

#include "JuceHeader.h"

#include <string>
#include <memory>

class Settings
{
public:
    static Settings& getInstance();

    ThreadPool& getThreadPool();
    void addJob(ThreadPoolJob *job, bool deleteJobWhenFinished);

    OpenGLContext& getOpenGLContext();
    void addOpenGLRenderer(OpenGLRenderer* renderer);
    void removeOpenGLRenderer(OpenGLRenderer* renderer);

    bool hasProperty(const char* propertyName);
    void removeProperty(const char* propertyName);

    String readString(StringRef propertyName, 
                      const String& defaultValue = String::empty);
    void writeString(const String & propertyName, 
                     const String & value);

private:
    Settings();
    ~Settings();

    File getSettingsDirectory();
    PropertiesFile::Options createConfigFileOptions();
    void saveConfigToFile();

    ThreadPool threadPool;

    PropertiesFile configFile;

    String privateKeyForInstallation;
    String publicKeyForInstallation;

    OpenGLContext openGLContext;
    class DelegatingOpenGLRenderer;
    ScopedPointer<DelegatingOpenGLRenderer> delegatingOpenGLRenderer;
};

#endif
