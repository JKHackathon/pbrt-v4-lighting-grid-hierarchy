#ifndef LIGHTING_GRID_HIERARCHY_H
#define LIGHTING_GRID_HIERARCHY_H

#include "kdtree3d.h"
#include "pbrt/pbrt.h"
#include "pbrt/util/transform.h"
#include <nanovdb/NanoVDB.h>
#include <pbrt/samplers.h>
#include <fstream>

class LGH
{
public:
    // TODO: separate into constructor and initialization function
    // TODO: fix inputs
    // LGH(pbrt::SampledGrid<float> temperature_grid, int depth, float base_voxel_size, float transmission);
    LGH(const nanovdb::FloatGrid* temperature_grid, int depth, float base_voxel_size, float transmission, pbrt::Transform transform);

    // pbrt::SampledSpectrum
    float get_intensity(int L,
                                        Vector3f targetPos,
                                        KDNode* light,
                                        float radius,
                                        pbrt::SampledWavelengths lambda,
                                        pbrt::Sampler sampler,
                                        pbrt::Medium medium);

    pbrt::SampledSpectrum get_total_illum(pbrt::Point3f pos,
                                          pbrt::SampledWavelengths lambda,
                                          pbrt::Sampler sampler,
                                          pbrt::Medium medium);

    const float TEMP_THRESHOLD = 1.0f;

    const pbrt::Transform medium_transform;


    // static void extract_lights(pbrt::SampledGrid<float> temperature_grid);

    const float alpha = 1.0f;
    const int l_max;

    // TODO: remove this to use pbrt transmission instead
    const float transmission;
    const nanovdb::FloatGrid* m_temperature_grid;

private:
    void create_S0(const nanovdb::FloatGrid* temperature_grid);
    void deriveNewS(int l);//, KDTree S0);
    Vector3f calcNewPos(int l, Vector3f target_light_pos, std::vector<KDNode*> j_lights);//const Vector3f& gv, int l, const KDTree& S0) const;
    float calcNewI(int l, Vector3f target_light_pos, std::vector<KDNode*> j_lights); //const Vector3f& gv, int l, const KDTree& S0) const;

    float blendingFunction(int level, float distance, float r_l);

    std::vector<KDTree*> lighting_grids;
    std::vector<float> h;

    Vector3f BBoxMin;
    Vector3f BBoxMax;

    // --- Shadow map support ---
    // For each level, store a vector of filtered densities (one per grid vertex)
    std::vector<std::vector<float>> filtered_densities;
    // For each level, store the grid vertex positions (to match filtered_densities)
    std::vector<std::vector<Vector3f>> grid_vertices;

    // Pre-filter the density field for a given level
    void prefilter_density_field(int level, float h_l, const nanovdb::FloatGrid* density_grid);
};

#endif // LIGHTING_GRID_HIERARCHY_H
