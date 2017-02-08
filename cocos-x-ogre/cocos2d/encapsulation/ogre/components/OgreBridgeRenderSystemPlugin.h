#ifndef OGRE_BRIDGE_RENDERSYSTEM_Plugin_H
#define OGRE_BRIDGE_RENDERSYSTEM_Plugin_H


#include "OgrePlugin.h"

namespace Ogre
{
	class BridgeRenderSystem;
}

namespace Ogre
{
    class BridgeRenderSystemPlugin : public Plugin
    {

    public:
        BridgeRenderSystemPlugin();

        const String& getName() const;
        
        void install();
        void uninstall();

        void initialise();
        void shutdown();
        
    private:
        BridgeRenderSystem *mRenderSystem;
    };
}
#endif //OGRE_BRIDGE_RENDERSYSTEM_Plugin_H