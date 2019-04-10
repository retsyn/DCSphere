#include "DCSphereNode.h"

#include <maya/MFnPlugin.h>

MStatus initializePlugin(MObject obj)
{
	MStatus status;

	MFnPlugin fnPlugin(obj, "Matthew D. Riche", "1.0", "Any");

	status = fnPlugin.registerNode("dcsphere",
		DCSphereNode::id,
		DCSphereNode::creator,
		DCSphereNode::initialize);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return MS::kSuccess;
}


MStatus uninitializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin fnPlugin(obj);

	status = fnPlugin.deregisterNode(DCSphereNode::id);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return MS::kSuccess;
}