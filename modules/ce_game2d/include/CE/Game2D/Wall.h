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
		public:
			static float GetWallThickness();
			static float GetDoubleWallThickness();
			static float GetPostThickness();
			static float GetDoublePostThickness();

			Wall(Vector2<float> position, Vector2<float> extent);
			virtual ~Wall();

			virtual void ResetCollisionMask();
			virtual void OnAdded(Group *group);
		};

		class WallGrid
		{
			void CalculateObstructionPoints(Vector2<float> scale);

		public:
			typedef struct ObstructionPoint
			{
				Vector2<float> position;
				bool positiveX, positiveY;
			} ObstructionPoint;
		protected:
			std::vector<ObstructionPoint> m_obstructionPoints;
			friend class Wall;
		public:
			std::vector<ObstructionPoint> *GetObstructionPoints();

		public:
			unsigned char **m_data;
			unsigned short m_width, m_height;
			typedef enum Type
			{
				Horizontal = 2,
				Vertical = 4,
				Post = 8,
				Fill = 16,
				Done = 32
			} Type;

			WallGrid(unsigned short width, unsigned short height);
			~WallGrid();

			unsigned short GetWidth() const;
			unsigned short GetHeight() const;

			bool SetHorizontal(unsigned short x, unsigned short y, bool state);
			bool SetVertical(unsigned short x, unsigned short y, bool state);
			bool SetPost(unsigned short x, unsigned short y, bool state);
			bool SetFill(unsigned short x, unsigned short y, bool state);

			bool IsHorizontal(unsigned short x, unsigned short y) const;
			bool IsVertical(unsigned short x, unsigned short y) const;
			bool IsPost(unsigned short x, unsigned short y) const;
			bool IsFill(unsigned short x, unsigned short y) const;

			unsigned char GetPoint(unsigned short x, unsigned short y) const;
			void SetPoint(unsigned short x, unsigned short y, unsigned char point);
			void GenerateWalls(PhysicalGroup *group, Vector2<float> scale);
		};
	}
}

#endif
