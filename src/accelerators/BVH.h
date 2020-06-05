//
// Created by LEI XU on 11/28/18.
//

#pragma once
// accelerators/bvh.h*

#include "Primitive.h"
//#include <atomic>

namespace  hikari{
// BVHAccel Forward Declarations
    struct BVHBuildNode;
    struct BVHPrimitiveInfo;

// BVHAccel Declarations
    
    class BVHAccel {
        public:
        // BVHAccel Local Declarations
        // BVHAccel Public Types
        enum class SplitMethod { SAH };
        // static int leafNodes, totalLeafNodes, totalPrimitives, interiorNodes;

        // BVHAccel Public Methods
        BVHAccel(std::vector<std::shared_ptr<Primitive>> p, int maxPrimsInNode = 1, SplitMethod splitMethod = SplitMethod::SAH);
        Bounds WorldBound() const;
        ~BVHAccel();

        bool IntersectP(BVHBuildNode* node, const Ray &ray) const;
        bool Intersect(BVHBuildNode* node, const Ray &ray, Intersection* isect) const;
        BVHBuildNode* root;


        // BVHAccel Private Methods
        BVHBuildNode* recursiveBuild(std::vector<BVHPrimitiveInfo> &primitiveInfo, int start,
                                     int end, int *totalNodes,
                                     std::vector<std::shared_ptr<Primitive>> &orderedPrims);

        // BVHAccel Private Data
        const int maxPrimsInNode;
        const SplitMethod splitMethod;
        std::vector<std::shared_ptr<Primitive>> primitives;
        // LinearBVHNode *nodes = nullptr;
    };
    void CreateBVHAccelerator(std::vector<std::shared_ptr<Primitive>> prims);
}

