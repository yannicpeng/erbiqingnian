
#include "renderElement.h"
#include "renderMaterialInstance.h"
#include "renderNode.h"
#include "renderMesh.h"

_NAMESPACE_BEGIN

RenderElement::RenderElement(void)
{
	m_parentNode		= 0;
	m_mesh				= 0;
	m_materialInstance	= 0;
}

RenderElement::RenderElement( const String& name )
{
	m_parentNode		= 0;
	m_mesh				= 0;
	m_materialInstance	= 0;
	m_name				= name;
}

RenderElement::~RenderElement(void)
{
}

RenderMaterial* RenderElement::getMaterial()
{
	return &m_materialInstance->getMaterial();
}

const Matrix4& RenderElement::getTransform( void ) const
{
	if (m_parentNode)
	{
		return m_parentNode->_getFullTransform();
	}
	else
	{
		return Matrix4::IDENTITY;
	}
}

void RenderElement::_notifyAttached( RenderNode* parent )
{
	m_parentNode = parent;
}

void RenderElement::setMaterialInstance( RenderMaterialInstance * val )
{
	if (m_materialInstance)
	{
		delete m_materialInstance;
	}
	m_materialInstance = val;
}

void RenderElement::setMesh( RenderMesh * val )
{
// 	if (m_mesh)
// 	{
// 		delete m_mesh;
// 	}
	m_mesh = val;
}

_NAMESPACE_END