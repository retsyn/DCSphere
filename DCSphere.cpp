/*
MIT License

Copyright(c) 2019 Matthew D. Riche

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "DCSphereNode.h"

bool SphereIntersect(MVector pointA, MVector pointB, float radiusA, float radiusB);



MTypeId DCSphereNode::id(0x00001966);
// Plugs
MObject DCSphereNode::aInPointA;
MObject DCSphereNode::aInPointB;
MObject DCSphereNode::aOutPointA;
MObject DCSphereNode::aOutPointB;

MObject DCSphereNode::aRadiusA;
MObject DCSphereNode::aRadiusB;

MObject DCSphereNode::aCollide;


DCSphereNode::DCSphereNode()
{
}

DCSphereNode::~DCSphereNode() 
{
}

void* DCSphereNode::creator()
{
	return new DCSphereNode();
}

MStatus DCSphereNode::compute(const MPlug& plug, MDataBlock& data)
{
	MStatus status;

	// If user attempts to request plug data that isn't relevant to our computation, we safely return.
	if ((plug != aOutPointA) || (plug != aOutPointB)) {
		return MS::kUnknownParameter;
	}

	MVector inPointAValue = data.inputValue(aInPointA, &status).asVector();
	MVector inPointBValue = data.inputValue(aInPointB, &status).asVector();
	// Maybe outputs shouldn't be in this part?
	MVector outPointAValue = data.inputValue(aOutPointA, &status).asVector();
	MVector outPointBValue = data.inputValue(aOutPointB, &status).asVector();

	bool collideFlag = data.inputValue(aCollide, &status).asBool();

	float radiusA = data.inputValue(aRadiusA, &status).asFloat();
	float radiusB = data.inputValue(aRadiusB, &status).asFloat();

	// Chec for sphere-sphere intersection first off.  If there's none, stop compute.
	if (SphereIntersect(inPointAValue, inPointBValue, radiusA, radiusB)) {
		// Do the displacement calculations
		collideFlag = true;

	}
	else {
		collideFlag = false;
	}

	MDataHandle hOutput = data.outputValue(aCollide, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	hOutput.setBool(collideFlag);
	hOutput.setClean();

	MDataHandle hOutPointA = data.outputValue(aOutPointA, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	hOutPointA.setMVector(outPointAValue);
	hOutPointA.setClean();

	MDataHandle hOutPointB = data.outputValue(aOutPointB, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	hOutPointB.setMVector(outPointBValue);
	hOutPointB.setClean();



	data.setClean(plug);

	return MS::kSuccess;
}

bool SphereIntersect(MVector pointA, MVector pointB, float radiusA, float radiusB)
{
	// Find the distance between centre points of A and B.
	MVector d = pointA - pointB;
	// Square the distance first to avoid excess computation from sqrt function.
	double dist2 = (d * d); 
	
	// Spheres intersect if squared distance is less than sum of radii.
	float radiusSum = radiusA + radiusB;

	return dist2 <= radiusSum * radiusSum;
}


MStatus DCSphereNode::initialize()
{
	MStatus status;
	MFnNumericAttribute nAttr;

	// Output plugs
	// Result A Translate
	aOutPointA = nAttr.createPoint("outPositionA", "outPositionA");
	nAttr.setWritable(false);
	nAttr.setStorable(false);
	addAttribute(aOutPointA);
	// Result B Translate
	aOutPointB = nAttr.createPoint("outPositionB", "outPositionB");
	nAttr.setWritable(false);
	nAttr.setStorable(false);
	addAttribute(aOutPointB);

	// In point plugs.
	// Each in put must affect both out points for correct DG propagation.

	// Input A Translate
	aInPointA = nAttr.createPoint("inPositionA", "inPositionA");
	nAttr.setKeyable(true);
	nAttr.setWritable(true);
	addAttribute(aInPointA);
	attributeAffects(aInPointA, aOutPointA);
	attributeAffects(aInPointA, aOutPointB);
	// Input B Translate
	aInPointB = nAttr.createPoint("inPositionB", "inPositionB");
	nAttr.setKeyable(true);
	nAttr.setWritable(true);
	addAttribute(aInPointB);
	attributeAffects(aInPointB, aOutPointA);
	attributeAffects(aInPointB, aOutPointB);


	// Radii plugs.  Just floats in this case.
	aRadiusA = nAttr.create("radiusA", "radiusA", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	nAttr.setWritable(true);
	addAttribute(aRadiusA);
	attributeAffects(aRadiusA, aOutPointA);
	attributeAffects(aRadiusA, aOutPointB);

	aRadiusB = nAttr.create("radiusB", "radiusB", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	nAttr.setWritable(true);
	addAttribute(aRadiusB);
	attributeAffects(aRadiusB, aOutPointA);
	attributeAffects(aRadiusB, aOutPointB);

	// Collide bool-- such that riggers can trigger other events upon collision.
	aCollide = nAttr.create("collide", "collide", MFnNumericData::kBoolean);
	nAttr.setWritable(false);
	nAttr.setStorable(false);
	addAttribute(aCollide);

	return MS::kSuccess;
}


