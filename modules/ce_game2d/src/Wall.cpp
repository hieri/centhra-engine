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

	/*	Mask_Object = 1 << (Base_Object + 0),
			Mask_Wall = 1 << (Base_Object + 1),
			Mask_Prop = 1 << (Base_Object + 2),
			Mask_Projectile = 1 << (Base_Object + 3),
			Next_Object

			*/
		unsigned short g_wallColMask = Mask_Object | Mask_Wall | Mask_Prop | Mask_Projectile;

		Wall::Wall(Vector2<float> position, Vector2<float> extent) : PhysicalObject(position, extent)
		{
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
		void Wall::DoRender()
		{
			glTranslatef(m_position[0], m_position[1], 0.f);
			glScalef(m_extent[0], m_extent[1], 1.f);

			glColor3ub(255, 0, 0);
	//		glColor3ub(200, 200, 15);

			glBegin(GL_QUADS);
				glTexCoord2i(0, 1);
				glVertex2f(-0.5f, -0.5f);

				glTexCoord2i(1, 1);
				glVertex2f(0.5f, -0.5f);

				glTexCoord2i(1, 0);
				glVertex2f(0.5f, 0.5f);

				glTexCoord2i(0, 0);
				glVertex2f(-0.5f, 0.5f);
			glEnd();

			glColor3ub(255, 255, 255);
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
		void WallGrid::GenerateWalls(PhysicalGroup *group, Vector2<float> scale)
		{
			unsigned short start = 0, length = 0;
			bool isPlacing = false;
			Wall *wall = 0;

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
							for(unsigned short c = start; c < b; c++)
								m_data[a][c] |= Done;
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
								if(m_data[start][b] & Done)
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
									if(_w >= w)
										break;
									if(!(m_data[_a][_b] & Fill) || m_data[_a][_b] & Done)
									{
										valid = false;
										break;
									}
								}
								else
								{
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
							if(!w)
								break;
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
	}
}
