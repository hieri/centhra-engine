#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

// -OpenGL -
#include <GL/gl.h>

#ifdef _WIN32
	#include <GL/glext.h>
	#include <GL/wglext.h>
#elif __linux__
	#include <GL/glx.h>
#endif

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Game2D/Wall.h>

using namespace ce;
using namespace std;

namespace ce
{
	namespace game2d
	{
		float g_wallThickness = 0.05f;
		float g_doubleWallThickness = 2.f * g_wallThickness;

		float g_postThickness = 0.1f;
		float g_doublePostThickness = 2.f * g_postThickness;

		float g_obstructionPointPadding = 0.f;

		float Wall::GetWallThickness()
		{
			return g_wallThickness;
		}
		float Wall::GetDoubleWallThickness()
		{
			return g_doubleWallThickness;
		}
		float Wall::GetPostThickness()
		{
			return g_postThickness;
		}
		float Wall::GetDoublePostThickness()
		{
			return g_doublePostThickness;
		}

		unsigned short g_wallColMask = Mask_Object | Mask_Wall | Mask_Prop | Mask_Projectile;

		Wall::Wall(Vector2<float> position, Vector2<float> extent) : PhysicalObject(position, extent)
		{
			m_color = Color(255, 0, 0, 255);
			SetTypeMask(Mask_Wall);
			SetCollisionMask(g_wallColMask);
			SetStatic(true);
		}
		Wall::~Wall()
		{
		}
		void Wall::ResetCollisionMask()
		{
			SetCollisionMask(g_wallColMask);
		}
		void Wall::OnAdded(Group *group)
		{
			SetFixedRotation(true);
		}

		WallGrid::WallGrid(unsigned short width, unsigned short height) : m_width(width), m_height(height)
		{
			m_data = new unsigned char *[width + 1];
			for(unsigned short a = 0; a <= width; a++)
			{
				m_data[a] = new unsigned char[height + 1];
				for(unsigned short b = 0; b <= height; b++)
					m_data[a][b] = 0;
			}
		}
		WallGrid::~WallGrid()
		{
			for(unsigned short a = 0; a <= m_width; a++)
				delete [] m_data[a];
			delete [] m_data;
		}
		unsigned short WallGrid::GetWidth() const
		{
			return m_width;
		}
		unsigned short WallGrid::GetHeight() const
		{
			return m_height;
		}
		bool WallGrid::SetHorizontal(unsigned short x, unsigned short y, bool state)
		{
			if(state)
			{
				if(!(m_data[x][y] & Horizontal))
				{
					m_data[x][y] |= 1 | Horizontal;
					return true;
				}
			}
			else
			{
				if((m_data[x][y] & Horizontal))
				{
					m_data[x][y] &= ~Horizontal;
					if(m_data[x][y] == 1)
						m_data[x][y] = 0;
					return true;
				}
			}
			return false;
		}
		bool WallGrid::SetVertical(unsigned short x, unsigned short y, bool state)
		{
			if(state)
			{
				if(!(m_data[x][y] & Vertical))
				{
					m_data[x][y] |= 1 | Vertical;
					return true;
				}
			}
			else
			{
				if((m_data[x][y] & Vertical))
				{
					m_data[x][y] &= ~Vertical;
					if(m_data[x][y] == 1)
						m_data[x][y] = 0;
					return true;
				}
			}
			return false;
		}
		bool WallGrid::SetPost(unsigned short x, unsigned short y, bool state)
		{
			if(state)
			{
				if(!(m_data[x][y] & Post))
				{
					m_data[x][y] |= 1 | Post;
					return true;
				}
			}
			else
			{
				if((m_data[x][y] & Post))
				{
					m_data[x][y] &= ~Post;
					if(m_data[x][y] == 1)
						m_data[x][y] = 0;
					return true;
				}
			}
			return false;
		}
		bool WallGrid::SetFill(unsigned short x, unsigned short y, bool state)
		{
			if(state)
			{
				if(!(m_data[x][y] & Fill))
				{
					m_data[x][y] |= 1 | Fill;
					return true;
				}
			}
			else
			{
				if((m_data[x][y] & Fill))
				{
					m_data[x][y] &= ~Fill;
					if(m_data[x][y] == 1)
						m_data[x][y] = 0;
					return true;
				}
			}
			return false;
		}
		bool WallGrid::IsHorizontal(unsigned short x, unsigned short y) const
		{
			return (m_data[x][y] & Horizontal) != 0;
		}
		bool WallGrid::IsVertical(unsigned short x, unsigned short y) const
		{
			return (m_data[x][y] & Vertical) != 0;
		}
		bool WallGrid::IsPost(unsigned short x, unsigned short y) const
		{
			return (m_data[x][y] & Post) != 0;
		}
		bool WallGrid::IsFill(unsigned short x, unsigned short y) const
		{
			return (m_data[x][y] & Fill) != 0;
		}
		unsigned char WallGrid::GetPoint(unsigned short x, unsigned short y) const
		{
			return m_data[x][y];
		}
		void WallGrid::SetPoint(unsigned short x, unsigned short y, unsigned char point)
		{
			m_data[x][y] = point;
		}
		void WallGrid::GenerateWalls(PhysicalGroup *group, Vector2<float> scale)
		{
			unsigned short start = 0, length = 0;
			bool isPlacing = false;
			Wall *wall = 0;

			//- Fill -
			for(unsigned short b = 0; b <= m_height; b++)
				for(unsigned short a = 0; a <= m_width; a++)
				{
					if(m_data[a][b] & Fill && !(m_data[a][b] & Done))
					{
						unsigned short w = 0, h = 0, _w = 0, _h = 0;
						unsigned short limit = 0;
						bool valid = true, hasLimit = false;
						for(unsigned short _b = b; _b <= m_height; _b++)
						{
							_h = _b - b;
							for(unsigned short _a = a; _a <= m_width; _a++)
							{
								if(_b == b && _a == a)
									continue;
								_w = _a - a;
								if(hasLimit)
								{
									//- Don't go farther than the predetermined width -
									if(_w > w)
										break;
									//- Stop entirely if we can't go further -
									if(!(m_data[_a][_b] & Fill) || m_data[_a][_b] & Done)
									{
										valid = false;
										break;
									}
								}
								else
								{
									//- Extend as far right as possible -
									if(!(m_data[_a][_b] & Fill) || m_data[_a][_b] & Done)
									{
										w = _a - a - 1;
										hasLimit = true;
										break;
									}
									if(_a == m_width)
									{
										w = _a - a;
										hasLimit = true;
										break;
									}
								}
							}
//							if(!w)
//								break;
							if(!valid)
								break;
							if(_h > h)
								h = _h;
						}
						for(unsigned short _b = b; _b <= (b + h); _b++)
							for(unsigned short _a = a; _a <= (a + w); _a++)
								m_data[_a][_b] |= Done;
						w++;
						h++;
						wall = new Wall(Vector2<float>(((float)a + (((float)w) / 2.f)) * scale[0], ((float)b + (((float)h) / 2.f)) * scale[1]), Vector2<float>(((float)w + g_doubleWallThickness) * scale[0], ((float)h + g_doubleWallThickness) * scale[1]));
						group->Add(wall);
					}
				}
			for(unsigned short b = 0; b <= m_height; b++)
				for(unsigned short a = 0; a <= m_width; a++)
					if(m_data[a][b] & Done)
						m_data[a][b] &= ~Done;

			//- Vertical -
			for(unsigned short a = 0; a <= m_width; a++)
				for(unsigned short b = 0; b <= m_height; b++)
				{
					if(isPlacing)
					{
						if(!(m_data[a][b] & Vertical))
						{
							isPlacing = false;
							length = b - start;
							if(!length)
								continue;
							wall = new Wall(Vector2<float>((float)a * scale[0], ((float)length / 2.f + (float)start) * scale[1]), Vector2<float>(g_doubleWallThickness * scale[0], ((float)length + g_doubleWallThickness) * scale[1]));
							group->Add(wall);
						}
					}
					else
					{
						if(m_data[a][b] & Vertical)
						{
							start = b;
							isPlacing = true;
						}
					}
				}

			//- Horizontal -
			for(unsigned short b = 0; b <= m_height; b++)
				for(unsigned short a = 0; a <= m_width; a++)
				{
					if(isPlacing)
					{
						if(!(m_data[a][b] & Horizontal))
						{
							isPlacing = false;
							length = a - start;
							if(!length)
								continue;
							wall = new Wall(Vector2<float>(((float)length / 2.f + (float)start) * scale[0], (float)b * scale[1]), Vector2<float>(((float)length + g_doubleWallThickness) * scale[0], g_doubleWallThickness * scale[1]));
							group->Add(wall);
						}
					}
					else
					{
						if(m_data[a][b] & Horizontal)
						{
							start = a;
							isPlacing = true;
						}
					}
				}

			//- Post -
			for(unsigned short b = 0; b <= m_height; b++)
				for(unsigned short a = 0; a <= m_width; a++)
					if(m_data[a][b] & Post)
					{
						wall = new Wall(Vector2<float>((float)a * scale[0], (float)b * scale[1]), Vector2<float>(g_doublePostThickness * scale[0], g_doublePostThickness * scale[1]));
						group->Add(wall);
					}

			CalculateObstructionPoints(scale);
		}
		void WallGrid::CalculateObstructionPoints(Vector2<float> scale)
		{
			for(unsigned short b = 0; b <= m_height; b++)
				for(unsigned short a = 0; a <= m_width; a++)
				{
					if(a && b)
					{
						//- Bottom Left -
						if((m_data[a][b] & Horizontal || m_data[a][b] & Vertical) && !(m_data[a - 1][b] & Horizontal) && !(m_data[a][b - 1] & Vertical))
						{
							ObstructionPoint point = { Vector2<float>(((float)a - g_wallThickness - g_obstructionPointPadding) * scale[0], ((float)b - g_wallThickness - g_obstructionPointPadding) * scale[1]), true, true };
							m_obstructionPoints.push_back(point);
						}
						//- Bottom Right -
						if((m_data[a - 1][b] & Horizontal || m_data[a][b] & Vertical) && !(m_data[a][b] & Horizontal) && !(m_data[a][b - 1] & Vertical))
						{
							ObstructionPoint point = { Vector2<float>(((float)a + g_wallThickness + g_obstructionPointPadding) * scale[0], ((float)b - g_wallThickness - g_obstructionPointPadding) * scale[1]), false, true };
							m_obstructionPoints.push_back(point);
						}
						//- Top Left -
						if((m_data[a][b - 1] & Vertical || m_data[a][b] & Horizontal) && !(m_data[a][b] & Vertical) && !(m_data[a - 1][b] & Horizontal))
						{
							ObstructionPoint point = { Vector2<float>(((float)a - g_wallThickness - g_obstructionPointPadding) * scale[0], ((float)b + g_wallThickness + g_obstructionPointPadding) * scale[1]), true, false };
							m_obstructionPoints.push_back(point);
						}
						//- Top Right -
						if((m_data[a - 1][b] & Horizontal || (m_data[a][b - 1] & Vertical)) && !(m_data[a][b] & Horizontal) && !(m_data[a][b] & Vertical))
						{
							ObstructionPoint point = { Vector2<float>(((float)a + g_wallThickness + g_obstructionPointPadding) * scale[0], ((float)b + g_wallThickness + g_obstructionPointPadding) * scale[1]), false, false };
							m_obstructionPoints.push_back(point);
						}
					}
				
					if(!a && !b)
					{
						//- Bottom Left -
						if(m_data[a][b] & Horizontal || m_data[a][b] & Vertical)
						{
							ObstructionPoint point = { Vector2<float>(((float)a - g_wallThickness - g_obstructionPointPadding) * scale[0], ((float)b - g_wallThickness - g_obstructionPointPadding) * scale[1]), true, true };
							m_obstructionPoints.push_back(point);
						}
						//- Bottom Right -
						if(m_data[a][b] & Vertical && !(m_data[a][b] & Horizontal))
						{
							ObstructionPoint point = { Vector2<float>(((float)a + g_wallThickness + g_obstructionPointPadding) * scale[0], ((float)b - g_wallThickness - g_obstructionPointPadding) * scale[1]), false, true };
							m_obstructionPoints.push_back(point);
						}
						//- Top Left -
						if(m_data[a][b] & Horizontal && !(m_data[a][b] & Vertical))
						{
							ObstructionPoint point = { Vector2<float>(((float)a - g_wallThickness - g_obstructionPointPadding) * scale[0], ((float)b + g_wallThickness + g_obstructionPointPadding) * scale[1]), true, false };
							m_obstructionPoints.push_back(point);
						}
					}

					if(!a && b)
					{
						//- Bottom Left -
						if((m_data[a][b] & Horizontal || m_data[a][b] & Vertical) && !(m_data[a][b - 1] & Vertical))
						{
							ObstructionPoint point = { Vector2<float>(((float)a - g_wallThickness - g_obstructionPointPadding) * scale[0], ((float)b - g_wallThickness - g_obstructionPointPadding) * scale[1]), true, true };
							m_obstructionPoints.push_back(point);
						}
						//- Bottom Right -
						if(m_data[a][b] & Vertical && !(m_data[a][b] & Horizontal) && !(m_data[a][b - 1] & Vertical))
						{
							ObstructionPoint point = { Vector2<float>(((float)a + g_wallThickness + g_obstructionPointPadding) * scale[0], ((float)b - g_wallThickness - g_obstructionPointPadding) * scale[1]), false, true };
							m_obstructionPoints.push_back(point);
						}
						//- Top Left -
						if((m_data[a][b - 1] & Vertical || m_data[a][b] & Horizontal) && !(m_data[a][b] & Vertical))
						{
							ObstructionPoint point = { Vector2<float>(((float)a - g_wallThickness - g_obstructionPointPadding) * scale[0], ((float)b + g_wallThickness + g_obstructionPointPadding) * scale[1]), true, false };
							m_obstructionPoints.push_back(point);
						}
						//- Top Right -
						if(m_data[a][b - 1] & Vertical && !(m_data[a][b] & Horizontal) && !(m_data[a][b] & Vertical))
						{
							ObstructionPoint point = { Vector2<float>(((float)a + g_wallThickness + g_obstructionPointPadding) * scale[0], ((float)b + g_wallThickness + g_obstructionPointPadding) * scale[1]), false, false };
							m_obstructionPoints.push_back(point);
						}
					}

					if(a && !b)
					{
						//- Bottom Left -
						if((m_data[a][b] & Horizontal || m_data[a][b] & Vertical) && !(m_data[a - 1][b] & Horizontal))
						{
							ObstructionPoint point = { Vector2<float>(((float)a - g_wallThickness - g_obstructionPointPadding) * scale[0], ((float)b - g_wallThickness - g_obstructionPointPadding) * scale[1]), true, true };
							m_obstructionPoints.push_back(point);
						}
						//- Bottom Right -
						if((m_data[a - 1][b] & Horizontal || m_data[a][b] & Vertical) && !(m_data[a][b] & Horizontal))
						{
							ObstructionPoint point = { Vector2<float>(((float)a + g_wallThickness + g_obstructionPointPadding) * scale[0], ((float)b - g_wallThickness - g_obstructionPointPadding) * scale[1]), false, true };
							m_obstructionPoints.push_back(point);
						}
						//- Top Left -
						if(m_data[a][b] & Horizontal && !(m_data[a][b] & Vertical) && !(m_data[a - 1][b] & Horizontal))
						{
							ObstructionPoint point = { Vector2<float>(((float)a - g_wallThickness - g_obstructionPointPadding) * scale[0], ((float)b + g_wallThickness + g_obstructionPointPadding) * scale[1]), true, false };
							m_obstructionPoints.push_back(point);
						}
						//- Top Right -
						if(m_data[a - 1][b] & Horizontal && !(m_data[a][b] & Horizontal) && !(m_data[a][b] & Vertical))
						{
							ObstructionPoint point = { Vector2<float>(((float)a + g_wallThickness + g_obstructionPointPadding) * scale[0], ((float)b + g_wallThickness + g_obstructionPointPadding) * scale[1]), false, false };
							m_obstructionPoints.push_back(point);
						}
					}

					if(m_data[a][b] & Post)
					{
						//- Post -
						{
							ObstructionPoint point = {Vector2<float>(((float)a - g_postThickness - g_obstructionPointPadding) * scale[0], ((float)b - g_postThickness - g_obstructionPointPadding) * scale[1]), true, true};
							m_obstructionPoints.push_back(point);
						}
						{
							ObstructionPoint point = {Vector2<float>(((float)a + g_postThickness + g_obstructionPointPadding) * scale[0], ((float)b - g_postThickness - g_obstructionPointPadding) * scale[1]), false, true};
							m_obstructionPoints.push_back(point);
						}
						{
							ObstructionPoint point = { Vector2<float>(((float)a - g_postThickness - g_obstructionPointPadding) * scale[0], ((float)b + g_postThickness + g_obstructionPointPadding) * scale[1]), true, false };
							m_obstructionPoints.push_back(point);
						}
						{
							ObstructionPoint point = { Vector2<float>(((float)a + g_postThickness + g_obstructionPointPadding) * scale[0], ((float)b + g_postThickness + g_obstructionPointPadding) * scale[1]), false, false };
							m_obstructionPoints.push_back(point);
						}
					}

					//- Inner Corners -
					if(a && b)
					{
						//- Bottom Left -
						if(m_data[a - 1][b] & Horizontal && m_data[a][b - 1] & Vertical)
						{
							ObstructionPoint point = { Vector2<float>(((float)a - g_wallThickness - g_obstructionPointPadding) * scale[0], ((float)b - g_wallThickness - g_obstructionPointPadding) * scale[1]), true, true };
							m_obstructionPoints.push_back(point);
						}
						//- Bottom Right -
						if(m_data[a][b] & Horizontal && m_data[a][b - 1] & Vertical)
						{
							ObstructionPoint point = { Vector2<float>(((float)a + g_wallThickness + g_obstructionPointPadding) * scale[0], ((float)b - g_wallThickness - g_obstructionPointPadding) * scale[1]), false, true };
							m_obstructionPoints.push_back(point);
						}
						//- Top Left -
						if(m_data[a - 1][b] & Horizontal && m_data[a][b] & Vertical)
						{
							ObstructionPoint point = { Vector2<float>(((float)a - g_wallThickness - g_obstructionPointPadding) * scale[0], ((float)b + g_wallThickness + g_obstructionPointPadding) * scale[1]), true, false };
							m_obstructionPoints.push_back(point);
						}
						//- Top Right -
						if(m_data[a][b] & Horizontal && m_data[a][b] & Vertical)
						{
							ObstructionPoint point = { Vector2<float>(((float)a + g_wallThickness + g_obstructionPointPadding) * scale[0], ((float)b + g_wallThickness + g_obstructionPointPadding) * scale[1]), false, false };
							m_obstructionPoints.push_back(point);
						}
					}

					if(!a && !b)
					{
						//- Top Right -
						if(m_data[a][b] & Horizontal && m_data[a][b] & Vertical)
						{
							ObstructionPoint point = { Vector2<float>(((float)a + g_wallThickness + g_obstructionPointPadding) * scale[0], ((float)b + g_wallThickness + g_obstructionPointPadding) * scale[1]), false, false };
							m_obstructionPoints.push_back(point);
						}
					}

					if(!a && b)
					{
						//- Bottom Right -
						if(m_data[a][b] & Horizontal && m_data[a][b - 1] & Vertical)
						{
							ObstructionPoint point = { Vector2<float>(((float)a + g_wallThickness + g_obstructionPointPadding) * scale[0], ((float)b - g_wallThickness - g_obstructionPointPadding) * scale[1]), false, true };
							m_obstructionPoints.push_back(point);
						}
						//- Top Right -
						if(m_data[a][b] & Horizontal && m_data[a][b] & Vertical)
						{
							ObstructionPoint point = { Vector2<float>(((float)a + g_wallThickness + g_obstructionPointPadding) * scale[0], ((float)b + g_wallThickness + g_obstructionPointPadding) * scale[1]), false, false };
							m_obstructionPoints.push_back(point);
						}
					}

					if(a && !b)
					{
						//- Top Left -
						if(m_data[a - 1][b] & Horizontal && m_data[a][b] & Vertical)
						{
							ObstructionPoint point = { Vector2<float>(((float)a - g_wallThickness - g_obstructionPointPadding) * scale[0], ((float)b + g_wallThickness + g_obstructionPointPadding) * scale[1]), true, false };
							m_obstructionPoints.push_back(point);
						}
						//- Top Right -
						if(m_data[a][b] & Horizontal && m_data[a][b] & Vertical)
						{
							ObstructionPoint point = { Vector2<float>(((float)a + g_wallThickness + g_obstructionPointPadding) * scale[0], ((float)b + g_wallThickness + g_obstructionPointPadding) * scale[1]), false, false };
							m_obstructionPoints.push_back(point);
						}
					}

					if(a && !b)
					{
						//- Top Left -
						if(m_data[a - 1][b] & Horizontal && m_data[a][b] & Vertical)
						{
							ObstructionPoint point = { Vector2<float>(((float)a - g_wallThickness - g_obstructionPointPadding) * scale[0], ((float)b + g_wallThickness + g_obstructionPointPadding) * scale[1]), true, false };
							m_obstructionPoints.push_back(point);
						}
						//- Top Right -
						if(m_data[a][b] & Horizontal && m_data[a][b] & Vertical)
						{
							ObstructionPoint point = { Vector2<float>(((float)a + g_wallThickness + g_obstructionPointPadding) * scale[0], ((float)b + g_wallThickness + g_obstructionPointPadding) * scale[1]), false, false };
							m_obstructionPoints.push_back(point);
						}
					}
				}
		}
		vector<WallGrid::ObstructionPoint> *WallGrid::GetObstructionPoints()
		{
			return &m_obstructionPoints;
		}
	}
}
