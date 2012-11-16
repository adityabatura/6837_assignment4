1. Compile with make clean; make.

2. Collaborated with Kenny and Giuseppe. Helped me with lighting -- I didn't know about multiplying ambient light by a diffuse or texture color. I helped them a bit with the hit detection for Group.

3. No references other than lecture slides.

4. Yes. For scene3, the sphere is "fatter" than it should be. I thought it was normals, but plotting normals (-normals normals.bmp) showed the same normals as the solution, but additional things corresponding to the "fatter" regions. I suspect it has something to do with detecting additional hits... :(

I *think* the issue is I'm not passing in a clean new Hit object when doing the intersection for the transform. I probably should create Hit newHit(), then pass newHit() into the intersect(newRay, newHit, tmin) function instead of just h. Then given the newHit, compare it's t value with h.getT(). However, I tried something like that, and the lighting was way off, and I don't have time right now to figure that out :(

5. No extra credit.

6. As always, fun assignment. Debugging was a bit more annoying that previous ones, but seeing the results come together was awesome.
