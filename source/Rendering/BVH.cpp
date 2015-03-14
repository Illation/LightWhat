#include "BVH.hpp"


bvhNode::bvhNode()
{
}

bvhNode::~bvhNode()
{
	//withy comment by tom
}

void bvhNode::clearNode(){
	if (!(Child0 == nullptr))
	{
		Child0->clearNode();
	}
	delete Child0;
	Child0 = nullptr;
	if (!(Child1 == nullptr))
	{
		Child1->clearNode();
	}
	delete Child1;
	Child1 = nullptr;
}
void bvhNode::appendSmallestVolume(bvhNode *node)
{
	if (Child0 == nullptr)
	{
		Child0 = node;
	}
	else
	{
		if (Child1 == nullptr)
		{
			Child1 = node;
		}
		else
		{
			float volume0 = Child0->bounds.volume();
			float volume1 = Child1->bounds.volume();
			if (volume0<=volume1)
			{
				if (volume0 <= node->bounds.volume())
				{
					node->appendSmallestVolume(Child0);
				}
				else Child0->appendSmallestVolume(node);
			}
			else
			{
				if (volume1 <= node->bounds.volume())
				{
					node->appendSmallestVolume(Child1);
				}
				else Child1->appendSmallestVolume(node);
			}
		}
	}
}

BVH::BVH(){
}

BVH::~BVH(){
	clear();
}

void BVH::Build(Scene *scPtr){
	outerNode = new bvhNode();
	//get unsorted nodes
	m_ScenePtr = scPtr;
	vector<bvhNode*> unsortedNodes;
	for (size_t i = 0; i < scPtr->shapes.size(); i++)
	{
		Mesh *cMeshPtr = nullptr;
		bvhNode *lNode = new bvhNode();
		shapeType shTp = scPtr->shapes[i]->getType();
		shapeNodeIdx snIdx;
		switch (shTp){
		case PLANE:
			snIdx.shapeIdx = i;
			snIdx.hasSubShapes = false;
			unsortedIndices.push_back(snIdx);
			break;
		case SPHERE:
			lNode->bounds = scPtr->shapes[i]->getBoundingBox(0, 0);
			lNode->objectCenter = scPtr->shapes[i]->getObjectCenter(0, 0);
			lNode->isLeaf = true;
			snIdx.shapeIdx = i;
			snIdx.hasSubShapes = false;
			lNode->Indices.push_back(snIdx);
			unsortedNodes.push_back(lNode);
			break;
		case MESH: 
			cMeshPtr = (Mesh*)scPtr->shapes[i];
			for (size_t j = 0; j < (unsigned int)cMeshPtr->getPListCount(); j++){
				for (size_t k = 0; k < cMeshPtr->m_TriLists[j].triangles.size(); k++){
					bvhNode *lfNode = new bvhNode();
					lfNode->bounds = scPtr->shapes[i]->getBoundingBox(j, k);
					lfNode->objectCenter = scPtr->shapes[i]->getObjectCenter(j, k);
					lfNode->isLeaf = true;
					shapeNodeIdx snIdxTri;
					snIdxTri.shapeIdx = i;
					snIdxTri.hasSubShapes = true;
					snIdxTri.subIdx1 = j;
					snIdxTri.subIdx2 = k;
					lfNode->Indices.push_back(snIdxTri);
					unsortedNodes.push_back(lfNode);
				}
			}
			break;
		}
	}
	//recursivly generate tree
	buildSubTree(outerNode, &unsortedNodes, 4096);
}
void BVH::buildSubTree(bvhNode *node, vector<bvhNode*> *unsortedSubs, int maxSubTreeDeph)
{
	if (unsortedSubs->size() > 0)
	{
		//Get Nodes AABB
		node->bounds = nodeListBounds(unsortedSubs);


		//Heuristic
		if (maxSubTreeDeph>0 && unsortedSubs->size()>2)
		{
			//create 2 new triangle lists
			vector<bvhNode*> subsSplit0;
			vector<bvhNode*> subsSplit1;
			//find longest axis for box
			vec3 deltaBounds = node->bounds.m_Max - node->bounds.m_Min;
			//split into 2 lists
			Split(deltaBounds, unsortedSubs, subsSplit0, subsSplit1);
			//get bounding boxes for new splits
			AABB bounds0 = nodeListBounds(&subsSplit0);
			AABB bounds1 = nodeListBounds(&subsSplit1);
			//get new bounding box differentials
			vec3 deltaBounds0 = bounds0.m_Max - bounds0.m_Min;
			vec3 deltaBounds1 = bounds1.m_Max - bounds1.m_Min;
			//create 4 triangle lists for QBVH
			vector<bvhNode*> subs0;
			vector<bvhNode*> subs1;
			vector<bvhNode*> subs2;
			vector<bvhNode*> subs3;
			//split into 4 lists
			Split(deltaBounds0, &subsSplit0, subs0, subs1);
			Split(deltaBounds1, &subsSplit1, subs2, subs3);
			//delete intermediate lists
			subsSplit0.clear();
			subsSplit1.clear();

			//recursivly build tree for children
			node->Child0 = new bvhNode();
			buildSubTree(node->Child0, &subs0, maxSubTreeDeph - 1);
			node->Child1 = new bvhNode();
			buildSubTree(node->Child1, &subs1, maxSubTreeDeph - 1);
			node->Child2 = new bvhNode();
			buildSubTree(node->Child2, &subs2, maxSubTreeDeph - 1);
			node->Child3 = new bvhNode();
			buildSubTree(node->Child3, &subs3, maxSubTreeDeph - 1);
		}
		else
		{
			//child is null
			node->Child0 = nullptr;
			node->Child1 = nullptr;
			//put indices in this node
			for (size_t i = 0; i < unsortedSubs->size(); i++)
			{
				for (size_t j = 0; j < unsortedSubs->at(i)->Indices.size(); j++)
				{
					node->Indices.push_back(unsortedSubs->at(i)->Indices[j]);
				}
			}
		}
	}
}

AABB BVH::nodeListBounds(vector<bvhNode*> *unsortedSubs){
	vec3 *min = new vec3(unsortedSubs->at(0)->bounds.m_Min), *max = new vec3(unsortedSubs->at(0)->bounds.m_Max);
	for (size_t i = 0; i < unsortedSubs->size(); i++){
		if (unsortedSubs->at(i)->bounds.m_Min.x < min->x)min->x = unsortedSubs->at(i)->bounds.m_Min.x;
		if (unsortedSubs->at(i)->bounds.m_Min.y < min->y)min->y = unsortedSubs->at(i)->bounds.m_Min.y;
		if (unsortedSubs->at(i)->bounds.m_Min.z < min->z)min->z = unsortedSubs->at(i)->bounds.m_Min.z;

		if (unsortedSubs->at(i)->bounds.m_Max.x > max->x)max->x = unsortedSubs->at(i)->bounds.m_Max.x;
		if (unsortedSubs->at(i)->bounds.m_Max.y > max->y)max->y = unsortedSubs->at(i)->bounds.m_Max.y;
		if (unsortedSubs->at(i)->bounds.m_Max.z > max->z)max->z = unsortedSubs->at(i)->bounds.m_Max.z;
	}
	AABB ret = AABB(vec3(min->x, min->y, min->z), vec3(max->x, max->y, max->z));
	delete min;
	min = nullptr;
	delete max;
	max = nullptr;
	return ret;
}

void BVH::Split(vec3 deltaBounds, vector<bvhNode*> *unsortedSubs, vector<bvhNode*> &subs0, vector<bvhNode*> &subs1)
{
	//find longest axis for box
	bool xGreatest = abs(deltaBounds.x) >= abs(deltaBounds.y) && abs(deltaBounds.x) >= abs(deltaBounds.z);
	bool yGreatest = abs(deltaBounds.y) >= abs(deltaBounds.x) && abs(deltaBounds.y) >= abs(deltaBounds.z) && !xGreatest;
	bool zGreatest = abs(deltaBounds.z) >= abs(deltaBounds.y) && abs(deltaBounds.z) >= abs(deltaBounds.x) && !xGreatest && !yGreatest;
	//find average triangle midpoint
	double nodesMid = 0.0;
	if (xGreatest)for (size_t i = 0; i < unsortedSubs->size(); i++)
		nodesMid += (double)unsortedSubs->at(i)->objectCenter.x;
	if (yGreatest)for (size_t i = 0; i < unsortedSubs->size(); i++)
		nodesMid += (double)unsortedSubs->at(i)->objectCenter.y;
	if (zGreatest)for (size_t i = 0; i < unsortedSubs->size(); i++)
		nodesMid += (double)unsortedSubs->at(i)->objectCenter.z;
	nodesMid /= (double)unsortedSubs->size();


	//for triangle list
	for (size_t i = 0; i < unsortedSubs->size(); i++)
	{
		// Put into tribuckets based on midpoint's side of
		// average point in the longest axis 
		if (xGreatest)
		{
			if (unsortedSubs->at(i)->objectCenter.x == (float)nodesMid)
			{
				if (subs0.size() <= subs1.size())subs0.push_back(unsortedSubs->at(i));
				else subs1.push_back(unsortedSubs->at(i));
			}
			else if (unsortedSubs->at(i)->objectCenter.x <= (float)nodesMid)
			{
				subs0.push_back(unsortedSubs->at(i));
			}
			else
			{
				subs1.push_back(unsortedSubs->at(i));
			}

		}
		if (yGreatest)
		{
			if (unsortedSubs->at(i)->objectCenter.y == (float)nodesMid)
			{
				if (subs0.size() <= subs1.size())subs0.push_back(unsortedSubs->at(i));
				else subs1.push_back(unsortedSubs->at(i));
			}
			else if (unsortedSubs->at(i)->objectCenter.y <= (float)nodesMid)
			{
				subs0.push_back(unsortedSubs->at(i));
			}
			else
			{
				subs1.push_back(unsortedSubs->at(i));
			}
		}
		if (zGreatest)
		{
			if (unsortedSubs->at(i)->objectCenter.z == (float)nodesMid)
			{
				if (subs0.size() <= subs1.size())subs0.push_back(unsortedSubs->at(i));
				else subs1.push_back(unsortedSubs->at(i));
			}
			else if (unsortedSubs->at(i)->objectCenter.z <= (float)nodesMid)
			{
				subs0.push_back(unsortedSubs->at(i));
			}
			else
			{
				subs1.push_back(unsortedSubs->at(i));
			}
		}
	}
}

void BVH::clear(){
	if (!(outerNode == nullptr))
	{
		outerNode->clearNode();
		delete outerNode;
		outerNode = nullptr;
	}
	unsortedIndices.clear();
}
