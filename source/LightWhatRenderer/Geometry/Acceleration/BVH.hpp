#pragma once
#include "AABB.hpp"
#include "../../Kernel/LWScene.hpp"

struct shapeNodeIdx{
	size_t shapeIdx;

	bool hasSubShapes;
	size_t subIdx1, subIdx2;
};
struct bvhNode
{
public:
	bvhNode();
	~bvhNode();

	void clearNode();
	void appendSmallestVolume(bvhNode *node);

	bool isUsed = true;
	AABB bounds;
	point3 objectCenter;
	bool isLeaf = false;
	bvhNode *Child0 = nullptr, *Child1 = nullptr , *Child2 = nullptr, *Child3 = nullptr;
	vector<shapeNodeIdx> Indices;
};
class BVH{
public:
	BVH();
	~BVH();

	void Build(LWScene *scPtr);
	void clear();

	bvhNode *outerNode = nullptr;
	vector<shapeNodeIdx> unsortedIndices;

private:
	void buildSubTree(bvhNode *node, vector<bvhNode*> *unsortedSubs, int maxSubTreeDeph);
	AABB nodeListBounds(vector<bvhNode*> *unsortedSubs);
	void Split(vec3 deltaBounds, vector<bvhNode*> *unsortedSubs, vector<bvhNode*> &subs0, vector<bvhNode*> &subs1);
	LWScene *m_ScenePtr;
};