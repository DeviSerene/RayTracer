###########################################
#       Georgina Markey's Raytracer       #
#       s4914142@bournemouth.ac.uk        #
# https://github.com/DeviSerene/RayTracer #
###########################################
Threaded Raytracer on the CPU: including lighting, reflectivity, refraction/transparancy and shadows.
To run the program, open the Release folde and open RayTracer.exe
You are able to view the doxygen manual by opening html folder and then open index.html

###########################################
#                Controls                 #
###########################################
Left/Right: Change the threading method: SingleThreaded -> OpenMP -> MultiThreaded.
Up: Spawn a randomly generated Sphere.
Down: Remove a Sphere from the scene.
W: Move camera forwards.
S: Move camera backwards.
A: Move camera to the left.
D: Move camera to the right.
Esc: Close the program.

###########################################
#               Information               #
###########################################
In the top left of the screen, there will be text indicating what method is being used.
Below that will be the frames per second.
In multi-threading method, below the fps will be the amount of time it takes each thread to complete its workload in seconds.

