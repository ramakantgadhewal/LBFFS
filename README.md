# LBFFS [English/[Japanese](README_ja.md)]
LBFFS is a flow simulator based on lattice boltzmann method, written in C++ and OpenCL languages to run on a single GPU

## Features
* D3Q19 discrete velocity model
* Recursive regularized collision model
* Large eddy simulation based on Smagorinsky sub-grid scale model with damping near walls
* On-lattice boundary condition on wall by half-way Bounce-Back method
* Off-lattice boundary condition on wall by Filippova & Hanel’s Interpolated Bounce-Back method
* Off-lattice boundary setting by importing a STL file
* Outlet boundary condition which suppresses wave reflections [Geier et al., Comput. Math. Appl. (2015), Appendix F]
* Spongezones which suppress wave reflections at outlet boundaries
* Immersed Boundary Method

## Run script on Google Colaboratory  
[Sample](runScriptOnColab.ipynb)

## Test cases
* Poiseuille flow (Re=100, Laminar)
<table>
<tr>
<td>Velocity distribution</td>
<td>u profile</td>
</tr>
<tr>
<td><img src="https://user-images.githubusercontent.com/109857341/180640617-7e83c0b4-61df-4ed4-ac4f-39554b86affe.png" width="320px"></td>
<td><img src="https://user-images.githubusercontent.com/109857341/194307518-df53ed6d-47fb-4a8c-a005-064abdea8af3.png" width="320px"></td>

</tr>
</table>

* Lid driven cavity flow (Re=100, Laminar)
<table>
<tr>
<td>Velocity vector</td>
<td>u profile at x=0.5m</td>
</tr>
<tr>
<td><img src="https://user-images.githubusercontent.com/109857341/180638527-6905b752-ebff-4695-a5c2-aacec47b16ac.png" width="320px"></td>
<td><img src="https://user-images.githubusercontent.com/109857341/194307248-1eb3bf34-bc92-4e1e-8275-79690664ef3e.png" width="320px"></td>

</tr>
</table>

* Flow around a cylinder (Re=100, Laminar)

https://user-images.githubusercontent.com/109857341/195817642-dc11b2af-c81f-4ab7-9215-0dbf3b18e614.mp4

<table>
<tr>
<td>Cx</td>
<td>Cy</td>
</tr>
<tr>
<td><img src="https://user-images.githubusercontent.com/109857341/195794865-2a9b0cf8-9825-48bc-81aa-85fd03c4bf30.png" width="320px"></td>
<td><img src="https://user-images.githubusercontent.com/109857341/195794938-69f6a37a-250e-42c5-a596-59c255b23292.png" width="320px"></td>
</tr>
</table>

* Flow around a moving cylinder (Laminar)

https://user-images.githubusercontent.com/109857341/184158102-c2e7cd08-8ab5-4cbc-9b9f-9c97f7118309.mp4

* Backward facing step flow (Re=5500, Turbulence)

https://user-images.githubusercontent.com/109857341/195793711-509993e8-f4a5-4960-bbc7-af8fb2dc5010.mp4

<img src="https://user-images.githubusercontent.com/109857341/195793783-89527b68-6543-470c-b5e6-5a5561d9a0c7.png" width="640px">


* Flow around a car (Actual physical properties, Turbulence)

https://user-images.githubusercontent.com/109857341/194307950-79486366-6146-4bf7-a8bc-0f12c478207e.mp4



## Licence
[BSD-3-Clause license](LICENSE)


