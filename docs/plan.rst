Vague Planning
===============

Implementation
----------------

motivation
~~~~~~~~~~~~~~~
increase our ability to have technical collaborations. open science. spread the use of the rosetta energy model. fun and profit.

arbitrary reweighting. spatial, etc... rosetta can't do that, and it would be hard to add. easy with everything more transparent and array based.

primitives
~~~~~~~~~~~~~~~
should be as simple as possible, arrays of primitives (atom, dipole, stub? Other?). For connectivity, simple, gpu friendly fixed-size adjacency lists? Are we ok with max 4 connections? If not, maybe boost graph. Easy to map to numpy - eigen homogeneous coordinates, could bitpack meta-data (atom type, etc...) in redundant fields like in rif.

nblist
~~~~~~~~~~~~~~~
Count pair ruins vectorizability? Maybe nblist needs to be baked in early…. Nonissue once nblist is there. Otherwise, do homogeneous calc  then subtract? How bad would precision issues be? Could mitigate some by “softening” some atom pairs -- eg N vs CA never generates too bad a clash. By res: ragged array style

Nblist Use getneighbors template func? BVH? Capsules? Make updatable within bounds like rosetta nblist? Would be efficient for mostly rigid bodies. For folding, talk to Rasmus Fonseca about chaintrees.

smart updating
~~~~~~~~~~~~~~~
Smart updates - at some point, will want a general purpose caching api for precalc data. Want to keep basic model as simple as possible,  nblist, “domainmap”. Don't need right away, but keep in mind. Precalc grids/etable could be part of this.

approximations
~~~~~~~~~~~~~~~
grid-based, interpolated and not. etables. reduced atom-type sets (fa-elec). FFT wtih dima

vectorization / GPU
~~~~~~~~~~~~~~~
should look into upcoming c++ parallelism TS... if there's an early implementation, should probably use it. supposed to handle vectorization.


derivs
~~~~~~~~~~~~~~~
RBDL seems good. implementation follows Roy Featherston's book. Have to ignore all the inertia stuff?

community involvement
----------------------
goal should be to eventually make rosetta use this API(?) use new, cool energy function developments as a carrot.

would need permission to use dunbrack libraries. or just make new ones? gotta be something in scikit learn that would help. Maybe Nathan Kutz would be into this. Call it multiscale discrete dimensionality reduction.

Jeff grey would buy into this project, esp. If the python interface is nice… pandas compatible energy tables. Might be able to get Sergey to help out.

vjay pande wants the rosette energy functuon

Energy model science
---------------------

redo hbonds
~~~~~~~~~~~~~~~
might be easier than porting what's in rosetta. can't be that hard.

redo dunbrack
~~~~~~~~~~~~~~~
ditto.