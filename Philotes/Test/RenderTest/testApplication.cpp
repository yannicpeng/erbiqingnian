
#include "testApplication.h"
#include "render.h"
#include "renderCamera.h"
#include "renderSceneManager.h"
#include "renderCellNode.h"
#include "renderTransform.h"
#include "renderDirectionalLightDesc.h"
#include "renderDirectionalLight.h"

#include "terrain/renderTerrain.h"

#include "debug/renderDebugLine.h"
#include "debug/renderDebugGrid.h"

#include "testCamera.h"

_NAMESPACE_BEGIN

TestApplication::TestApplication( const GearCommandLine& cmdline )
	: GearApplication(cmdline),
	m_debugLine(NULL),
	m_debugGrid(NULL)
{

}

TestApplication::~TestApplication()
{

}

void TestApplication::onInit( void )
{	
	srand(::GetTickCount());

	m_cameraMgr = new TestCameraManager(m_sceneManager->getCamera());
	m_cameraMgr->setStyle(CS_FREELOOK);
	m_cameraMgr->setTopSpeed(100);

	// ����line����
	m_debugLine = new RenderLineElement("dbg_line");

	Vector3 ptLast(Math::RangeRandom(-30,30),Math::RangeRandom(-30,30),Math::RangeRandom(-30,30));

	m_debugLine->clearLine();
	for (size_t i=0; i<100; i++)
	{
		Vector3 pt1(Math::RangeRandom(-30,30),Math::RangeRandom(-30,30),Math::RangeRandom(-30,30));
		
		m_debugLine->addLine(ptLast,pt1,Colour(1,0,0,1));
		ptLast = pt1;
	}

	RenderCellNode* cell = m_sceneManager->createCellNode("_dbgCell");
	m_sceneManager->getRootCellNode()->addChild(cell);
	RenderTransform* tn = cell->createChildTransformNode("line");
	//tn->attachObject(m_debugLine);
	tn->setPosition(0,30,0);

	m_debugGrid = new RenderGridElement(100,40);

	RenderDirectionalLightDesc lightdesc;
	lightdesc.intensity = 1;
	lightdesc.color     = Colour(0.5f,0.5f,0.5f,1);
	lightdesc.direction = Vector3(0.55f, -0.3f, 0.75f).normalisedCopy();
	m_dirlight = m_renderer->createLight(lightdesc);
	m_renderer->setAmbientColor(Colour());

	m_terrain = new RenderTerrain(m_sceneManager);
	TerrainDesc td;
	td.terrainSize = 513;
	td.batchSize = 65;
	td.worldSize = 12000.0f;
	td.layers.Append(TerrainLayer(100,"textures/dirt_grayrocky_diffusespecular.dds","textures/dirt_grayrocky_normalheight.dds"));
	td.layers.Append(TerrainLayer(30,"textures/grass_green-01_diffusespecular.dds","textures/grass_green-01_normalheight.dds"));
	td.layers.Append(TerrainLayer(200,"textures/growth_weirdfungus-03_diffusespecular.dds","textures/growth_weirdfungus-03_normalheight.dds"));
	m_terrain->prepareData(td);
	m_terrain->loadData();
}

void TestApplication::onShutdown( void )
{
	m_terrain->unloadData();
	delete m_terrain;
	m_terrain = NULL;

	m_dirlight->release();
	m_dirlight = NULL;

	delete m_cameraMgr;
	m_cameraMgr = NULL;

	delete m_debugLine;
	m_debugLine = NULL;

	delete m_debugGrid;
	m_debugGrid = NULL;
}

void TestApplication::onTickPreRender( float dtime )
{

}

void TestApplication::onRender( void )
{
	m_terrain->cull(m_sceneManager->getCamera());

	Render* renderer = getRender();

	if (renderer)
	{
		uint32 windowWidth  = 0;
		uint32 windowHeight = 0;
		getSize(windowWidth, windowHeight);
		if (windowWidth > 0 && windowHeight > 0)
		{
			renderer->clearBuffers();
			//renderer->queueMeshForRender(*m_debugGrid);
			renderer->queueLightForRender(*m_dirlight);

			// ��Ⱦ����
			renderer->render(m_sceneManager->getCamera());

			m_rewriteBuffers = renderer->swapBuffers();
		}
	}
}

void TestApplication::onTickPostRender( float dtime )
{

}

bool TestApplication::keyPressed( const KeyEvent &arg )
{
	m_cameraMgr->injectKeyDown(arg);
	return true;
}

bool TestApplication::keyReleased( const KeyEvent &arg )
{
	m_cameraMgr->injectKeyUp(arg);
	return true;
}

bool TestApplication::mouseMoved( const MouseEvent &arg )
{
	m_cameraMgr->injectMouseMove(arg);
	return true;
}

bool TestApplication::mousePressed( const MouseEvent &arg, MouseButtonID id )
{
	m_cameraMgr->injectMouseDown(arg,id);
	return true;
}

bool TestApplication::mouseReleased( const MouseEvent &arg, MouseButtonID id )
{
	m_cameraMgr->injectMouseUp(arg,id);
	return true;
}

_NAMESPACE_END
