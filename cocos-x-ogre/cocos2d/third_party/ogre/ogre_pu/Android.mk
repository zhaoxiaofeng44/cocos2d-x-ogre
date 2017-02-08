LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := universe_static

LOCAL_MODULE_FILENAME := libuniverse

	
# ogre_pu
LOCAL_OGRE_PARTICLE_UNIVERSE_INCLUDE := \
	$(LOCAL_PATH)/include	\
	$(LOCAL_PATH)/include/Externs	\
	$(LOCAL_PATH)/include/ParticleAffectors \
	$(LOCAL_PATH)/include/ParticleBehaviours \
	$(LOCAL_PATH)/include/ParticleEmitters \
	$(LOCAL_PATH)/include/ParticleEventHandlers \
	$(LOCAL_PATH)/include/ParticleRenderers

LOCAL_OGRE_PARTICLE_UNIVERSE_SRC_FILES := \
	src/ParticleUniverseAffector.cpp  \
    src/ParticleUniverseAffectorTokens.cpp  \
    src/ParticleUniverseAliasTokens.cpp  \
    src/ParticleUniverseAtlasImage.cpp  \
    src/ParticleUniverseAttachable.cpp  \
    src/ParticleUniverseAttachableTokens.cpp  \
    src/ParticleUniverseBehaviourTokens.cpp  \
    src/ParticleUniverseCameraDependency.cpp  \
    src/ParticleUniverseCameraDependencyTokens.cpp  \
    src/ParticleUniverseDll.cpp  \
    src/ParticleUniverseDynamicAttribute.cpp  \
    src/ParticleUniverseDynamicAttributeTokens.cpp  \
    src/ParticleUniverseEmitter.cpp  \
    src/ParticleUniverseEmitterTokens.cpp  \
    src/ParticleUniverseEventHandlerTokens.cpp  \
    src/ParticleUniverseExtern.cpp  \
    src/ParticleUniverseExternTokens.cpp  \
    src/ParticleUniverseForceField.cpp  \
    src/ParticleUniverseNoise.cpp  \
    src/ParticleUniverseObserver.cpp  \
    src/ParticleUniverseObserverTokens.cpp  \
    src/ParticleUniverseParticle.cpp  \
    src/ParticleUniverseParticlePool.cpp  \
    src/ParticleUniversePCH.cpp  \
    src/ParticleUniversePlugin.cpp  \
    src/ParticleUniverseRenderer.cpp  \
    src/ParticleUniverseRendererTokens.cpp  \
    src/ParticleUniverseScriptDeserializer.cpp  \
    src/ParticleUniverseScriptSerializer.cpp  \
    src/ParticleUniverseSystem.cpp  \
    src/ParticleUniverseSystemManager.cpp  \
    src/ParticleUniverseSystemManagerTokens.cpp  \
    src/ParticleUniverseSystemTokens.cpp  \
    src/ParticleUniverseTechnique.cpp  \
    src/ParticleUniverseTechniqueTokens.cpp  \
    src/ParticleUniverseVisualParticle.cpp  \
    src/ParticleEmitters/ParticleUniverseBoxEmitter.cpp  \
    src/ParticleEmitters/ParticleUniverseBoxEmitterTokens.cpp  \
    src/ParticleEmitters/ParticleUniverseCircleEmitter.cpp  \
    src/ParticleEmitters/ParticleUniverseCircleEmitterTokens.cpp  \
    src/ParticleEmitters/ParticleUniverseLineEmitter.cpp  \
    src/ParticleEmitters/ParticleUniverseLineEmitterTokens.cpp  \
    src/ParticleEmitters/ParticleUniverseMeshSurfaceEmitter.cpp  \
    src/ParticleEmitters/ParticleUniverseMeshSurfaceEmitterTokens.cpp  \
    src/ParticleEmitters/ParticleUniversePointEmitter.cpp  \
    src/ParticleEmitters/ParticleUniversePointEmitterTokens.cpp  \
    src/ParticleEmitters/ParticleUniversePositionEmitter.cpp  \
    src/ParticleEmitters/ParticleUniversePositionEmitterTokens.cpp  \
    src/ParticleEmitters/ParticleUniverseSlaveEmitter.cpp  \
    src/ParticleEmitters/ParticleUniverseSlaveEmitterTokens.cpp  \
    src/ParticleEmitters/ParticleUniverseSphereSurfaceEmitter.cpp  \
    src/ParticleEmitters/ParticleUniverseSphereSurfaceEmitterTokens.cpp  \
    src/ParticleEmitters/ParticleUniverseVertexEmitter.cpp  \
    src/ParticleEmitters/ParticleUniverseVertexEmitterTokens.cpp  \
    src/ParticleRenderers/ParticleUniverseBeamRenderer.cpp  \
    src/ParticleRenderers/ParticleUniverseBeamRendererTokens.cpp  \
    src/ParticleRenderers/ParticleUniverseBillboardRenderer.cpp  \
    src/ParticleRenderers/ParticleUniverseBillboardRendererTokens.cpp  \
    src/ParticleRenderers/ParticleUniverseBox.cpp  \
    src/ParticleRenderers/ParticleUniverseBoxRenderer.cpp  \
    src/ParticleRenderers/ParticleUniverseBoxRendererTokens.cpp  \
    src/ParticleRenderers/ParticleUniverseBoxSet.cpp  \
    src/ParticleRenderers/ParticleUniverseEntityRenderer.cpp  \
    src/ParticleRenderers/ParticleUniverseEntityRendererTokens.cpp  \
    src/ParticleRenderers/ParticleUniverseLightRenderer.cpp  \
    src/ParticleRenderers/ParticleUniverseLightRendererTokens.cpp  \
    src/ParticleRenderers/ParticleUniversePrimitiveShapeSet.cpp  \
    src/ParticleRenderers/ParticleUniverseRibbonTrailRenderer.cpp  \
    src/ParticleRenderers/ParticleUniverseRibbonTrailRendererTokens.cpp  \
    src/ParticleRenderers/ParticleUniverseSphere.cpp  \
    src/ParticleRenderers/ParticleUniverseSphereRenderer.cpp  \
    src/ParticleRenderers/ParticleUniverseSphereRendererTokens.cpp  \
    src/ParticleRenderers/ParticleUniverseSphereSet.cpp  \
    src/ParticleAffectors/ParticleUniverseAlignAffector.cpp  \
    src/ParticleAffectors/ParticleUniverseAlignAffectorTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseBaseCollider.cpp  \
    src/ParticleAffectors/ParticleUniverseBaseColliderTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseBaseForceAffector.cpp  \
    src/ParticleAffectors/ParticleUniverseBaseForceAffectorTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseBoxCollider.cpp  \
    src/ParticleAffectors/ParticleUniverseBoxColliderTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseCollisionAvoidanceAffector.cpp  \
    src/ParticleAffectors/ParticleUniverseCollisionAvoidanceAffectorTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseColourAffector.cpp  \
    src/ParticleAffectors/ParticleUniverseColourAffectorTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseFlockCenteringAffector.cpp  \
    src/ParticleAffectors/ParticleUniverseFlockCenteringAffectorTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseForceFieldAffector.cpp  \
    src/ParticleAffectors/ParticleUniverseForceFieldAffectorTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseGeometryRotator.cpp  \
    src/ParticleAffectors/ParticleUniverseGeometryRotatorTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseGravityAffector.cpp  \
    src/ParticleAffectors/ParticleUniverseGravityAffectorTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseInterParticleCollider.cpp  \
    src/ParticleAffectors/ParticleUniverseInterParticleColliderTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseJetAffector.cpp  \
    src/ParticleAffectors/ParticleUniverseJetAffectorTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseLineAffector.cpp  \
    src/ParticleAffectors/ParticleUniverseLineAffectorTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseLinearForceAffector.cpp  \
    src/ParticleAffectors/ParticleUniverseLinearForceAffectorTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseParticleFollower.cpp  \
    src/ParticleAffectors/ParticleUniverseParticleFollowerTokens.cpp  \
    src/ParticleAffectors/ParticleUniversePathFollower.cpp  \
    src/ParticleAffectors/ParticleUniversePathFollowerTokens.cpp  \
    src/ParticleAffectors/ParticleUniversePlaneCollider.cpp  \
    src/ParticleAffectors/ParticleUniversePlaneColliderTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseRandomiser.cpp  \
    src/ParticleAffectors/ParticleUniverseRandomiserTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseScaleAffector.cpp  \
    src/ParticleAffectors/ParticleUniverseScaleAffectorTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseScaleVelocityAffector.cpp  \
    src/ParticleAffectors/ParticleUniverseScaleVelocityAffectorTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseSineForceAffector.cpp  \
    src/ParticleAffectors/ParticleUniverseSineForceAffectorTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseSphereCollider.cpp  \
    src/ParticleAffectors/ParticleUniverseSphereColliderTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseTextureAnimator.cpp  \
    src/ParticleAffectors/ParticleUniverseTextureAnimatorTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseTextureRotator.cpp  \
    src/ParticleAffectors/ParticleUniverseTextureRotatorTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseVelocityMatchingAffector.cpp  \
    src/ParticleAffectors/ParticleUniverseVelocityMatchingAffectorTokens.cpp  \
    src/ParticleAffectors/ParticleUniverseVortexAffector.cpp  \
    src/ParticleAffectors/ParticleUniverseVortexAffectorTokens.cpp  \
    src/ParticleObservers/ParticleUniverseOnClearObserver.cpp  \
    src/ParticleObservers/ParticleUniverseOnClearObserverTokens.cpp  \
    src/ParticleObservers/ParticleUniverseOnCollisionObserver.cpp  \
    src/ParticleObservers/ParticleUniverseOnCollisionObserverTokens.cpp  \
    src/ParticleObservers/ParticleUniverseOnCountObserver.cpp  \
    src/ParticleObservers/ParticleUniverseOnCountObserverTokens.cpp  \
    src/ParticleObservers/ParticleUniverseOnEmissionObserver.cpp  \
    src/ParticleObservers/ParticleUniverseOnEmissionObserverTokens.cpp  \
    src/ParticleObservers/ParticleUniverseOnEventFlagObserver.cpp  \
    src/ParticleObservers/ParticleUniverseOnEventFlagObserverTokens.cpp  \
    src/ParticleObservers/ParticleUniverseOnExpireObserver.cpp  \
    src/ParticleObservers/ParticleUniverseOnExpireObserverTokens.cpp  \
    src/ParticleObservers/ParticleUniverseOnPositionObserver.cpp  \
    src/ParticleObservers/ParticleUniverseOnPositionObserverTokens.cpp  \
    src/ParticleObservers/ParticleUniverseOnQuotaObserver.cpp  \
    src/ParticleObservers/ParticleUniverseOnQuotaObserverTokens.cpp  \
    src/ParticleObservers/ParticleUniverseOnRandomObserver.cpp  \
    src/ParticleObservers/ParticleUniverseOnRandomObserverTokens.cpp  \
    src/ParticleObservers/ParticleUniverseOnTimeObserver.cpp  \
    src/ParticleObservers/ParticleUniverseOnTimeObserverTokens.cpp  \
    src/ParticleObservers/ParticleUniverseOnVelocityObserver.cpp  \
    src/ParticleObservers/ParticleUniverseOnVelocityObserverTokens.cpp  \
    src/ParticleEventHandlers/ParticleUniverseDoAffectorEventHandler.cpp  \
    src/ParticleEventHandlers/ParticleUniverseDoAffectorEventHandlerTokens.cpp  \
    src/ParticleEventHandlers/ParticleUniverseDoEnableComponentEventHandler.cpp  \
    src/ParticleEventHandlers/ParticleUniverseDoEnableComponentEventHandlerTokens.cpp  \
    src/ParticleEventHandlers/ParticleUniverseDoExpireEventHandler.cpp  \
    src/ParticleEventHandlers/ParticleUniverseDoExpireEventHandlerTokens.cpp  \
    src/ParticleEventHandlers/ParticleUniverseDoFreezeEventHandler.cpp  \
    src/ParticleEventHandlers/ParticleUniverseDoFreezeEventHandlerTokens.cpp  \
    src/ParticleEventHandlers/ParticleUniverseDoPlacementParticleEventHandler.cpp  \
    src/ParticleEventHandlers/ParticleUniverseDoPlacementParticleEventHandlerTokens.cpp  \
    src/ParticleEventHandlers/ParticleUniverseDoScaleEventHandler.cpp  \
    src/ParticleEventHandlers/ParticleUniverseDoScaleEventHandlerTokens.cpp  \
    src/ParticleEventHandlers/ParticleUniverseDoStopSystemEventHandler.cpp  \
    src/ParticleEventHandlers/ParticleUniverseDoStopSystemEventHandlerTokens.cpp  \
    src/Externs/ParticleUniverseBoxColliderExtern.cpp  \
    src/Externs/ParticleUniverseBoxColliderExternTokens.cpp  \
    src/Externs/ParticleUniverseGravityExtern.cpp  \
    src/Externs/ParticleUniverseGravityExternTokens.cpp  \
    src/Externs/ParticleUniversePhysXActorExtern.cpp  \
    src/Externs/ParticleUniversePhysXActorExternTokens.cpp  \
    src/Externs/ParticleUniversePhysXBridge.cpp  \
    src/Externs/ParticleUniversePhysXFluidExtern.cpp  \
    src/Externs/ParticleUniversePhysXFluidExternTokens.cpp  \
    src/Externs/ParticleUniverseSceneDecoratorExtern.cpp  \
    src/Externs/ParticleUniverseSceneDecoratorExternTokens.cpp  \
    src/Externs/ParticleUniverseSphereColliderExtern.cpp  \
    src/Externs/ParticleUniverseSphereColliderExternTokens.cpp  \
    src/Externs/ParticleUniverseVortexExtern.cpp  \
    src/Externs/ParticleUniverseVortexExternTokens.cpp  \
    src/ParticleBehaviours/ParticleUniverseSlaveBehaviour.cpp  \
    src/ParticleBehaviours/ParticleUniverseSlaveBehaviourTokens.cpp
	

LOCAL_EXPORT_C_INCLUDES :=
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/include 
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../include 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include
LOCAL_C_INCLUDES += $(LOCAL_OGRE_PARTICLE_UNIVERSE_INCLUDE)

LOCAL_SRC_FILES := 
LOCAL_SRC_FILES += $(LOCAL_OGRE_PARTICLE_UNIVERSE_SRC_FILES)    
					
					
LOCAL_WHOLE_STATIC_LIBRARIES := ogremain_static 

include $(BUILD_STATIC_LIBRARY)
$(call import-module,../third_party/ogre/ogre_main)