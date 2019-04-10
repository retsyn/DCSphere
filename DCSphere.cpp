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

MTypeId DCSphereNode::id(0x00000231);

MObject DCSphereNode::aOutTranslateA;
MObject DCSphereNode::aOutTranslateB;
MObject DCSphereNode::aInTranslateA;
MObject DCSphereNode::aInTranslateB;
MObject DCSphereNode::aRadiusA;
MObject DCSphereNode::aRadiusB;
MObject DCSphereNode::aCollision;
MObject DCSphereNode::aWeightA;
MObject DCSphereNode::aWeightB;

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

	
	data.setClean(plug);

	return MS::kSuccess;
}


MStatus DCSphereNode::initialize()
{
	MStatus status;
	MFnNumericAttribute nAttr;

	return MS::kSuccess;
}


