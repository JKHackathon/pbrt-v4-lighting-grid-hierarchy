#pragma once
#include "Vector3f.h"
#include <vector>
#include <memory>
#include <algorithm>   // std::nth_element

// ─────────────────────────────────────────────────────────────────────────────
// KDNode with a stable index "idx"  (needed for shadow_cubes[level][idx])
// ─────────────────────────────────────────────────────────────────────────────
struct KDNode {
    Vector3f                point;      // world-space position
    float                   intensity;  // I_j  (luminance or energy)
    int                     idx = -1;   // stable slot in nodesFlat_
    std::unique_ptr<KDNode> left;
    std::unique_ptr<KDNode> right;

    KDNode(const Vector3f& pos, float I, int index)
        : point(pos), intensity(I), idx(index) {}
};

// ─────────────────────────────────────────────────────────────────────────────
// Simple KD-tree for point lights
// ─────────────────────────────────────────────────────────────────────────────
class KDTree {
public:
    using LightPair = std::pair<Vector3f, float>;

    // Build from a list of {position, intensity}
    explicit KDTree(const std::vector<LightPair>& lights) {
        nodesFlat_.reserve(lights.size());
        root_ = build(lights, 0);
    }

    // Return *all* nodes within "radius" of target
    void radiusSearch(const Vector3f&          target,
                      float                    radius,
                      std::vector<KDNode*>&    results) const
    {
        radiusSearchRecursive(root_.get(), target,
                              radius * radius, 0, results);
    }

    // Flat, array-style access used by LGH
    size_t              size() const                { return nodesFlat_.size(); }
    KDNode*             at(size_t i) const          { return nodesFlat_[i]; }

private:
    // ------------------------------------------------------------------
    std::unique_ptr<KDNode> build(const std::vector<LightPair>& pts, int depth)
    {
        if (pts.empty()) return nullptr;

        const int    axis   = depth % 3;
        const size_t median = pts.size() / 2;

        std::vector<LightPair> ptsCopy = pts;
        std::nth_element(ptsCopy.begin(), ptsCopy.begin() + median, ptsCopy.end(),
            [axis](const LightPair& a, const LightPair& b) {
                return a.first[axis] < b.first[axis];
            });

        int     myIdx = static_cast<int>(nodesFlat_.size());
        auto    node  = std::make_unique<KDNode>(
                            ptsCopy[median].first,
                            ptsCopy[median].second,
                            myIdx);

        nodesFlat_.push_back(node.get());   // keep stable pointer

        std::vector<LightPair> leftPts (ptsCopy.begin(),            ptsCopy.begin() + median);
        std::vector<LightPair> rightPts(ptsCopy.begin() + median + 1, ptsCopy.end());

        node->left  = build(leftPts,  depth + 1);
        node->right = build(rightPts, depth + 1);
        return node;
    }

    void radiusSearchRecursive(KDNode*               node,
                               const Vector3f&       target,
                               float                 radius2,
                               int                   depth,
                               std::vector<KDNode*>& results) const
    {
        if (!node) return;

        if (target.distanceSquared(node->point) <= radius2)
            results.push_back(node);

        const int axis = depth % 3;
        float diff     = target[axis] - node->point[axis];

        KDNode* nearChild  = diff <= 0 ? node->left.get()  : node->right.get();
        KDNode* farChild   = diff <= 0 ? node->right.get() : node->left.get();

        radiusSearchRecursive(nearChild, target, radius2, depth + 1, results);
        if (diff * diff <= radius2)
            radiusSearchRecursive(farChild, target, radius2, depth + 1, results);
    }

    // ------------------------------------------------------------------
    std::unique_ptr<KDNode>   root_;
    std::vector<KDNode*>      nodesFlat_;   // indexable container
};
