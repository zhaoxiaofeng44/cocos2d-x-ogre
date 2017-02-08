
/* Copyright 2013 Jonne Nauha / jonne@adminotech.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "OgreBridgeRenderSystemPlugin.h"

#include "OgreRoot.h"
#include "components/OgreBridgeRenderSystem.h"

namespace Ogre
{
    const String sPluginName = "Headless RenderSystem";

    BridgeRenderSystemPlugin::BridgeRenderSystemPlugin() : 
        mRenderSystem(0)
    {
    }

    const String& BridgeRenderSystemPlugin::getName() const
    {
        return sPluginName;
    }

    void BridgeRenderSystemPlugin::install()
    {
        LogManager::getSingleton().logMessage("HeadlessPlugin::install", Ogre::LML_TRIVIAL);
        
        mRenderSystem = new BridgeRenderSystem();
        Root::getSingleton().addRenderSystem(mRenderSystem);
    }

    void BridgeRenderSystemPlugin::uninstall()
    {
        if (mRenderSystem)
            delete mRenderSystem;
        mRenderSystem = 0;
    }

    void BridgeRenderSystemPlugin::initialise()
    {
    }

    void BridgeRenderSystemPlugin::shutdown()
    {
    }
}

#ifndef OGRE_STATIC_LIB

namespace Ogre
{
    static HeadlessPlugin *plugin;

    extern "C" void _OgreHeadlessExport dllStartPlugin(void) throw()
    {
        plugin = new HeadlessPlugin();
        Root::getSingleton().installPlugin(plugin);
    }

    extern "C" void _OgreHeadlessExport dllStopPlugin(void)
    {
        Root::getSingleton().uninstallPlugin(plugin);
        delete plugin;
    }
}

#endif
