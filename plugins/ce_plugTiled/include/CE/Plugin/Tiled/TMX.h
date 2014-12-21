#ifndef _CE_PLUGIN_TILED_TMX_H_
#define _CE_PLUGIN_TILED_TMX_H_

//- Standard Library -
#include <string>
#include <vector>

//- Centhra Engine -
#include <CE/Game2D/World.h>
#include <CE/UI/CameraView2DCtrl.h>

namespace ce
{
	namespace plugin
	{
		namespace tiled
		{
			class TMX
			{
			public:
				typedef struct TileSetDef
				{
					std::string name, file;
					unsigned short firstGID, tileWidth, tileHeight, imageWidth, imageHeight;
					Image *image;
					game2d::TileSet *tileSet;
				} TileSetDef;

				typedef enum LayerType
				{
					Layer_Unknown,
					Layer_Tile,
					Layer_Object,
					Layer_Image
				} LayerType;

				class Layer
				{
				public:
					std::string m_name;
					bool m_renderView;
					unsigned short m_type, m_width, m_height, m_idx;
					std::map<std::string, std::string> m_propertyMap;

					Layer();
				};

				class TileLayer : public Layer
				{
				public:
					game2d::TileMap *m_tileMap;

					TileLayer();
					~TileLayer();
				};

				class ObjectDef
				{
				public:
					typedef enum ObjectType
					{
						Object_Unknown,
						Object_Tile,
						Object_Point,
						Object_Rectangle,
						Object_Ellipse,
						Object_Polygon,
						Object_Polyline
					} ObjectType;

					unsigned short m_type, m_width, m_height, m_gid;
					short m_x, m_y;
					std::string m_name, m_typeStr;
					std::map<std::string, std::string> m_propertyMap;

					ObjectDef();
				};

				class ObjectLayer : public Layer
				{
				public:
					std::vector<ObjectDef *> m_objectDefVec;

					ObjectLayer();
					~ObjectLayer();
				};

				class ImageLayer : public Layer
				{
				public:
					std::string m_imageFile;

					ImageLayer();
					~ImageLayer();
				};

				static TMX *CreateFromFile(const char *file, TMX *tmx = 0);

				TMX();
				~TMX();

				void PopulateWorld(game2d::World *world, Vector2<float> scale = Vector2<float>(1.f, 1.f));
				void ExportWorldToFile(game2d::World *world, const char *file);

				virtual game2d::PhysicalObject *LoadObject(ObjectDef *object);
				virtual void SaveObjects(void *groupLayer);

			protected:
				std::string m_file, m_orientation, m_version;
				Vector2<unsigned short> m_size, m_tileSize;
				std::vector<Layer *> m_layerVec;
				std::vector<TileSetDef *> m_tileSetVec;
				std::map<game2d::TileSet *, TileSetDef *> m_tileSetMap;
				std::map<std::string, std::string> m_propertyMap;
			};
		}
	}
}

#endif
