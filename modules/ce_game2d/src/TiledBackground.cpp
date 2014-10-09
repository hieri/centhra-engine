//- Centhra Engine -
#include <CE/Game2D/TiledBackground.h>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		TiledBackground::TiledBackground(ui::CameraView2DCtrl *viewCtrl)
		{
			m_viewCtrl = viewCtrl;
		}

		void TiledBackground::PushTileMap(TileMap *tileMap, float scale)
		{
			m_tileMapVec.push_back(pair<TileMap *, float>(tileMap, scale));
		}
		void TiledBackground::Render()
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glPushMatrix();
			Camera *camera = m_viewCtrl->GetCamera();
			if(camera)
			{
				game2d::PhysicalObject *focus = camera->GetFocus();
				{
					Vector2<float> focusPosition = focus->GetPosition();
					Vector2<float> focusExtent = focus->GetExtent();

					Vector2<float> viewScale = m_viewCtrl->GetViewScale();
					Vector2<int_canvas> viewExtent = m_viewCtrl->GetExtent();

					glPushMatrix();
					Vector2<float> half((float)viewExtent[0] / 2.f, (float)viewExtent[1] / 2.f);
					Vector2<float> focusHalf = focusExtent / 2.f;
					glTranslatef(half[0] - viewScale[0] * focusPosition[0], half[1] - viewScale[1] * focusPosition[1], 0.f);
	//						glTranslatef(half[0] - m_viewScale[0] * (focusPosition[0] + focusHalf[0]), half[1] - m_viewScale[1] * (focusPosition[1] + focusHalf[1]), 0.f);
					glScalef(viewScale[0], viewScale[1], 1.f);

					float minX, minY, maxX, maxY;
					minX = ((focusPosition[0] + focusHalf[0]) - half[0] / viewScale[0]);
					minY = ((focusPosition[1] + focusHalf[1]) - half[1] / viewScale[1]);
					maxX = ((focusPosition[0] + focusHalf[0]) + half[0] / viewScale[0]);
					maxY = ((focusPosition[1] + focusHalf[1]) + half[1] / viewScale[1]);

					for(vector<pair<TileMap *, float> >::iterator it = m_tileMapVec.begin(); it != m_tileMapVec.end(); it++)
					{
						glPushMatrix();
						
						TileMap *tileMap = it->first;
						float scale = it->second;
						Vector2<unsigned short> tileSize = tileMap->GetTileSize();
						int _minX, _minY, _maxX, _maxY;
						_minX = (int)((minX / tileSize[0]) / scale) - 1;
						_maxX = (int)((maxX / tileSize[0]) / scale) + 1;
						_minY = (int)((minY / tileSize[1]) / scale) - 1;
						_maxY = (int)((maxY / tileSize[1]) / scale) + 1;						

						if(_minX < 0)
							_minX = 0;
						if(_minY < 0)
							_minY = 0;
						if(_maxX < 0)
							_maxX = 0;
						if(_maxY < 0)
							_maxY = 0;

						glScalef(scale, scale, 1.f);
						tileMap->Render(_minX, _minY, _maxX, _maxY);
						glPopMatrix();
					}

					glPopMatrix();
				}
			}
			glPopMatrix();

			glDisable(GL_BLEND);
		}
	}
}
