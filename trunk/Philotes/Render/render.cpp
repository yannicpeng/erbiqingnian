
#include <render.h>
#include <renderDesc.h>
#include <renderMesh.h>
#include <renderMeshContext.h>
#include <renderMaterial.h>
#include <renderMaterialInstance.h>
#include <renderProjection.h>
#include <renderTarget.h>
#include <renderLight.h>
#include <algorithm>

#include "d3d9/D3D9Render.h"
#include "renderDesc.h"

_NAMESPACE_BEGIN

Render *Render::createRender(const RenderDesc &desc)
{
	Render *renderer = 0;
	const bool valid = desc.isValid();
	if(valid)
	{
		switch(desc.driver)
		{
			case DRIVER_DIRECT3D9:
			#if defined(RENDERER_ENABLE_DIRECT3D9)
				renderer = new D3D9Render(desc);
			#endif
				break;
		}
	}
	if(renderer && !renderer->isOk())
	{
		renderer->release();
		renderer = 0;
	}
	ph_assert2(renderer, "Failed to create renderer!");
	return renderer;
}

const char *Render::getDriverTypeName(DriverType type)
{
	const char *name = 0;
	switch(type)
	{
		case DRIVER_OPENGL:     name = "OpenGL";     break;
		case DRIVER_DIRECT3D9:  name = "Direct3D9";  break;
		case DRIVER_DIRECT3D10: name = "Direct3D10"; break;
		case DRIVER_LIBGCM:     name = "LibGCM";     break;
	}
	ph_assert2(name, "Unable to find Name String for Render Driver Type.");
	return name;
}

		
Render::Render(DriverType driver) :
	m_driver						(driver),
	m_deferredVBUnlock				(true)
{
	m_pixelCenterOffset = 0;
	setAmbientColor(RenderColor(64,64,64, 255));
    setClearColor(RenderColor(133,153,181,255));
	strncpy_s(m_deviceName, sizeof(m_deviceName), "UNKNOWN", sizeof(m_deviceName));
}

void Render::setVertexBufferDeferredUnlocking( bool enabled )
{
	m_deferredVBUnlock = enabled;
}

bool Render::getVertexBufferDeferredUnlocking() const
{
	return m_deferredVBUnlock;
}

Render::~Render(void)
{
}

void Render::release(void)
{
	delete this;
}

// get the driver type for this renderer.
Render::DriverType Render::getDriverType(void) const
{
	return m_driver;
}

// get the offset to the center of a pixel relative to the size of a pixel (so either 0 or 0.5).
scalar Render::getPixelCenterOffset(void) const
{
	return m_pixelCenterOffset;
}

// get the name of the hardware device.
const char *Render::getDeviceName(void) const
{
	return m_deviceName;
}

// adds a mesh to the render queue.
void Render::queueMeshForRender(RenderElement &mesh)
{
	ph_assert2( mesh.isValid(),  "Mesh Context is invalid.");
	ph_assert2(!mesh.isLocked(), "Mesh Context is already locked to a Render.");
	if(mesh.isValid() && !mesh.isLocked())
	{
		mesh.m_renderer = this;
		if (mesh.screenSpace)
		{
			m_screenSpaceMeshes.push_back(&mesh);
		}
		else switch (mesh.getMaterial()->getType())
		{
		case  RenderMaterial::TYPE_LIT:
			m_visibleLitMeshes.push_back(&mesh);
			break;
		default: //case RenderMaterial::TYPE_UNLIT:
			m_visibleUnlitMeshes.push_back(&mesh);
		//	break;
		}
	}
}

// adds a light to the render queue.
void Render::queueLightForRender(RenderLight &light)
{
	ph_assert2(!light.isLocked(), "Light is already locked to a Render.");
	if(!light.isLocked())
	{
		light.m_renderer = this;
		m_visibleLights.push_back(&light);
	}
}

// renders the current scene to the offscreen buffers. empties the render queue when done.
void Render::render(const Matrix4 &eye, const Matrix4 &proj, RenderTarget *target, bool depthOnly)
{
	RENDERER_PERFZONE(Render_render);
	const uint32 numLights = (uint32)m_visibleLights.size();
	if(target)
	{
		target->bind();
	}

	// TODO: Sort meshes by material..
	if(beginRender())
	{
		if(!depthOnly)
		{
			// What the hell is this? Why is there specialized code in here for a projection matrix...
			// YOU CAN PASS THE PROJECTION MATIX RIGHT INTO THIS FUNCTION!
			// TODO: Get rid of this.
			if (m_screenSpaceMeshes.size())
			{
				Matrix4 id = Matrix4::IDENTITY;
				Matrix4 pj;
				RenderProjection::makeProjectionMatrix(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f,pj);
				bindViewProj(id, pj);	//TODO: pass screen space matrices
				renderMeshes(m_screenSpaceMeshes, RenderMaterial::PASS_UNLIT);	//render screen space stuff first so stuff we occlude doesn't waste time on shading.
			}
		}
		
		if(depthOnly)
		{
			RENDERER_PERFZONE(Render_render_depthOnly);
			bindAmbientState(RenderColor(0,0,0,255));
			bindViewProj(eye, proj);
			renderMeshes(m_visibleLitMeshes,   RenderMaterial::PASS_DEPTH);
			renderMeshes(m_visibleUnlitMeshes, RenderMaterial::PASS_DEPTH);
		}
		else  if(numLights > RENDERER_DEFERRED_THRESHOLD)
		{
			RENDERER_PERFZONE(Render_render_deferred);
			bindDeferredState();
			bindViewProj(eye, proj);
			renderMeshes(m_visibleLitMeshes,   RenderMaterial::PASS_UNLIT);
			renderMeshes(m_visibleUnlitMeshes, RenderMaterial::PASS_UNLIT);
			renderDeferredLights();
		}
		else if(numLights > 0)
		{
			RENDERER_PERFZONE(Render_render_lit);
			bindAmbientState(m_ambientColor);
			bindViewProj(eye, proj);
			RenderLight &light0 = *m_visibleLights[0];
			light0.bind();
			renderMeshes(m_visibleLitMeshes, light0.getPass());
			light0.m_renderer = 0;
			
			bindAmbientState(RenderColor(0,0,0,255));
			beginMultiPass();
			for(uint32 i=1; i<numLights; i++)
			{
				RenderLight &light = *m_visibleLights[i];
				light.bind();
				renderMeshes(m_visibleLitMeshes, light.getPass());
				light.m_renderer = 0;
			}
			endMultiPass();
			renderMeshes(m_visibleUnlitMeshes, RenderMaterial::PASS_UNLIT);
		}
		else
		{
			RENDERER_PERFZONE(Render_render_unlit);
			bindAmbientState(RenderColor(0,0,0,255));
			bindViewProj(eye, proj);
			renderMeshes(m_visibleLitMeshes,   RenderMaterial::PASS_UNLIT);
			renderMeshes(m_visibleUnlitMeshes, RenderMaterial::PASS_UNLIT);
		}
		endRender();
	}
	if(target) target->unbind();
	m_visibleLitMeshes.clear();
	m_visibleUnlitMeshes.clear();
	m_screenSpaceMeshes.clear();
	m_visibleLights.clear();
}

// sets the ambient lighting color.
void Render::setAmbientColor(const RenderColor &ambientColor)
{
	m_ambientColor   = ambientColor;
	m_ambientColor.a = 255;
}

void Render::setClearColor(const RenderColor &clearColor)
{
	m_clearColor   = clearColor;
	m_clearColor.a = 255;
}

void Render::renderMeshes(std::vector<RenderElement*> & meshes, RenderMaterial::Pass pass)
{
	RENDERER_PERFZONE(Render_renderMeshes);
	
	RenderMaterial         *lastMaterial         = 0;
	RenderMaterialInstance *lastMaterialInstance = 0;
	const RenderMesh       *lastMesh             = 0;
	
	const uint32 numMeshes = (uint32)meshes.size();
	for(uint32 i=0; i<numMeshes; i++)
	{
		RenderElement &context = *meshes[i];
		bindMeshContext(context);
		bool instanced = context.mesh->getInstanceBuffer()?true:false;
		
		if(context.materialInstance && context.materialInstance != lastMaterialInstance)
		{
			if(lastMaterial) lastMaterial->unbind();
			lastMaterialInstance =  context.materialInstance;
			lastMaterial         = &context.materialInstance->getMaterial();
			lastMaterial->bind(pass, lastMaterialInstance, instanced);
		}
		else if(context.getMaterial() != lastMaterial)
		{
			if(lastMaterial) lastMaterial->unbind();
			lastMaterialInstance = 0;
			lastMaterial         = context.getMaterial();
			lastMaterial->bind(pass, lastMaterialInstance, instanced);
		}
		
		if(lastMaterial) lastMaterial->bindMeshState(instanced);
		if(context.mesh != lastMesh)
		{
			if(lastMesh) lastMesh->unbind();
			lastMesh = context.mesh;
			if(lastMesh) lastMesh->bind();
		}
		if(lastMesh) context.mesh->render(context.getMaterial());
		context.m_renderer = 0;
	}
	if(lastMesh)     lastMesh->unbind();
	if(lastMaterial) lastMaterial->unbind();
}


void Render::renderDeferredLights(void)
{
	RENDERER_PERFZONE(Render_renderDeferredLights);
	
	const uint32 numLights = (uint32)m_visibleLights.size();
	for(uint32 i=0; i<numLights; i++)
	{
		renderDeferredLight(*m_visibleLights[i]);
	}
}

_NAMESPACE_END