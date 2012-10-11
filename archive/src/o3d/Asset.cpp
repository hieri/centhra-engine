//- Standard Library -
#include <vector>

//- SDL2 -
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_thread.h>

//- Assimp -
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

//- Centhra Engine -
#include <CE/Asset.h>
#include <CE/Base.h>
#include <CE/Game.h>
#include <CE/o3dAsset.h>
#include <CE/o3dInstance.h>
#include <CE/Shader.h>

using namespace std;
using namespace Assimp;

// TODO: Write working averaging algorithms for the rotations and scaling.

namespace ce
{
	ShaderProgram *g_defaultSkinningProgram = 0;
	ShaderProgram *g_skinningProgram = 0;
	Shader *g_defaultSkinningFragmentShader = 0;
	Shader *g_defaultSkinningVertexShader = 0;

	ShaderProgram *getDefaultSkinningProgram()
	{
		return g_defaultSkinningProgram;
	}
	ShaderProgram *getSkinningProgram()
	{
		if(!g_skinningProgram)
		{
			if(!g_defaultSkinningProgram)
			{
				unsigned char resDefaultSkinningFragmentShader[] = {
	 				#include "shaders/skinning_fragment.glsl.h"
				};

				unsigned char resDefaultSkinningVertexShader[] = {
	 				#include "shaders/skinning_vertex.glsl.h"
				};

				g_defaultSkinningFragmentShader = Shader::createFromMemory(resDefaultSkinningFragmentShader, sizeof(resDefaultSkinningFragmentShader), GL_FRAGMENT_SHADER_ARB);
				g_defaultSkinningVertexShader = Shader::createFromMemory(resDefaultSkinningVertexShader, sizeof(resDefaultSkinningVertexShader), GL_VERTEX_SHADER_ARB);

				g_defaultSkinningProgram = new ShaderProgram;
				g_defaultSkinningProgram->attachShader(g_defaultSkinningFragmentShader);
				g_defaultSkinningProgram->attachShader(g_defaultSkinningVertexShader);
				g_defaultSkinningProgram->link();
			}
			g_skinningProgram = g_defaultSkinningProgram;
		}
		return g_skinningProgram;
	}
	void setSkinningProgram(ShaderProgram *program)
	{
		g_skinningProgram = program;
	}

	aiLogStream g_logStream;
	bool g_logInit = false;
	bool g_logStatus = false;
	void assimpSetLoggerStatus(bool status)
	{
		if(status)
		{
			if(!g_logStatus)
			{
				if(!g_logInit)
				{
					g_logStream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, 0);
					g_logInit = true;
				}
				aiAttachLogStream(&g_logStream);
			}
		}
		else
			aiDetachLogStream(&g_logStream);
		g_logStatus = status;
	}

	unsigned int g_numNodes = 0;
	Asset::Node *readNode(aiScene *scene, Asset::Node *parent, Asset *rootAsset, aiNode *aiN)
	{
		Asset::Node *node = new Asset::Node();
		node->m_name = string(aiN->mName.data);
		node->m_rootAsset = rootAsset;
		node->m_parent = parent;
		node->m_id = g_numNodes++;
		rootAsset->m_nodeMap[node->m_name] = node;

		aiVector3D pos;
		aiQuaternion rot;
		aiMatrix4x4 m = aiN->mTransformation;
		m.DecomposeNoScaling(rot, pos);

		node->m_transformation = Matrix4x4<float>((float *)m[0]);
		node->m_absoluteTransformation = parent ? parent->m_absoluteTransformation * node->m_transformation : node->m_transformation;

		node->m_positionOffset = Vector3<float>(pos.x, pos.y, pos.z);
		node->m_rotationOffset = Quaternion<float>(rot.w, rot.x, rot.y, rot.z);

		unsigned int numMeshes = aiN->mNumMeshes;
		node->m_numMeshes = numMeshes;
		node->m_meshes = new unsigned int[numMeshes];
		for(unsigned int a = 0; a < numMeshes; a++)
			node->m_meshes[a] = aiN->mMeshes[a];

		unsigned int numChildren = aiN->mNumChildren;
		node->m_numChildren = numChildren;
		node->m_children = new Asset::Node *[numChildren];
		for(unsigned int a = 0; a < numChildren; a++)
			node->m_children[a] = readNode(scene, node, rootAsset, aiN->mChildren[a]);

		return node;
	}
	void storeNode(Asset::Node *node, Asset *rootAsset)
	{
		rootAsset->m_nodes[node->m_id] = node;
		for(unsigned int a = 0; a < node->m_numChildren; a++)
			storeNode(node->m_children[a], rootAsset);
	}

	unsigned int defaultFlags =
		aiProcess_CalcTangentSpace         | // calculate tangents and bitangents if possible
		aiProcess_JoinIdenticalVertices    | // join identical vertices/ optimize indexing
		aiProcess_ValidateDataStructure    | // perform a full validation of the loader's output
		aiProcess_ImproveCacheLocality     | // improve the cache locality of the output vertices
		aiProcess_RemoveRedundantMaterials | // remove redundant materials
		aiProcess_FindDegenerates          | // remove degenerated polygons from the import
		aiProcess_FindInvalidData          | // detect invalid model data, such as invalid normal vectors
		aiProcess_GenUVCoords              | // convert spherical, cylindrical, box and planar mapping to proper UVs
		aiProcess_TransformUVCoords        | // preprocess UV transformations (scaling, translation ...)
		aiProcess_FindInstances            | // search for instanced meshes and remove them by references to one master
		aiProcess_LimitBoneWeights         | // limit bone weights to 4 per vertex
		aiProcess_OptimizeMeshes		   | // join small meshes, if possible;
		aiProcess_GenSmoothNormals		   | // generate smooth normal vectors if not existing
		aiProcess_SplitLargeMeshes         | // split large, unrenderable meshes into submeshes
		aiProcess_Triangulate			   | // triangulate polygons with more than 3 edges
	//	aiProcess_ConvertToLeftHanded	   | // convert everything to D3D left handed space
		aiProcess_FlipUVs                  | // apparently this needs to be done
		aiProcess_SortByPType              | // make 'clean' meshes which consist of a single typ of primitives
	0;

	Asset::AnimationChannel::AnimationChannel()
	{
		m_numPositionKeys = 0;
		m_numRotationKeys = 0;
		m_numScalingKeys = 0;
		m_positionKeys = 0;
		m_rotationKeys = 0;
		m_scalingKeys = 0;
		m_targetNode = 0;
	}
	Asset::AnimationChannel::~AnimationChannel()
	{
		delete [] m_positionKeys;
		delete [] m_rotationKeys;
		delete [] m_scalingKeys;
	}
	Asset::AnimationChannel::Key::Key()
	{
		m_positionKey = 0;
		m_rotationKey = 0;
		m_scalingKey = 0;
	}

	Asset::Animation::Animation()
	{
		m_channels = 0;
		m_duration = 0.f;
		m_numChannels = 0;
		m_rootAsset = 0;
		m_ticksPerSecond = 0.f;
	}
	Asset::Animation::~Animation()
	{
		for(unsigned int a = 0; a < m_numChannels; a++)
			delete m_channels[a];
		delete [] m_channels;
	}
	Asset::Animation::Instance::Instance(Asset::Instance *rootAssetInstance, Animation *animation)
	{
		m_animation = animation;
		m_loop = false;
		m_isRunning = false;
		m_rootAssetInstance = rootAssetInstance;
		m_time = 0.f;
	}
	Asset::Animation::Instance::~Instance()
	{
		if(m_isRunning)
			stop();
		for(map<Node *, Node::Instance *>::iterator it = m_nodeInstances.begin(); it != m_nodeInstances.end(); it++)
			delete it->second;
	}
	bool Asset::Animation::Instance::isRunning() const
	{
		return m_isRunning;
	}
	void Asset::Animation::Instance::reset()
	{
		m_time = 0.f;
		for(unsigned int a = 0; a < m_animation->m_numChannels; a++)
			m_channelKeys[a] = Asset::AnimationChannel::Key();
	}
	void Asset::Animation::Instance::start()
	{
		if(m_isRunning)
			reset();
		else
		{
			unsigned int numChannels = m_animation->m_numChannels;
			m_channelKeys = new Asset::AnimationChannel::Key[numChannels];
			for(unsigned int a = 0; a < numChannels; a++)
			{
				Asset::AnimationChannel *channel = m_animation->m_channels[a];
				Asset::Node *node = channel->m_targetNode;
				Asset::Node::Instance *nodeInstance = new Asset::Node::Instance(m_rootAssetInstance, node);
				m_nodeInstances[node] = nodeInstance;
			}

			m_isRunning = true;
		}
	}
	void Asset::Animation::Instance::step(unsigned int timeMS)
	{
		o3dInstance *instance = m_rootAssetInstance->m_object->getO3DInstance();
		float timeStep = ((float)timeMS) / 1000.f * instance->getTimeScale();
		m_time += timeStep * m_animation->m_ticksPerSecond;

		bool calculate = true;
		unsigned int numChannels = m_animation->m_numChannels;
		if(m_time >= m_animation->m_duration)
		{
			if(m_loop)
			{
				m_time -= m_animation->m_duration;
				for(unsigned int a = 0; a < numChannels; a++)
					m_channelKeys[a] = Asset::AnimationChannel::Key();
			}
			else
				calculate = false;
		}
		if(calculate)
			for(unsigned int a = 0; a < numChannels; a++)
			{
				Asset::AnimationChannel *channel = m_animation->m_channels[a];
				Asset::Node *targetNode = channel->m_targetNode;
				Asset::Node::Instance *nodeInstance = m_nodeInstances[targetNode];
				Asset::AnimationChannel::Key &key = m_channelKeys[a];

				Vector3<float> position;
				if(channel->m_numPositionKeys > 1)
				{
					while(channel->m_positionKeys[key.m_positionKey].first < m_time && key.m_positionKey < (channel->m_numPositionKeys - 1))
						key.m_positionKey++;
					float &timeA = channel->m_positionKeys[key.m_positionKey - 1].first;
					float &timeB = channel->m_positionKeys[key.m_positionKey].first;
					float journey = (m_time - timeA) / (timeB - timeA);
					Vector3<float> &posA = channel->m_positionKeys[key.m_positionKey - 1].second;
					Vector3<float> &posB = channel->m_positionKeys[key.m_positionKey].second;
					position = posA + (posB - posA) * journey;
				}
				else
					position = channel->m_positionKeys[0].second;

				Quaternion<float> rotation;
				if(channel->m_numRotationKeys > 1)
				{
					while(channel->m_rotationKeys[key.m_rotationKey].first < m_time && key.m_rotationKey < (channel->m_numRotationKeys - 1))
						key.m_rotationKey++;
					float &timeA = channel->m_rotationKeys[key.m_rotationKey - 1].first;
					float &timeB = channel->m_rotationKeys[key.m_rotationKey].first;
					float journey = (m_time - timeA) / (timeB - timeA);
					Quaternion<float> &rotA = channel->m_rotationKeys[key.m_rotationKey - 1].second;
					Quaternion<float> &rotB = channel->m_rotationKeys[key.m_rotationKey].second;
					rotation = Quaternion<float>::interpolate(rotA, rotB, journey);
				}
				else
					rotation = channel->m_rotationKeys[0].second;

				Vector3<float> scale;
				if(channel->m_numScalingKeys > 1)
				{
					while(channel->m_scalingKeys[key.m_scalingKey].first < m_time && key.m_scalingKey < (channel->m_numScalingKeys - 1))
						key.m_positionKey++;
					float &timeA = channel->m_scalingKeys[key.m_scalingKey - 1].first;
					float &timeB = channel->m_scalingKeys[key.m_scalingKey].first;
					float journey = (m_time - timeA) / (timeB - timeA);
					Vector3<float> &scaleA = channel->m_scalingKeys[key.m_scalingKey - 1].second;
					Vector3<float> &scaleB = channel->m_scalingKeys[key.m_scalingKey].second;
					scale = scaleA + (scaleB - scaleA) * journey;
				}
				else
					scale = channel->m_scalingKeys[0].second;

				nodeInstance->m_positionOffset = position;
				nodeInstance->m_rotationOffset = rotation;
				nodeInstance->m_scaleOffset = scale;
			}
		else
			stop();
	}
	void Asset::Animation::Instance::stop()
	{
		if(m_isRunning)
		{
			m_isRunning = false;

			for(unsigned int a = 0; a < m_animation->m_numChannels; a++)
			{
				Asset::AnimationChannel *channel = m_animation->m_channels[a];
				Asset::Node *node = channel->m_targetNode;

				delete m_nodeInstances[node];
				m_nodeInstances.erase(node);
			}
			delete [] m_channelKeys;
			m_channelKeys = 0;
		}
	}

	Asset::Material::Material()
	{
		m_textureData = 0;
	}

	Asset::Mesh::Mesh()
	{
		m_vbo = 0;
		m_ebo = 0;
		m_indices = 0;
		m_materialIdx = 0;
		m_normals = 0;
		m_numFaces = 0;
		m_numVertices = 0;
		m_rootAsset = 0;
		m_stride = 0;
		m_vertexWeightOffset = 0;
		m_normalOffset = 0;
		m_texCoordOffset = 0;
		m_texCoords = 0;
		m_vertexWeightCounts = 0;
		m_vertexWeights = 0;
		m_vertices = 0;
	}
	Asset::Mesh::~Mesh()
	{
		delete [] m_indices;
		delete [] m_vertices;
		delete [] m_normals;
		if(m_vertexWeights)
			for(unsigned int a = 0; a < m_numVertices; a++)
				delete [] m_vertexWeights[a];
		delete [] m_vertexWeights;
		delete [] m_vertexWeightCounts;
		delete m_vbo;
		delete m_ebo;
	}
	void Asset::Mesh::render(Asset::Instance *assetInstance)
	{
		ShaderProgram *skinningProgram = getSkinningProgram();
		int weightLoc = skinningProgram->getAttribLocation("weight");
		int indexLoc = skinningProgram->getAttribLocation("index");
		int numBonesLoc = skinningProgram->getAttribLocation("numBones");

		//- Hardware Rendering -
		if(!m_vbo)
		{
			m_stride = 3;

			if(m_normals)
			{
				m_normalOffset = m_stride * 4;
				m_stride += 3;
			}
			if(m_texCoords)
			{
				m_texCoordOffset = m_stride * 4;
				m_stride += 2;
			}
			if(m_vertexWeights)
			{
				m_vertexWeightOffset = m_stride * 4;
				m_stride += 8;
			}

			unsigned int size = m_numVertices * m_stride;
			char *vbo = new char[size * 4];

			for(unsigned int a = 0; a < m_numVertices; a++)
			{
				float *ptr = (float *)&vbo[a * m_stride * 4];

				unsigned int b = 0;
				
				ptr[b++] = m_vertices[a][0];
				ptr[b++] = m_vertices[a][1];
				ptr[b++] = m_vertices[a][2];

				if(m_normals)
				{
					ptr[b++] = m_normals[a][0];
					ptr[b++] = m_normals[a][1];
					ptr[b++] = m_normals[a][2];
				}
				
				if(m_texCoords)
				{
					ptr[b++] = m_texCoords[a][0];
					ptr[b++] = m_texCoords[a][1];
				}

				if(m_vertexWeights)
				{
					char *cptr = (char *)&ptr[b];
					unsigned int count = m_vertexWeightCounts[a];
					for(unsigned int c = 0; c < 6; c++)
						if(c < count)
						{
							cptr[24 + c] = m_vertexWeights[a][c]->second->m_id;
							ptr[b + c] = m_vertexWeights[a][c]->first;
						}
						else
						{
							cptr[24 + c] = -1;
							ptr[b + c] = 0.0f;
						}
					cptr[30] = (char)count;
					cptr[31] = 0;
				}
			}

			m_stride *= 4;
			m_vbo = new BufferObject(BufferObject::Array);
			m_vbo->setData(vbo, size * 4, BufferObject::StaticDraw);
			delete [] vbo;
		}
		if(!m_ebo)
		{
			m_ebo = new BufferObject(BufferObject::ElementArray);
			m_ebo->setData(m_indices, m_numFaces * 3 * 2, BufferObject::StaticDraw);
		}

		m_vbo->bind();
		m_ebo->bind();
		
		Image *textureData = m_rootAsset->m_materials[m_materialIdx]->m_textureData;
		if(textureData)
		{
			glEnable(GL_TEXTURE_2D);
			textureData->bind();
		}

		glVertexPointer(3, GL_FLOAT, m_stride, 0);
		glEnableClientState(GL_VERTEX_ARRAY);

		if(m_normals)
		{
			glNormalPointer(GL_FLOAT, m_stride, (GLvoid *)m_normalOffset);
			glEnableClientState(GL_NORMAL_ARRAY);
		}

		if(m_texCoords)
		{
			glTexCoordPointer(2, GL_FLOAT, m_stride, (GLvoid *)(int)m_texCoordOffset);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		skinningProgram->use();

		float *mat = new float[16 * m_rootAsset->m_numNodes];
		for(unsigned int a = 0; a < m_rootAsset->m_numNodes; a++)
		{
			Node *nd = m_rootAsset->m_nodes[a];
			float *m = &mat[16 * a];

			Matrix4x4<float> mm = assetInstance->m_nodeInstances[a]->m_absoluteTransformation * nd->m_offset;
			float *n = &mm[0];

			for(unsigned int b = 0; b < 16; b++)
				m[b] = n[b];
		}

		int boneMatricesLoc = skinningProgram->getUniformLocation("boneMatrices");
		ShaderProgram::uniformMatrix4fv(boneMatricesLoc, m_rootAsset->m_numNodes, GL_TRUE, mat);
		delete [] mat;

		ShaderProgram::vertexAttribPointer(weightLoc, 4, GL_FLOAT, 0, m_stride, (GLvoid *)m_vertexWeightOffset);
		ShaderProgram::vertexAttribPointer(indexLoc, 4, GL_BYTE, 0, m_stride, (GLvoid *)(m_vertexWeightOffset + 24));
		ShaderProgram::vertexAttribPointer(numBonesLoc, 1, GL_BYTE, 0, m_stride, (GLvoid *)(m_vertexWeightOffset + 30));

		ShaderProgram::enableVertexAttribArray(weightLoc);
		ShaderProgram::enableVertexAttribArray(indexLoc);
		ShaderProgram::enableVertexAttribArray(numBonesLoc);


		glDrawElements(GL_TRIANGLES, m_numFaces * 3, GL_UNSIGNED_SHORT, 0);

	//	for(unsigned int a = 0; a < m_numFaces; a++)
	//		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort *)0+(a * 3));

		ShaderProgram::disableVertexAttribArray(weightLoc);
		ShaderProgram::disableVertexAttribArray(indexLoc);
		ShaderProgram::disableVertexAttribArray(numBonesLoc);

		ShaderProgram::unuse();

		glDisableClientState(GL_VERTEX_ARRAY);
		if(m_normals)
			glDisableClientState(GL_NORMAL_ARRAY);
		if(m_texCoords)
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		if(textureData)
			glDisable(GL_TEXTURE_2D);

		//- Software Rendering -
/*
//		glEnable(GL_LIGHTING);
//		getSkinningProgram()->use();

		glEnable(GL_TEXTURE_2D);
		Image *textureData = m_rootAsset->m_materials[m_materialIdx]->m_textureData;
		if(textureData)
			textureData->bind();

		for(unsigned int a = 0; a < m_numFaces; a++)
		{
//			glBegin(GL_LINE_LOOP);
			glBegin(GL_TRIANGLES);
			for(unsigned int b = 0; b < 3; b++)
			{
				unsigned short i = m_indices[a * 3 + b];
				Vector3<float> p = m_vertices[i];
				Vector3<float> n;
				Vector2<float> t;
				if(m_normals)
					n = m_normals[i];
				if(m_texCoords)
					t = m_texCoords[i];

				Vector3<float> pos;
				unsigned int vc = m_vertexWeightCounts[i];
				if(vc)
					for(unsigned int c = 0; c < vc; c++)
					{
						Matrix4x4<float> tr = Matrix4x4<float>::buildFromTranslation(Vector3<float>(p[0], p[1], p[2]));
						pair<float, Node *> *vw = m_vertexWeights[i][c];
						pos += (assetInstance->m_nodeInstances[vw->second->m_id]->m_absoluteTransformation * vw->second->m_offset * tr).getTranslation() * vw->first;
	//					pos += (vw->second->m_absoluteTransformation * vw->second->m_offset * tr).getTranslation() * vw->first;
					}
				else
					pos = p;

				glColor4f(1.f, 1.f, 1.f, 1.f);
//				if(m_vertexWeights[i][0]->second->m_id == 14)
//					glColor4f(0.1f, 0.1f, 0.1f, 1.f);

				if(m_texCoords)
					glTexCoord2f(t[0], t[1]);
				if(m_normals)
					glNormal3f(n[0], n[1], n[2]);
				glVertex3f(pos[0], pos[1], pos[2]);
			}
			glEnd();
		}

		glBindTexture(GL_TEXTURE_2D,0);
		glDisable(GL_TEXTURE_2D);

//		ShaderProgram::unuse();
//		glDisable(GL_LIGHTING);
//		glColor4f(1.0f,1.0f,1.0f,1.0f);

		*/
	}
	Asset::Mesh::Instance::Instance(Asset::Instance *rootAssetInstance, Mesh *mesh)
	{
		m_mesh = mesh;
		m_rootAssetInstance = rootAssetInstance;
	}

	Asset::Node::Node()
	{
		m_children = 0;
		m_id = 0;
		m_meshes = 0;
		m_numChildren = 0;
		m_numMeshes = 0;
		m_parent = 0;
		m_rootAsset = 0;
	}
	void Asset::Node::render(Asset::Instance *assetInstance)
	{

if(true)
{
		if(true)
		{
		// TODO: FIX THIS CUZ CRASH OMG
			Matrix4x4<float> mat(assetInstance->m_nodeInstances[m_id]->m_transformation);
			mat.transpose();
			glMultMatrixf(&mat[0]);

			for(unsigned int a = 0; a < m_numMeshes; a++)
				m_rootAsset->m_meshes[m_meshes[a]]->render(assetInstance);
		}
		else
		{
//			Vector3<float> posA = m_absoluteTransformation.getTranslation();
			Vector3<float> posA = assetInstance->m_nodeInstances[m_id]->m_absoluteTransformation.getTranslation();
			for(unsigned int a = 0; a < m_numChildren; a++)
			{
//				Vector3<float> posB = m_children[a]->m_absoluteTransformation.getTranslation();
				Vector3<float> posB = assetInstance->m_nodeInstances[m_children[a]->m_id]->m_absoluteTransformation.getTranslation();
				glBegin(GL_LINES);
					glVertex3f(posA[0], posA[1], posA[2]);
					glVertex3f(posB[0], posB[1], posB[2]);
				glEnd();
			}
		}
}

		for(unsigned int a = 0; a < m_numChildren; a++)
		{
			glPushMatrix();
				m_children[a]->render(assetInstance);
			glPopMatrix();
		}
	}
	Asset::Node::Instance::Instance(Asset::Instance *rootAssetInstance, Node *node)
	{
		m_node = node;
		m_rootAssetInstance = rootAssetInstance;

		m_rotationOffset = node->m_rotationOffset;
		m_transformation = node->m_transformation;
		m_absoluteTransformation = node->m_absoluteTransformation;
		m_positionOffset = node->m_positionOffset;
		m_scaleOffset = node->m_scaleOffset;
	}

	Asset *Asset::createFromFile(const char *file)
	{
		if(!fileExists(file))
		{
			setError("Asset::createFromFile: File does not exist <%s>.", file);
			return 0;
		}

		Importer importer;
		aiScene *scene = (aiScene *)importer.ReadFile(file, defaultFlags);

		if(!scene)
			return 0;

		Asset *asset = new Asset();

		if(scene->HasMeshes())
		{
			unsigned int numMeshes = scene->mNumMeshes;
			asset->m_numMeshes = numMeshes;
			asset->m_meshes = new Mesh *[numMeshes];

			for(unsigned int a = 0; a < numMeshes; a++)
			{
				aiMesh *meshData = scene->mMeshes[a];
				Mesh *m = new Mesh();
				asset->m_meshes[a] = m;
				m->m_rootAsset = asset;
				m->m_name = string(meshData->mName.data);
				m->m_materialIdx = meshData->mMaterialIndex;

				unsigned int numVertices = meshData->mNumVertices;
				m->m_numVertices = numVertices;
				m->m_vertices = new Vector3<float>[numVertices];
				m->m_vertexWeights = new pair<float, Node *> **[numVertices];
				m->m_vertexWeightCounts = new unsigned int[numVertices];

				for(unsigned int b = 0; b < numVertices; b++)
				{
					float x, y, z;
					x = meshData->mVertices[b].x;
					y = meshData->mVertices[b].y;
					z = meshData->mVertices[b].z;
					m->m_vertices[b] = Vector3<float>(x, y, z);
					m->m_vertexWeightCounts[b] = 0;
				}

				if(meshData->mNormals)
				{
					m->m_normals = new Vector3<float>[numVertices];
					for(unsigned int b = 0; b < numVertices; b++)
					{
						float x, y, z;
						x = meshData->mNormals[b].x;
						y = meshData->mNormals[b].y;
						z = meshData->mNormals[b].z;
						m->m_normals[b] = Vector3<float>(x, y, z);
					}
				}

				for(unsigned int b = 0; b < 4; b++)
					if(meshData->mNumUVComponents[b] == 2)
					{
						m->m_texCoords = new Vector2<float>[numVertices];
						for(unsigned int c = 0; c < numVertices; c++)
						{
							float x, y;
							x = meshData->mTextureCoords[b][c].x;
							y = meshData->mTextureCoords[b][c].y;
							m->m_texCoords[c] = Vector2<float>(x, y);
						}
					}

				unsigned int numFaces = meshData->mNumFaces;
				m->m_numFaces = numFaces;
				m->m_indices = new unsigned short[numFaces * 3];
				for(unsigned int b = 0; b < numFaces; b++)
					for(unsigned int c = 0; c < 3; c++)
						m->m_indices[(b * 3) + c] = (unsigned short)meshData->mFaces[b].mIndices[c];
			}
		}

		if(scene->HasMaterials())
		{
			unsigned int numMaterials = scene->mNumMaterials;
			asset->m_numMaterials = numMaterials;
			asset->m_materials = new Material *[numMaterials];
			for(unsigned int a = 0; a < numMaterials; a++)
			{
				aiMaterial *mat = scene->mMaterials[a];
				
				aiString taco;
				aiGetMaterialString(mat, AI_MATKEY_NAME, &taco);

				Material *m = new Material();
				asset->m_materials[a] = m;

				for(unsigned int b = 0; b < mat->mNumProperties; b++)
				{
					aiMaterialProperty *prop = mat->mProperties[b];
					if(!strcmp(prop->mKey.data, "$tex.file"))
					{
						aiString textureURI;
						for(unsigned int c = 0; c < AI_TEXTURE_TYPE_MAX; c++)
							if(AI_SUCCESS == aiGetMaterialTexture(mat, (aiTextureType)c, prop->mIndex, &textureURI))
							{
								string s = filePath(file);
								s.append(string(textureURI.data));
								s = fileCompressPath(s.c_str());
								m->m_textureURI = s;
								m->m_textureData = Image::createFromFile(s.c_str());
							}
					}
				}
			}
		}

		g_numNodes = 0;
		Node *root = readNode(scene, 0, asset, scene->mRootNode);
		asset->m_nodes = new Node *[g_numNodes];
		asset->m_numNodes = g_numNodes;
		storeNode(root, asset);

		if(scene->HasMeshes())
		{
			unsigned int numMeshes = scene->mNumMeshes;
			for(unsigned int a = 0; a < numMeshes; a++)
			{
				aiMesh *meshData = scene->mMeshes[a];

				if(meshData->HasBones())
				{
					Mesh *m = asset->m_meshes[a];
					vector<pair<float, Node *> *> *vertexWeights = new vector<pair<float, Node *> *>[m->m_numVertices];
					multimap<Node *, pair<float, Node *> *> vertexWeightMap;

					unsigned int numBones = meshData->mNumBones;
					for(unsigned int c = 0; c < numBones; c++)
					{
						aiBone *bone = meshData->mBones[c];
						Node *node = asset->m_nodeMap[string(bone->mName.data)];
					
						aiMatrix4x4 m = bone->mOffsetMatrix;
						node->m_offset = Matrix4x4<float>((float *)m[0]); //(m.a1,m.a2,m.a3,m.a4,m.b1,m.b2,m.b3,m.b4,m.c1,m.c2,m.c3,m.c4,m.d1,m.d2,m.d3,m.d4);
						for(unsigned int d = 0; d < bone->mNumWeights; d++)
						{
							aiVertexWeight vw = bone->mWeights[d];
							multimap<Node *, pair<float, Node *> *>::iterator it;
							pair<multimap<Node *, pair<float, Node *> *>::iterator, multimap<Node *, pair<float, Node *> *>::iterator> ret;
							pair<float, Node *> *VW = 0;
							ret = vertexWeightMap.equal_range(node);
							if(vertexWeightMap.count(node))
								for(it = ret.first; it != ret.second; it++)
									if(it->second->first == vw.mWeight)
									{
										VW = it->second;
										break;
									}
							if(!VW)
							{
								VW = new pair<float, Node *>();
								VW->first = vw.mWeight;
								VW->second = node;
								vertexWeightMap.insert(pair<Node *, pair<float, Node *> *>(node,VW));
							}
							
							vertexWeights[vw.mVertexId].insert(vertexWeights[vw.mVertexId].end(), VW);
						}
					}

					for(unsigned int b = 0; b < m->m_numVertices; b++)
					{
						unsigned int count = vertexWeights[b].size();
						m->m_vertexWeightCounts[b] = count;
						if(count)
						{
							m->m_vertexWeights[b] = new pair<float, Node *> *[count];
							unsigned int c = 0;
							for(vector<pair<float, Node *> *>::iterator it = vertexWeights[b].begin(); it != vertexWeights[b].end(); it++)
								m->m_vertexWeights[b][c++] = (*it);
						}
					}

					delete [] vertexWeights;
				}
			}
		}

		if(scene->HasAnimations())
		{
			unsigned int numAnimations = scene->mNumAnimations;
			asset->m_numAnimations = numAnimations;
			asset->m_animations = new Animation *[numAnimations];
			for(unsigned int a = 0; a < numAnimations; a++)
			{
				aiAnimation *animData = scene->mAnimations[a];

				Animation *anim = new Animation();
				asset->m_animations[a] = anim;
				anim->m_name = string(animData->mName.data);
				anim->m_rootAsset = asset;
				asset->m_animationMap[anim->m_name] = anim;
//				print("Animation %i: '%s'\n", a, anim->m_name.c_str());

				anim->m_duration = (float)animData->mDuration;
				anim->m_ticksPerSecond = (float)(animData->mTicksPerSecond != 0.0 ? animData->mTicksPerSecond : 25.0);

				unsigned int numChannels = animData->mNumChannels;
				anim->m_numChannels = numChannels;
				anim->m_channels = new AnimationChannel *[numChannels];
				for(unsigned int b = 0; b < numChannels; b++)
				{
					aiNodeAnim *chanData = animData->mChannels[b];

					AnimationChannel *chan = new AnimationChannel();
					anim->m_channels[b] = chan;
					chan->m_targetNode = asset->m_nodeMap[string(chanData->mNodeName.data)];

					unsigned int numPositionKeys = chanData->mNumPositionKeys;
					chan->m_numPositionKeys = numPositionKeys;
					chan->m_positionKeys = new pair<float, Vector3<float> >[numPositionKeys];
					for(unsigned int c = 0; c < numPositionKeys; c++)
					{
						float time = (float)chanData->mPositionKeys[c].mTime;
						Vector3<float> vec;
						vec[0] = chanData->mPositionKeys[c].mValue.x;
						vec[1] = chanData->mPositionKeys[c].mValue.y;
						vec[2] = chanData->mPositionKeys[c].mValue.z;
						chan->m_positionKeys[c] = pair<float, Vector3<float> >(time, vec);
					}

					unsigned int numRotationKeys = chanData->mNumRotationKeys;
					chan->m_numRotationKeys = numRotationKeys;
					chan->m_rotationKeys = new pair<float, Quaternion<float> >[numRotationKeys];

					for(unsigned int c = 0; c < numRotationKeys; c++)
					{
						float time = (float)chanData->mRotationKeys[c].mTime;
						Quaternion<float> quat;
						quat[0] = chanData->mRotationKeys[c].mValue.w;
						quat[1] = chanData->mRotationKeys[c].mValue.x;
						quat[2] = chanData->mRotationKeys[c].mValue.y;
						quat[3] = chanData->mRotationKeys[c].mValue.z;

						chan->m_rotationKeys[c] = pair<float, Quaternion<float> >(time, quat);
					}
					
					unsigned int numScalingKeys = chanData->mNumScalingKeys;
					chan->m_numScalingKeys = numScalingKeys;
					chan->m_scalingKeys = new pair<float, Vector3<float> >[numScalingKeys];
					for(unsigned int c = 0; c < numScalingKeys; c++)
					{
						float time = (float)chanData->mScalingKeys[c].mTime;
						Vector3<float> vec;
						vec[0] = chanData->mScalingKeys[c].mValue.x;
						vec[1] = chanData->mScalingKeys[c].mValue.y;
						vec[2] = chanData->mScalingKeys[c].mValue.z;
						chan->m_scalingKeys[c] = pair<float, Vector3<float> >(time, vec);
					}
				}
			}
		}

		return asset;
	}
	Asset *Asset::createFromMemory(const unsigned char *memory, unsigned int size)
	{
		return 0;
	}
	Asset::Asset()
	{
		m_animations = 0;
		m_materials = 0;
		m_meshes = 0;
		m_nodes = 0;
		m_numAnimations = 0;
		m_numMaterials = 0;
		m_numMeshes = 0;
		m_numNodes = 0;
	}
	Asset::Instance::Instance(Asset *asset, o3dAsset *object)
	{
		m_animationThread = 0;
		m_asset = asset;
		m_object = object;
		m_runAnimationThread = false;

		m_nodeInstances = new Asset::Node::Instance *[asset->m_numNodes];
		for(unsigned int a = 0; a < asset->m_numNodes; a++)
			m_nodeInstances[a] = new Asset::Node::Instance(this, asset->m_nodes[a]);
	}
	Asset::Instance::~Instance()
	{
		if(m_animationThread)
		{
			stopAllAnimations();
			m_runAnimationThread = false;
			SDL_WaitThread((SDL_Thread *)m_animationThread, 0);
		}

		for(unsigned int a = 0; a < m_asset->m_numNodes; a++)
			delete m_nodeInstances[a];
		delete [] m_nodeInstances;

		for(map<Animation *, Animation::Instance *>::iterator it = m_animationInstances.begin(); it != m_animationInstances.end(); it++)
			delete it->second;
		for(map<Mesh *, Mesh::Instance *>::iterator it = m_meshInstances.begin(); it != m_meshInstances.end(); it++)
			delete it->second;
	}
	int animationThread(void *assetInstancePtr)
	{
		Asset::Instance *assetInstance = (Asset::Instance *)assetInstancePtr;
		Asset *asset = assetInstance->m_asset;
		Game *game = Game::getCurrent();
		unsigned int lastTime = game->getRunTime();

		while(game->isRunning() && assetInstance->m_runAnimationThread)
		{
			SDL_Delay(1);
			unsigned int runTime = game->getRunTime();
			unsigned int updateTick = 1000 / 60;
			if((runTime - lastTime) < updateTick)
				continue;
			if(!assetInstance->m_animationInstances.size())
				break;

			map<Asset::Node *, vector<Asset::Node::Instance *> > nodeInstanceVectorMap;

			map<Asset::Animation *, Asset::Animation::Instance *>::iterator it = assetInstance->m_animationInstances.begin();
			while(it != assetInstance->m_animationInstances.end())
			{
				if(it->second->isRunning())
				{
					Asset::Animation *anim = it->first;
					Asset::Animation::Instance *animInstance = it->second;
					animInstance->step(runTime - lastTime);
					for(unsigned int a = 0; a < anim->m_numChannels; a++)
					{
						Asset::AnimationChannel *channel = anim->m_channels[a];
						nodeInstanceVectorMap[channel->m_targetNode].insert(nodeInstanceVectorMap[channel->m_targetNode].end(), animInstance->m_nodeInstances[channel->m_targetNode]);
					}

					it++;
				}
				else
				{
					delete it->second;
					assetInstance->m_animationInstances.erase(it++);
				}
			}

			for(unsigned int a = 0; a < asset->m_numNodes; a++)
			{
				Asset::Node *node = asset->m_nodes[a];
				unsigned int count = nodeInstanceVectorMap.count(node);
				if(count)
				{
					Quaternion<float> rotation;
					Vector3<float> position;
					Vector3<float> scale;

					//- Blend the Node Instance Information -
					for(vector<Asset::Node::Instance *>::iterator it = nodeInstanceVectorMap[node].begin(); it != nodeInstanceVectorMap[node].end(); it++)
					{
						Asset::Node::Instance *nodeInstance = *it;
						position += nodeInstance->m_positionOffset;
						rotation = nodeInstance->m_rotationOffset;
						scale = nodeInstance->m_scaleOffset;
					}
					position /= count;

					//- Construct Transformation -
					Matrix4x4<float> mat = rotation.getMatrix4x4();

					mat[0] *= scale[0];
					mat[1] *= scale[1];
					mat[2] *= scale[2];
					mat[3] = position[0];

					mat[4] *= scale[0];
					mat[5] *= scale[1];
					mat[6] *= scale[2];
					mat[7] = position[1];

					mat[8] *= scale[0];
					mat[9] *= scale[1];
					mat[10] *= scale[2];
					mat[11] = position[2];

					assetInstance->m_nodeInstances[a]->m_transformation = mat;
					if(node->m_parent)
						assetInstance->m_nodeInstances[a]->m_absoluteTransformation = assetInstance->m_nodeInstances[node->m_parent->m_id]->m_absoluteTransformation * mat;
					else
						assetInstance->m_nodeInstances[a]->m_absoluteTransformation = mat;
				}
				else
					(*assetInstance->m_nodeInstances[a]) = Asset::Node::Instance(assetInstance, asset->m_nodes[a]);
			}

			lastTime = runTime;
		}

		return 0;
	}
	o3dAsset *Asset::Instance::getObject() const
	{
		return m_object;
	}
	bool Asset::Instance::isRunningAnimation(const char *name) const
	{
		if(!m_asset->m_animationMap.count(name))
			return false;
		return m_animationInstances.count(m_asset->m_animationMap[name]) != 0;
	}
	void Asset::Instance::playAnimation(const char *name, bool loop)
	{
		if(isRunningAnimation(name))
		{
			Asset::Animation::Instance *animationInstance = m_animationInstances[m_asset->m_animationMap[name]];
			animationInstance->m_loop = loop;
			animationInstance->reset();
		}
		else if(m_asset->m_animationMap.count(name))
		{
			if(!m_animationThread)
			{
				m_runAnimationThread = true;
				m_animationThread = SDL_CreateThread(animationThread, "AnimationThread", this);
			}

			Asset::Animation *animation = m_asset->m_animationMap[name];
			Asset::Animation::Instance *animationInstance = new Asset::Animation::Instance(this, animation);
			m_animationInstances[animation] = animationInstance;
			animationInstance->m_loop = loop;
			animationInstance->start();
		}
	}
	void Asset::Instance::render()
	{
		if(m_asset->m_numNodes)
			m_asset->m_nodes[0]->render(this);
	}
	void Asset::Instance::stopAnimation(const char *name)
	{
		if(isRunningAnimation(name))
		{
			Asset::Animation *animation = m_asset->m_animationMap[name];
			Asset::Animation::Instance *animationInstance = m_animationInstances[animation];
			delete animationInstance;
			m_animationInstances.erase(animation);
		}
	}
	void Asset::Instance::stopAllAnimations()
	{
		map<Asset::Animation *, Asset::Animation::Instance *>::iterator it = m_animationInstances.begin();
		while(it != m_animationInstances.end())
		{
			delete it->second;
			m_animationInstances.erase(it++);
		}
	}
}
