# LBFFS
LBFFS is a flow simulator based on Lattice Boltzmann Method, written in C++ and OpenCL languages to run on a single GPU

## Features
* D3Q19 discrete velocity model
* Recursive regularized collision model
* Large eddy simulation based on Smagorinsky sub-grid scale model with damping near walls
* On-lattice boundary condition on wall by half-way Bounce-Back method
* Off-lattice boundary condition on wall by Filippova & Hanel’s Interpolated Bounce-Back method
* Off-lattice boundary setting by importing a STL file
* Outlet boundary condition which suppresses wave reflections [Geier et al., Comput. Math. Appl. (2015), Appendix F]
* Spongezones which suppress wave reflections at outlet boundaries

## Test cases
* Lid driven cavity flow (Re=100)
<table>
<tr>
<td>Velocity vector</td>
<td>u profile at x=0.5m</td>
</tr>
<tr>
<td><img src="https://user-images.githubusercontent.com/109857341/180638527-6905b752-ebff-4695-a5c2-aacec47b16ac.png" width="320px"></td>
<td><img src="https://user-images.githubusercontent.com/109857341/180638616-66064f75-6159-4655-a28d-1c7f0be1dcc7.png" width="320px"></td>
</tr>
</table>

* Poiseuille flow
* Flow around a cylinder
* Backward facing step flow
* Flow around a car
