#ifndef LIGHTING_GRID_HIERARCHY_H
#define LIGHTING_GRID_HIERARCHY_H

#include "kdtree3d.h"
#include "pbrt/pbrt.h"
#include "pbrt/util/transform.h"
#include <nanovdb/NanoVDB.h>
#include <pbrt/samplers.h>
#include "CubeDS.h"

class LGH
{
public:
    LGH(const nanovdb::FloatGrid* temperature_grid, const nanovdb::FloatGrid* density_grid, int depth, float base_voxel_size, pbrt::Transform transform);

    pbrt::SampledSpectrum get_intensity(int L,
                                        Vector3f targetPos,
                                        KDNode* light,
                                        float radius,
                                        pbrt::SampledWavelengths lambda,
                                        pbrt::Sampler sampler,
                                        pbrt::Medium medium);

    pbrt::SampledSpectrum get_total_illum(pbrt::Point3f pos,
                                          pbrt::SampledWavelengths lambda,
                                          pbrt::Sampler sampler,
                                          pbrt::Medium medium,
                                          pbrt::RNG rng,
                                          float tMax,
                                          pbrt::Ray ray);

    const float TEMP_THRESHOLD = 100.f;
    const float TEMP_SCALE = 1500;

    const pbrt::Transform medium_transform;

    const float alpha = 1.0f;
    const int l_max;

    const nanovdb::FloatGrid* m_temperature_grid;
    const nanovdb::FloatGrid* m_density_grid;
    

private:
    void create_S0(const nanovdb::FloatGrid* temperature_grid);
    void deriveNewS(int l);//, KDTree S0);
    Vector3f calcNewPos(int l, Vector3f target_light_pos, std::vector<KDNode*> j_lights);
    float calcNewI(int l, Vector3f target_light_pos, std::vector<KDNode*> j_lights);

    float blendingFunction(int level, float distance, float r_l);
    CubeDS create_cube_map(int                level,
                        float              h,
                        const Vector3f&    light_pos,
                        float              step     = 0.01f,
                        float              sigma_t  = 1.0f);

    std::vector<KDTree*> lighting_grids;
    std::vector<float> h;

    Vector3f BBoxMin;
    Vector3f BBoxMax;
};


#endif // LIGHTING_GRID_HIERARCHY_H
