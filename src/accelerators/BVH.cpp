//
// Created by LEI XU on 11/28/18.
//

#include "BVH.h"
#include <algorithm>

namespace hikari {

    struct BVHBuildNode {
        //BVHBiildNode data
        Bounds bounds;
        BVHBuildNode *children[2];
        int splitAxis=0, firstPrimOffset=0, nPrimitives=0;
        // BVHBuildNode Public Methods
        BVHBuildNode(){
            bounds = Bounds();
            *children = nullptr ;
        }
        void InitLeaf(BVHBuildNode* node, int first, int n, const Bounds &b) {
            node->firstPrimOffset = first;
            node->nPrimitives = n;
            node->bounds = b;
            node->children[0] = node->children[1] = nullptr;
            // leafNodes++;

            // totalLeafNodes++;
            // totalPrimitives += n;
        }
        void InitInterior(BVHAccel* ptr, int axis, BVHBuildNode *c0, BVHBuildNode *c1) {
            children[0] = c0;
            children[1] = c1;
            bounds = Union(c0->bounds, c1->bounds);
            splitAxis = axis;
            nPrimitives = 0;
            // interiorNodes++;
        }
    };
        struct BVHPrimitiveInfo {
        BVHPrimitiveInfo() {}
        BVHPrimitiveInfo(size_t primitiveNumber, const Bounds &bounds)
                : primitiveNumber(primitiveNumber),
                  bounds(bounds),
                  centroid(.5f * bounds.pMin + .5f * bounds.pMax) {}
        size_t primitiveNumber;
        Bounds bounds;
        Vector3 centroid;
    };
    
    struct BucketInfo {
        int count = 0;
        Bounds bounds;
    };

    // BVHAccel Method Definitions 
    BVHAccel::BVHAccel(std::vector<std::shared_ptr<Primitive> > p, int maxPrimsInNode, SplitMethod splitMethod)
            : maxPrimsInNode(std::min(255, maxPrimsInNode)),
              splitMethod(splitMethod),
              primitives(std::move(p)) {
        if (primitives.empty()) return;
        // Build BVH from _primitives_

        // Initialize _primitiveInfo_ array for primitives
        std::vector<BVHPrimitiveInfo> primitiveInfo(primitives.size());
        for (size_t i = 0; i < primitives.size(); ++i)
            primitiveInfo[i] = {i, primitives[i]->WorldBound()};

        // Build BVH tree for primitives using _primitiveInfo_

        int totalNodes = 0;
        std::vector<std::shared_ptr<Primitive>> orderedPrims;
        orderedPrims.reserve(primitives.size());

        root = recursiveBuild(primitiveInfo, 0, primitives.size(), &totalNodes, orderedPrims);
        primitives.swap(orderedPrims);
        primitiveInfo.resize(0);
        // Compute representation of depth-first traversal of BVH tree
        //treeBytes += totalNodes * sizeof(LinearBVHNode) + sizeof(*this) + primitives.size() * sizeof(primitives[0]);
    }


    BVHBuildNode* BVHAccel::recursiveBuild(std::vector<BVHPrimitiveInfo> &primitiveInfo, int start, int end, int *totalNodes, std::vector<std::shared_ptr<Primitive>> &orderedPrims)
    {
        BVHBuildNode* node = new BVHBuildNode();
        (*totalNodes)++;
        // Compute bounds of all primitives in BVH node
        Bounds bounds;
        for (int i = start; i < end; ++i)
            bounds = Union(bounds, primitiveInfo[i].bounds);
        int nPrimitives = end - start;
        if (nPrimitives == 1) {
            // Create leaf _BVHBuildNode_
            int firstPrimOffset = orderedPrims.size();
            for (int i = start; i < end; ++i) {
                int primNum = primitiveInfo[i].primitiveNumber;
                orderedPrims.push_back(primitives[primNum]);
            }
            node->InitLeaf(node,firstPrimOffset, nPrimitives, bounds);
            return node;
        }
        else {
            // Compute bound of primitive centroids, choose split dimension _dim_
            Bounds centroidBounds;
            for (int i = start; i < end; ++i)
                centroidBounds = Union(centroidBounds, primitiveInfo[i].centroid);
            int dim = centroidBounds.maxExtent();

            // Partition primitives into two sets and build children
            int mid = (start + end) / 2;
            if (centroidBounds.pMax[dim] == centroidBounds.pMin[dim]) {
                // Create leaf _BVHBuildNode_
                int firstPrimOffset = orderedPrims.size();
                for (int i = start; i < end; ++i) {
                    int primNum = primitiveInfo[i].primitiveNumber;
                    orderedPrims.push_back(primitives[primNum]);
                }
                node->InitLeaf(node,firstPrimOffset, nPrimitives, bounds);
                return node;
            } else {
                // Partition primitives based on _splitMethod_
                // Partition primitives using approximate SAH
                if (nPrimitives <= 2) {
                    // Partition primitives into equally-sized subsets
                    mid = (start + end) / 2;
                    std::nth_element(&primitiveInfo[start], &primitiveInfo[mid], &primitiveInfo[end - 1] + 1,
                                     [dim](const BVHPrimitiveInfo &a, const BVHPrimitiveInfo &b) {
                                         return a.centroid[dim] < b.centroid[dim]; });
                } else {
                    int nBuckets = 12;
                    BucketInfo buckets[nBuckets];
                    // Initialize _BucketInfo_ for SAH partition buckets
                    for (int i = start; i < end; ++i) {
                        int b = nBuckets * centroidBounds.Offset(primitiveInfo[i].centroid)[dim];
                        if (b == nBuckets) b = nBuckets - 1;
                        buckets[b].count++;
                        buckets[b].bounds = Union(buckets[b].bounds, primitiveInfo[i].bounds);
                    }
                    // Compute costs for splitting after each bucket
                    double cost[nBuckets - 1];
                    for (int i = 0; i < nBuckets - 1; ++i) {
                        Bounds b0, b1;
                        int count0 = 0, count1 = 0;
                        for (int j = 0; j <= i; ++j) {
                            b0 = Union(b0, buckets[j].bounds);
                            count0 += buckets[j].count;
                        }
                        for (int j = i + 1; j < nBuckets; ++j) {
                            b1 = Union(b1, buckets[j].bounds);
                            count1 += buckets[j].count;
                        }
                        cost[i] = 1 +
                                  (count0 * b0.SurfaceArea() + count1 * b1.SurfaceArea()) / bounds.SurfaceArea();
                    }
                    // Find bucket to split at that minimizes SAH metric
                    double  minCost = cost[0];
                    int minCostSplitBucket = 0;
                    for (int i = 1; i < nBuckets - 1; ++i) {
                        if (cost[i] < minCost) {
                            minCost = cost[i];
                            minCostSplitBucket = i;
                        }
                    }
                    // Either create leaf or split primitives at selected SAH
                    // bucket
                    int leafCost = nPrimitives;
                    if (nPrimitives > maxPrimsInNode || minCost < leafCost) {
                        BVHPrimitiveInfo *pmid = std::partition(
                                &primitiveInfo[start], &primitiveInfo[end - 1] + 1,
                                [=](const BVHPrimitiveInfo &pi) {
                                    int b = nBuckets * centroidBounds.Offset(pi.centroid)[dim];
                                    if (b == nBuckets) b = nBuckets - 1;
//                                        CHECK_GE(b, 0);
//                                        CHECK_LT(b, nBuckets);
                                    return b <= minCostSplitBucket;
                                });
                        mid = pmid - &primitiveInfo[0];
                    } else {
                        // Create leaf _BVHBuildNode_
                        int firstPrimOffset = orderedPrims.size();
                        for (int i = start; i < end; ++i) {
                            int primNum = primitiveInfo[i].primitiveNumber;
                            orderedPrims.push_back(primitives[primNum]);
                        }
                        node->InitLeaf(node,firstPrimOffset, nPrimitives, bounds);
                        return node;
                    }
                }
                node->InitInterior(this, dim,
                        recursiveBuild(primitiveInfo, start, mid, totalNodes, orderedPrims),
                        recursiveBuild(primitiveInfo, mid, end, totalNodes, orderedPrims));
            }
        }
        return node;
    }

    bool BVHAccel::Intersect(BVHBuildNode *node, const Ray &ray, Intersection* isect) const {

        if(root == nullptr) return false;

        Vector3 invDir(1 / ray.direction.x, 1 / ray.direction.y, 1 / ray.direction.z);
        int dirIsNeg[3] = {invDir.x < 0, invDir.y < 0, invDir.z < 0};
        //if intersect with this bounds
        if(node->bounds.IntersectP(ray,invDir,dirIsNeg))
        {
            //if it is a leaf node
            if(node->nPrimitives>0){
                for (int i = 0; i < node->nPrimitives; ++i){
                    Intersection* tmp = new Intersection();
                    if (primitives[node->firstPrimOffset + i]->Intersect(ray, tmp)){
                        isect->happened = true;
                        if(tmp->distance<isect->distance)
                        {
                            isect = tmp;
                        }
                    }
                }
                return isect->happened;
            }
            else //It is an interior node
                {
                Intersection *ins1 = new Intersection();
                if(Intersect(node->children[0],ray, ins1))
                {
                    isect = ins1;
                }
                Intersection *ins2 = new Intersection();
                if(Intersect(node->children[1],ray, ins2))
                {
                    if(ins2->distance < ins1->distance)
                        isect = ins2;
                }
                return isect->happened;
            }
        }
        else
            return isect->happened;
    }

    bool BVHAccel::IntersectP(BVHBuildNode *node, const Ray& ray) const 
    {
        Intersection isec=Intersection();
        if(!node)return false;
        Vector3 invDir(1 / ray.direction.x, 1 / ray.direction.y, 1 / ray.direction.z);
        int dirIsNeg[3] = {invDir.x < 0, invDir.y < 0, invDir.z < 0};
        //if intersect with this bounds
        if(node->bounds.IntersectP(ray,invDir,dirIsNeg)){
            //if it is a leaf node
            if(node->nPrimitives>0){
                for (int i = 0; i < node->nPrimitives; ++i){
                    Intersection* tmp = new Intersection();
                    if (primitives[node->firstPrimOffset + i]->Intersect(ray, tmp))
                    {
                        return true;
                    }
                }
                return false;
            }
            //else go to its two child nodes
            else{
                if(IntersectP(node->children[0],ray)) 
                {
                    return true;
                }
                else 
                {
                    return IntersectP(node->children[1],ray);
                }
            }
        }
        else 
            return false;
    }
}



