# The Manhattan Project

## Team Members
Varun Satheesh

Joel Kim

Aman Agarwal

Yuxin Ni

## Paper Implemented: Lighting Grid Hierarchy for Self-illuminating Explosions 
https://www.cemyuksel.com/research/lgh/lgh.pdf

#### Authors: 
Can Yuksel & Cem Yuksel

## Results

### Lighting Grid Heirarchy Output (Render Time (CPU): 5-10 mins): 
https://github.com/user-attachments/assets/8175e1d6-ace4-4990-bc25-64a943667423
- Primary improvement over base PBRT is in that of reduced noise and temporal flickering. While rendering speeds are comparable, for pbrt-v4 to remove this noise, sampling rate needs to be increased to values that are unreasonable to run on our devices.

### Base PBRT-v4 Output (Render Time (CPU): 5-10 mins, GPU: 5-15 secs): 
https://github.com/user-attachments/assets/6c972117-4b8b-4b7e-be71-1209b9fa4bd4

### Custom Pathtracer Output (GPU): (Render Time: 2 hours)
https://github.com/user-attachments/assets/fd0103cd-96e0-4f30-a9d2-221c40c5336f

GitHub Repo: https://github.com/aman190202/cuda

## Presentation: 
https://docs.google.com/presentation/d/1Av-D8AiICcnf_mRjBRvj3_hNDxfOkJupqBf9BdU79cs/edit?usp=sharing

### Cinematic Reveal: (Relevent rights to Christopher Nolan and Oppenheimer Team)
[oppen.mp4.zip](https://github.com/user-attachments/files/20151230/oppen.mp4.zip)

## Challenges and Known Bugs
PBRT provides an transmittance function that uses null-collision tracking and a majorant extinction coefficient to allow for adaptive, more efficient sampling along with unbiased calculations of transmittance, which is especially useful for heterogenous media like explosions. 
However, when using this transmittance computation in our LGH implementation, this resulted in no light attenuation which we likely attribute to be caused by the very short ray segments used in creating shadow cube maps. The shorter a ray segment is, the less likely a collision is sampled. 
To address this, we wrote our own transmittance function that simply marches through the ray and accumulates transmittance using Beer's Law, which gave us actual light attenuation.

We faced many challenges when trying to actually render the final results. As we completed each step of the paper implementation, we tested specific values step-size, depth of LGH, extinction coefficient, temperature scaling (primarily for accurate color), and light intensity scaling. 
However, the process of altering a single parameter and then rendering proved incredibly time consuming as occasionally, render times would spike for unknown reasons (likely due to how our systems managed CPU usage). Furthermor, the extinction coefficient and light intensity scaling initially produced ideal results 
that clearly displayed volume similarly to that of PBRT somehow no longer worked as intended when producing final renders. Note that no changes were made in between these steps, so it is unclear to us what caused this and attempting to resolve this issue reduced the amount of time we had to produce final scenes.

We also wanted to incorporate CUDA into the paper as it provides a massive speedup to rendering. PBRT-v4 also provides a GPU implementation, however, due to lack of documentation, it proved incredibly difficult to determine where to incorporate our existing CPU LGH implementation.
While the custom path tracer was produced in CUDA, the 3-dimensional KD-tree data structure used to store each grid level proved to be incredibly inefficient on GPU as it is recursive and hard to parallelize. Thus, in the future, we would like to alter our data structure to be more optimal for GPU speedup.


