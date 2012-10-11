#ifndef CE_O3DASSET_H
#define CE_O3DASSET_H

//- Centhra Engine -
#include <CE/Asset.h>
#include <CE/o3dObject.h>

namespace ce
{
	class o3dAsset : public o3dObject
	{
		Asset *m_asset;
		Asset::Instance *m_assetInstance;
		void *m_collisionShape, *m_motionState, *m_rigidBody;

	protected:
		virtual void draw();
		virtual void onSetPosition();
		virtual void onSetRotation();
		virtual void onSetScale();
		virtual void onSetVelocity();

	public:
		o3dAsset(o3dInstance *instance, Vector3<float> scale = Vector3<float>(1.f, 1.f, 1.f));
		~o3dAsset();

		Asset *getAsset() const;
		bool isRunningAnimation(const char *name) const;
		void playAnimation(const char *name, bool loop = false);
		void setAsset(Asset *asset);
		void stopAnimation(const char *name);
		void setDebugDraw(bool debugDraw);
		void setStatic(bool isStatic);
		virtual void update();
	};
}

#endif
