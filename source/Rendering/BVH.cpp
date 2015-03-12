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
			double volume0 = Child0->bounds.volume();
			double volume1 = Child1->bounds.volume();
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
	////handle duplicates
	//size_t nodeListSize = unsortedNodes.size();
	//for (size_t i = 0; i < nodeListSize; i++)
	//{
	//	for (size_t j = 0; j < nodeListSize; j++)
	//	{
	//		if (!(i == j) && unsortedNodes[i]->objectCenter == unsortedNodes[j]->objectCenter && unsortedNodes[i]->isUsed && unsortedNodes[j]->isUsed)
	//		{
	//			if ((unsortedNodes[i]->bounds.m_Min == unsortedNodes[j]->bounds.m_Min) &&
	//				(unsortedNodes[i]->bounds.m_Max == unsortedNodes[j]->bounds.m_Max))
	//			{
	//				for (size_t k = 0; k < unsortedNodes[j]->Indices.size(); k++)
	//				{
	//					unsortedNodes[i]->Indices.push_back(unsortedNodes[j]->Indices[k]);
	//				}
	//				unsortedNodes[j]->isUsed = false;
	//			}
	//			else
	//			{
	//				if (unsortedNodes[i]->bounds.volume() >= unsortedNodes[j]->bounds.volume())
	//				{
	//					unsortedNodes[i]->appendSmallestVolume(unsortedNodes[j]);
	//					unsortedNodes[j]->isUsed = false;
	//				}
	//				else
	//				{
	//					unsortedNodes[j]->appendSmallestVolume(unsortedNodes[i]);
	//					unsortedNodes[i]->isUsed = false;
	//				}
	//			}
	//		}
	//	}
	//}
	//vector<bvhNode*> unsortedNonDuplicate;
	//for (size_t i = 0; i < nodeListSize; i++)
	//{
	//	if (unsortedNodes[i]->isUsed)unsortedNonDuplicate.push_back(unsortedNodes[i]);
	//}
	//recursivly generate tree
	scaleAndSplit(outerNode, &unsortedNodes, 4096);
}

//void BVH::scaleAndSplit(bvhNode *node, vector<bvhNode*> *unsortedSubs, int maxSubTreeDeph){
//	if (unsortedSubs->size() >0 && maxSubTreeDeph>0)
//	{
//		//Get Nodes AABB
//		vec3 *min = new vec3(unsortedSubs->at(0)->bounds.m_Min), *max = new vec3(unsortedSubs->at(0)->bounds.m_Max);
//		vec3 *boxScaleAcc = new vec3();
//		for (size_t i = 0; i < unsortedSubs->size(); i++){
//			if (unsortedSubs->at(i)->bounds.m_Min.x < min->x)min->x = unsortedSubs->at(i)->bounds.m_Min.x;
//			if (unsortedSubs->at(i)->bounds.m_Min.y < min->y)min->y = unsortedSubs->at(i)->bounds.m_Min.y;
//			if (unsortedSubs->at(i)->bounds.m_Min.z < min->z)min->z = unsortedSubs->at(i)->bounds.m_Min.z;
//
//			if (unsortedSubs->at(i)->bounds.m_Max.x > max->x)max->x = unsortedSubs->at(i)->bounds.m_Max.x;
//			if (unsortedSubs->at(i)->bounds.m_Max.y > max->y)max->y = unsortedSubs->at(i)->bounds.m_Max.y;
//			if (unsortedSubs->at(i)->bounds.m_Max.z > max->z)max->z = unsortedSubs->at(i)->bounds.m_Max.z;
//
//			boxScaleAcc->operator+=( unsortedSubs->at(i)->bounds.m_Max - unsortedSubs->at(i)->bounds.m_Min);
//		}
//		node->bounds = AABB(vec3(min->x, min->y, min->z), vec3(max->x, max->y, max->z));
//
//		vector<bvhNode*> *subs0 = new vector<bvhNode*>();
//		vector<bvhNode*> *subs1 = new vector<bvhNode*>();
//
//		boxScaleAcc->x /= max->x - min->x;
//		boxScaleAcc->y /= max->y - min->y;
//		boxScaleAcc->z /= max->z - min->z;
//		if (boxScaleAcc->x < boxScaleAcc->y && boxScaleAcc->x < boxScaleAcc->z)
//		{
//			double center = min->x + (max->x - min->x) / 2;
//			for (size_t i = 0; i < unsortedSubs->size(); i++)
//			{
//				if (unsortedSubs->at(i)->objectCenter.x < center)
//				{
//					subs0->push_back(unsortedSubs->at(i));
//				}
//				else
//				{
//					subs1->push_back(unsortedSubs->at(i));
//				}
//			}
//		}
//		else if (boxScaleAcc->y < boxScaleAcc->x && boxScaleAcc->y < boxScaleAcc->z)
//		{
//			double center = min->y + (max->y - min->y) / 2;
//			for (size_t i = 0; i < unsortedSubs->size(); i++)
//			{
//				if (unsortedSubs->at(i)->objectCenter.y < center)
//				{
//					subs0->push_back(unsortedSubs->at(i));
//				}
//				else
//				{
//					subs1->push_back(unsortedSubs->at(i));
//				}
//			}
//		}
//		else
//		{
//			double center = min->z + (max->z - min->z) / 2;
//			for (size_t i = 0; i < unsortedSubs->size(); i++)
//			{
//				if (unsortedSubs->at(i)->objectCenter.z < center)
//				{
//					subs0->push_back(unsortedSubs->at(i));
//				}
//				else
//				{
//					subs1->push_back(unsortedSubs->at(i));
//				}
//			}
//		}
//		delete boxScaleAcc;
//		boxScaleAcc = nullptr;
//		delete min;
//		min = nullptr;
//		delete max;
//		max = nullptr;
//		if (subs0->size() + subs1->size() <= 2)
//		{
//			node->isLeaf = true;
//		}
//
//		if (subs0->size() <= 1)
//		{
//			if (subs0->size() == 1)node->Indices.push_back(subs0->at(0)->Indices[0]);
//		}
//		else
//		{
//			node->Child0 = new bvhNode();
//			scaleAndSplit(node->Child0, subs0, maxSubTreeDeph-1);
//		}
//		if (subs1->size() <= 1)
//		{
//			if (subs1->size() == 1)node->Indices.push_back(subs1->at(0)->Indices[0]);
//		}
//		else
//		{
//			node->Child1 = new bvhNode();
//			scaleAndSplit(node->Child1, subs1, maxSubTreeDeph - 1);
//		}
//		delete subs1;
//		subs1 = nullptr;
//		delete subs0;
//		subs0 = nullptr;
//	}
//	else
//	{
//		node->isLeaf = true;
//		for (size_t i = 0; i < unsortedSubs->size(); i++)
//		{
//			for (size_t j = 0; j < unsortedSubs->at(i)->Indices.size(); j++)
//			{
//				node->Indices.push_back(unsortedSubs->at(i)->Indices[j]);
//			}
//		}
//	}
//}

void BVH::scaleAndSplit(bvhNode *node, vector<bvhNode*> *unsortedSubs, int maxSubTreeDeph)
{
	if (unsortedSubs->size() > 0)
	{
		//Get Nodes AABB
		vec3 *min = new vec3(unsortedSubs->at(0)->bounds.m_Min), *max = new vec3(unsortedSubs->at(0)->bounds.m_Max);
		for (size_t i = 0; i < unsortedSubs->size(); i++){
			if (unsortedSubs->at(i)->bounds.m_Min.x < min->x)min->x = unsortedSubs->at(i)->bounds.m_Min.x;
			if (unsortedSubs->at(i)->bounds.m_Min.y < min->y)min->y = unsortedSubs->at(i)->bounds.m_Min.y;
			if (unsortedSubs->at(i)->bounds.m_Min.z < min->z)min->z = unsortedSubs->at(i)->bounds.m_Min.z;

			if (unsortedSubs->at(i)->bounds.m_Max.x > max->x)max->x = unsortedSubs->at(i)->bounds.m_Max.x;
			if (unsortedSubs->at(i)->bounds.m_Max.y > max->y)max->y = unsortedSubs->at(i)->bounds.m_Max.y;
			if (unsortedSubs->at(i)->bounds.m_Max.z > max->z)max->z = unsortedSubs->at(i)->bounds.m_Max.z;
		}
		node->bounds = AABB(vec3(min->x, min->y, min->z), vec3(max->x, max->y, max->z));


		//Heuristic
		if (maxSubTreeDeph>0 && unsortedSubs->size()>2)
		{
			//find longest axis for box
			vec3 deltaBounds = node->bounds.m_Max - node->bounds.m_Min;
			bool xGreatest = abs(deltaBounds.x) >= abs(deltaBounds.y) && abs(deltaBounds.x) >= abs(deltaBounds.z);
			bool yGreatest = abs(deltaBounds.y) >= abs(deltaBounds.x) && abs(deltaBounds.y) >= abs(deltaBounds.z) && !xGreatest;
			bool zGreatest = abs(deltaBounds.z) >= abs(deltaBounds.y) && abs(deltaBounds.z) >= abs(deltaBounds.x) && !xGreatest && ! yGreatest;
			//find average triangle midpoint
			point3 nodesMid=point3(0,0,0);
			for (size_t i = 0; i < unsortedSubs->size(); i++){
				nodesMid += unsortedSubs->at(i)->objectCenter;
			}
			nodesMid /= unsortedSubs->size();

			//create 2 new triangle lists
			vector<bvhNode*> subs0;
			vector<bvhNode*> subs1;

			//for triangle list
			for (size_t i = 0; i < unsortedSubs->size(); i++)
			{
			// Put into tribuckets based on midpoint's side of
			// average point in the longest axis 
				if (xGreatest)
				{
					if (unsortedSubs->at(i)->objectCenter.x == nodesMid.x)
					{
						if (subs0.size() <= subs1.size())subs0.push_back(unsortedSubs->at(i));
						else subs1.push_back(unsortedSubs->at(i));
					}
					else if (unsortedSubs->at(i)->objectCenter.x <= nodesMid.x)
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
					if (unsortedSubs->at(i)->objectCenter.y == nodesMid.y)
					{
						if (subs0.size() <= subs1.size())subs0.push_back(unsortedSubs->at(i));
						else subs1.push_back(unsortedSubs->at(i));
					}
					else if (unsortedSubs->at(i)->objectCenter.y <= nodesMid.y)
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
					if (unsortedSubs->at(i)->objectCenter.z == nodesMid.z)
					{
						if (subs0.size() <= subs1.size())subs0.push_back(unsortedSubs->at(i));
						else subs1.push_back(unsortedSubs->at(i));
					}
					else if (unsortedSubs->at(i)->objectCenter.z <= nodesMid.z)
					{
						subs0.push_back(unsortedSubs->at(i));
					}
					else
					{
						subs1.push_back(unsortedSubs->at(i));
					}
				}
			}

			node->Child0 = new bvhNode();
			scaleAndSplit(node->Child0, &subs0, maxSubTreeDeph - 1);
			node->Child1 = new bvhNode();
			scaleAndSplit(node->Child1, &subs1, maxSubTreeDeph - 1);
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

void BVH::clear(){
	if (!(outerNode == nullptr))
	{
		outerNode->clearNode();
		delete outerNode;
		outerNode = nullptr;
	}
	unsortedIndices.clear();
}