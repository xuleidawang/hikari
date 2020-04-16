//
// Created by LEI XU on 11/28/18.
//

#ifndef PATHTRACER_BVH_H
#define PATHTRACER_BVH_H
// accelerators/bvh.h*

#include "hikari.h"
#include "Primitive.h"
//#include <atomic>

namespace  hikari{
    struct BVHBuildNode;
// BVHAccel Forward Declarations
    struct BVHPrimitiveInfo;
//    struct MortonPrimitive;
    struct LinearBVHNode;

// BVHAccel Declarations
    inline int leafNodes, totalLeafNodes, totalPrimitives, interiorNodes;
    class BVHAccel {

    public:

        // BVHAccel Local Declarations


        // BVHAccel Public Types
        enum class SplitMethod { SAH };

        // BVHAccel Public Methods
        BVHAccel(std::vector<std::shared_ptr<Shape>> p,
                 int maxPrimsInNode = 1,
                 SplitMethod splitMethod = SplitMethod::SAH);
        Bounds WorldBound() const;
        ~BVHAccel();

        bool IntersectP(const Ray &ray) const;
        bool Intersect(BVHBuildNode* node, const Ray &ray, Intersection* isect) const;
        Intersection getIntersection(BVHBuildNode* node, const Ray& ray)const;
        BVHBuildNode* root;


        // BVHAccel Private Methods
        BVHBuildNode* recursiveBuild(std::vector<BVHPrimitiveInfo> &primitiveInfo, int start,
                                     int end, int *totalNodes,
                                     std::vector<std::shared_ptr<Shape>> &orderedPrims);

        // BVHAccel Private Data
        const int maxPrimsInNode;
        const SplitMethod splitMethod;
        std::vector<std::shared_ptr<Shape>> primitives;
        LinearBVHNode *nodes = nullptr;
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
            leafNodes++;

            totalLeafNodes++;
            totalPrimitives += n;
        }
        void InitInterior(BVHAccel* ptr, int axis, BVHBuildNode *c0, BVHBuildNode *c1) {
            children[0] = c0;
            children[1] = c1;
            bounds = Union(c0->bounds, c1->bounds);
            splitAxis = axis;
            nPrimitives = 0;
            interiorNodes++;
        }
    };

    struct LinearBVHNode {
        Bounds bounds;
        union {
            int primitivesOffset;   // leaf
            int secondChildOffset;  // interior
        };
        uint16_t nPrimitives;  // 0 -> interior node
        uint8_t axis;          // interior node: xyz
        uint8_t pad[1];        // ensure 32 byte total size
    };

    struct BucketInfo {
        int count = 0;
        Bounds bounds;
    };

    void CreateBVHAccelerator(std::vector<std::shared_ptr<Shape>> prims);
}

#endif //PATHTRACER_BVH_H
