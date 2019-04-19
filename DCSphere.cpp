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
#include <math.h>

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

	// Relevant plugs to assigned to local vars.
	MFloatVector inPointA = data.inputValue(aInPointA, &status).asFloatVector();
	MFloatVector inPointB = data.inputValue(aInPointB, &status).asFloatVector();
	float radiusA = data.inputValue(aRadiusA, &status).asFloat();
	float radiusB = data.inputValue(aRadiusB, &status).asFloat();

	// Local vars that'll become output.
	bool collideFlag = false;
	MFloatVector outPointAValue;
	MFloatVector outPointBValue;
	MFloatVector ba;
	MFloatVector ab;
	double dist;
	double push;

	// Chec for sphere-sphere intersection first off.  If there's none, stop compute.
	if (SphereIntersect(inPointA, inPointB, radiusA, radiusB)) {
		// Do the displacement calculations
		collideFlag = true;
		
		// We need to move the outPoints away from each other.
		// First find vectors ba/ab existing between inPointA and inPointB,
		ba.x = inPointB.x - inPointA.x;
		ba.y = inPointB.y - inPointA.y;
		ba.z = inPointB.z - inPointA.z;
		ab.x = inPointA.x - inPointB.x;
		ab.y = inPointA.y - inPointB.y;
		ab.z = inPointA.z - inPointB.z;

		// Get the distance between points
		dist = sqrt(pow((inPointA.x - inPointB.x), 2) + pow((inPointA.y - inPointB.y), 2) + pow((inPointA.z - inPointB.z), 2));

		// Calculate each sphere's "push-away" distance
		push = ((radiusA + radiusB) - dist) / 2;

		// Move each sphere to a point along BA and AB that intersects a circle around the original points but with radius equal to "push"
	}
	else {
		collideFlag = false;
	}

	cout << collideFlag;

	// XXX get back to this part and repair it after the algorithm is written.
	MDataHandle hOutput = data.outputValue(aCollide, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	hOutput.setBool(collideFlag);
	hOutput.setClean();

	hOutput = data.outputValue(aOutPointA, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	hOutput.setMFloatVector(outPointAValue);
	hOutput.setClean();

	data.setClean(plug);

	return MS::kSuccess;
}


bool SphereIntersect(MVector pointA, MVector pointB, float radiusA, float radiusB)
{
	// Find the distance between centre points of A and B.
	MFloatVector d = pointA - pointB;
	// Square the distance first to avoid excess computation from sqrt function.
	double dist2 = (d * d);
	
	// Spheres intersect if squared distance is less than sum of radii.
	float radiusSum = radiusA + radiusB;

	return (dist2 <= radiusSum * radiusSum);
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
	attributeAffects(aInPointA, aCollide);

	// Input B Translate
	aInPointB = nAttr.createPoint("inPositionB", "inPositionB");
	nAttr.setKeyable(true);
	nAttr.setWritable(true);
	addAttribute(aInPointB);
	attributeAffects(aInPointB, aOutPointA);
	attributeAffects(aInPointB, aOutPointB);
	attributeAffects(aInPointB, aCollide);

	// Radii plugs.  Just floats in this case.
	aRadiusA = nAttr.create("radiusA", "radiusA", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	nAttr.setWritable(true);
	addAttribute(aRadiusA);
	attributeAffects(aRadiusA, aOutPointA);
	attributeAffects(aRadiusA, aOutPointB);
	attributeAffects(aRadiusA, aCollide);

	aRadiusB = nAttr.create("radiusB", "radiusB", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	nAttr.setWritable(true);
	addAttribute(aRadiusB);
	attributeAffects(aRadiusB, aOutPointA);
	attributeAffects(aRadiusB, aOutPointB);
	attributeAffects(aRadiusB, aCollide);

	// Collide bool-- such that riggers can trigger other events upon collision.
	aCollide = nAttr.create("collide", "collide", MFnNumericData::kBoolean);
	nAttr.setWritable(false);
	nAttr.setStorable(false);
	addAttribute(aCollide);

	return MS::kSuccess;
}


