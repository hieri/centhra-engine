//- Standard Library -
#include <fstream>
#include <sstream>

//- Centhra Engine -
#include <CE/Game2D/Prop.h>
#include <CE/Renderer.h>
#include <CE/Quaternion.h>
#include <CE/Base.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		map<unsigned short, PropDef *> PropDef::ms_propDefs = map<unsigned short, PropDef *>();
		unsigned short PropDef::ms_nextID = 0;
		map<unsigned short, PropDef *> *PropDef::GetDefTable()
		{
			return &ms_propDefs;
		}
		PropDef *PropDef::GetDefByID(unsigned short propID)
		{
			if(ms_propDefs.count(propID))
				return ms_propDefs[propID];
			return 0;
		}
		PropDef *PropDef::GetDefByName(string name)
		{
			for(map<unsigned short, PropDef *>::iterator it = ms_propDefs.begin(); it != ms_propDefs.end(); it++)
			{
				PropDef *propDef = it->second;
				if(!propDef->m_name.compare(name))
					return propDef;
			}
			return 0;
		}
		void PropDef::LoadFromFile(const char *file)
		{
			ifstream inFile;
			inFile.open(file);

			string in, imageFile, spriteFile;
			while(getline(inFile, in))
			{
				//- Get name, single word only -
				stringstream lineStream(in);
				PropDef *def = new PropDef();
				lineStream >> def->m_name;

				//- Continue if line is a comment -
				if(!def->m_name.compare("//"))
					continue;

				//- Image File -
				getline(lineStream, imageFile, '\"'); //- Flush previous content -
				getline(lineStream, imageFile, '\"');

				//- Attributes -
				lineStream >> def->m_extent[0] >> def->m_extent[1];
				lineStream >> def->m_isStatic >> def->m_hasFixedRotation >> def->m_isCollidable;

				//- Read the sprite file -
				getline(lineStream, spriteFile, '\"'); //- Flush previous content -
				getline(lineStream, spriteFile, '\"');

				//- Create assets -
				def->m_image = Image::CreateFromFile(imageFile.c_str());
				if(spriteFile.length())
				{
					def->m_isAnimated = true;
					def->m_sprite = Sprite::LoadSpriteFromFile(spriteFile.c_str(), def->m_image);
				}

				//- Assign id and store to definition map -
				def->m_id = ms_nextID++;
				ms_propDefs[def->m_id] = def;
			}

			inFile.close();
		}
		void PropDef::Cleanup()
		{
			for(map<unsigned short, PropDef *>::iterator it = ms_propDefs.begin(); it != ms_propDefs.end(); it++)
				delete it->second;
			ms_propDefs.clear();
		}
		PropDef::PropDef()
		{
			m_id = 0;
			m_isAnimated = m_isStatic = m_hasFixedRotation = m_isCollidable = false;
			m_image = 0;
			m_sprite = 0;
		}
		PropDef::~PropDef()
		{
			if(m_isAnimated)
				delete m_sprite;
			delete m_image;
		}
		unsigned short PropDef::GetID() const
		{
			return m_id;
		}
		Vector2<float> PropDef::GetExtent() const
		{
			return m_extent;
		}
		string PropDef::GetName() const
		{
			return m_name;
		}
		Prop *PropDef::Spawn(Vector2<float> position)
		{
			return new Prop(position, m_extent, this);
		}
		void PropDef::UIRender(RenderContext &context)
		{
			ConfigTextured();

			if(m_isAnimated)
				m_sprite->Draw(context, 0, 0.f);
			else
			{
				m_image->Bind();
				RenderSquareTexturedFull(context);
			}
		}

		Prop::Prop(Vector2<float> position, Vector2<float> extent, PropDef *definition) : PhysicalObject(position, extent), m_propDef(definition)
		{
			SetTypeMask(Mask_Prop);
			if(!definition->m_isCollidable)
				SetCollisionMask(0);
			SetStatic(definition->m_isStatic);
			SetFixedRotation(definition->m_hasFixedRotation);
			m_currentAnimation = 1;
			m_animationTime = 0.f;
		}
		PropDef *Prop::GetPropDef() const
		{
			return m_propDef;
		}
		void Prop::OnProcess(float dt)
		{
			if(m_propDef->m_isAnimated)
				m_animationTime = m_propDef->m_sprite->GetAnimationTime(m_currentAnimation, m_animationTime + dt);
		}
		void Prop::DoRender(RenderContext &context)
		{
			if(m_mvChanged)
				CalculateModelViewMatrix();

			ConfigTextured();

			ShaderProgram::TexturedProgram *program = 0;
			if(context.useShaders)
				program = UseTexturedProgram();
			if(program != 0)
			{
				Matrix4x4<float> mvpMatrix = context.mvpMatrix * m_modelViewMatrix;
				glUniformMatrix4fv(program->mvpMatrix, 1, GL_FALSE, &mvpMatrix[0]);
			}
			else
			{
				Matrix4x4<float> mvMatrix = context.modelViewMatrix * m_modelViewMatrix;
				glLoadMatrixf(&mvMatrix[0]);
				glColor4ub(255, 255, 255, 255);
			}
			if(m_propDef->m_isAnimated)
				m_propDef->m_sprite->Draw(context, m_currentAnimation, m_animationTime);
			else
			{
				m_propDef->m_image->Bind();
				RenderSquareTexturedFull(context);
			}
		}
	}
}
