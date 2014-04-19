Centhra Game Engine
===================

Implemented Modules
-------------------

* ce_base
	* App - Core App template
	* Audio
	* Base - Module Functions/Definitions
	* Color - Color Data Structure
	* Font
	* Group
	* Image - Image Data Structure
	* Plugin
	* Socket - Object Orientated Socketing
	* Thread - Object Orientated Threading
	* Vector2 - 2d Vector Data Structure
	* Vector3
	* Quaternion
	* Matrix4x4
* ce_frontend
	* AppFrontend - OS Specific Frontend App
	* Canvas - Frontend Window Object
	* Event - Event Structure
	* Frontend - Module Functions/Definitions
	* KeyCode
	* Keyboard
	* ScanCode
* ce_game2D
	* Camera
	* CameraView2DCtrl
	* DefaultPhysicsHandler
	* Entity - Abstract object
	* Graph
	* PhysicalGroup
	* PhysicalObject
	* PhysicsHandler
	* Sprite
	* TileMap
	* Trigger
* ce_ui
	* ButtonCtrl
	* ColorCtrl
	* Control - Abstract UI Control Class
	* ImageCtrl
	* TextCtrl
	* TextEditCtrl
	* UI - Module Functions/Definitions
* ce_plugBox2D
	* Base
	* PhysicsHandler

TO-DO Features
--------------

* ce_base
	* Matrix3x3
* ce_gameBase
	* AIController
	* GameConnection
* ce_game3D
	* Asset
	* GameView3D
* ce_ui
	* Control
  	* Window
  	* List
* ce_plugBullet - physics handler
* ce_plugAssimp - asset loading handler
* ce_plugDevIL - image loading handler
* ce_plugFreeType - font loading handler

Dependencies
------------

* ce_base
	* OpenAL
	* DevIL
	* FreeType2
* ce_plugBox2d
	* Box2d
* xcb
	* xsltproc
