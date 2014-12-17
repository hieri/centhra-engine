#ifndef _CE_GAME2D_WALL_H_
#define _CE_GAME2D_WALL_H_

//- Centhra Engine -
#include <CE/Game2D/PhysicalObject.h>
#include <CE/Game2D/PhysicalGroup.h>

namespace ce
{
	namespace game2d
	{
		class Wall : public PhysicalObject
		{
		protected:
			virtual void DoRender();

		public:
			static float GetWallThickness();

			Wall(Vector2<float> position, Vector2<float> extent);
			virtual ~Wall();

			virtual void ResetCollisionMask();
			virtual void OnAdded(Group *group);
		};

		class WallGrid
		{
			void CalculateObstructionPoints(Vector2<float> scale);

		public:
			typedef enum Type
			{
				Horizontal = 2,
				Vertical = 4,
				Post = 8,
				Fill = 16,
				Done = 32
			} Type;

			typedef struct ObstructionPoint
			{
				Vector2<float> position;
				bool positiveX, positiveY;
			} ObstructionPoint;

			std::vector<ObstructionPoint> m_obstructionPoints;

			unsigned char **m_data;
			unsigned short m_width, m_height;

			WallGrid(unsigned short width, unsigned short height);
			~WallGrid();
			void GenerateWalls(PhysicalGroup *group, Vector2<float> scale);
		};
	}
}

#endif
