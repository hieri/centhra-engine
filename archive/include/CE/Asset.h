#ifndef CE_ASSET_H
#define CE_ASSET_H

//- Standard Library -
#include <map>
#include <string>

//- Centhra Engine -
#include <CE/BufferObject.h>
#include <CE/Image.h>
#include <CE/Matrix4x4.h>
#include <CE/Quaternion.h>
#include <CE/Shader.h>
#include <CE/Vector2.h>
#include <CE/Vector3.h>

namespace ce
{
	class o3dAsset;

	class Asset
	{
	public:
		class Instance;

		class Node
		{
		public:
			class Instance
			{
			public:
				Asset::Instance *m_rootAssetInstance;
				Node *m_node;
				Quaternion<float> m_rotationOffset;
				Matrix4x4<float> m_transformation, m_absoluteTransformation;
				Vector3<float> m_positionOffset, m_scaleOffset;

				Instance(Asset::Instance *rootAssetInstance, Node *node);
			};

			Asset *m_rootAsset;
			Quaternion<float> m_rotationOffset;
			Matrix4x4<float> m_transformation, m_absoluteTransformation, m_offset;
			Node *m_parent, **m_children;
			std::string m_name;
			unsigned int m_numChildren, m_numMeshes, *m_meshes, m_id;
			Vector3<float> m_positionOffset, m_scaleOffset;

			Node();
			~Node();

			void render(Asset::Instance *assetInstance);
		};

		class AnimationChannel
		{
		public:
			class Key
			{
			public:
				unsigned int m_positionKey, m_rotationKey, m_scalingKey;

				Key();
			};

			Node *m_targetNode;
			std::pair<float, Vector3<float> > *m_positionKeys, *m_scalingKeys;
			std::pair<float, Quaternion<float> > *m_rotationKeys;
			unsigned int m_numPositionKeys, m_numRotationKeys, m_numScalingKeys;

			AnimationChannel();
			~AnimationChannel();
		};

		class Animation
		{
		public:
			class Instance
			{
			public:
				Animation *m_animation;
				AnimationChannel::Key *m_channelKeys;
				Asset::Instance *m_rootAssetInstance;
				bool m_isRunning, m_loop;
				float m_time;
				std::map<Asset::Node *, Asset::Node::Instance *> m_nodeInstances;

				Instance(Asset::Instance *rootAssetInstance, Animation *animation);
				~Instance();

				bool isRunning() const;
				void reset();
				void start();
				void step(unsigned int timeMS);
				void stop();
			};

			Asset *m_rootAsset;
			AnimationChannel **m_channels;
			float m_duration, m_ticksPerSecond;
			std::string m_name;
			unsigned int m_numChannels;
			
			Animation();
			~Animation();
		};

		class Material
		{
		public:
			std::string m_textureURI;
			Image *m_textureData;

			Material();
		};

		class Mesh
		{
		public:
			class Instance
			{
			public:
				Asset::Instance *m_rootAssetInstance;
				Mesh *m_mesh;

				Instance(Asset::Instance *rootAssetInstance, Mesh *mesh);
			};

			Asset *m_rootAsset;
			BufferObject *m_vbo, *m_ebo;
			std::pair<float, Node *> ***m_vertexWeights;
			std::string m_name;
			unsigned char m_stride, m_normalOffset, m_texCoordOffset, m_vertexWeightOffset;
			unsigned short *m_indices;
			unsigned int m_numFaces, m_numVertices, m_materialIdx, *m_vertexWeightCounts;
			Vector2<float> *m_texCoords;
			Vector3<float> *m_normals, *m_vertices;

			Mesh();
			~Mesh();

			void render(Asset::Instance *assetInstance);
		};

		class Instance
		{
		public:
			Asset *m_asset;
			bool m_runAnimationThread;
			o3dAsset *m_object;
			std::map<Animation *, Animation::Instance *> m_animationInstances;
			std::map<Mesh *, Mesh::Instance *> m_meshInstances;
			Node::Instance **m_nodeInstances;
			void *m_animationThread;

			Instance(Asset *asset, o3dAsset *object);
			~Instance();

			o3dAsset *getObject() const;
			bool isRunningAnimation(const char *name) const;
			void playAnimation(const char *name, bool loop = false);
			void render();
			void stopAnimation(const char *name);
			void stopAllAnimations();
		};

		Animation **m_animations;
		std::map<std::string, Animation *> m_animationMap;
		std::map<std::string, Node *> m_nodeMap;
		Material **m_materials;
		Mesh **m_meshes;
		Node **m_nodes;
		unsigned int m_numAnimations, m_numMaterials, m_numMeshes, m_numNodes;

		Asset();

		static Asset *createFromFile(const char *file);
		static Asset *createFromMemory(const unsigned char *memory, unsigned int size);

		~Asset();
	};

	ShaderProgram *getDefaultSkinningProgram();
	ShaderProgram *getSkinningProgram();
	void setSkinningProgram(ShaderProgram *program);
}

#endif
