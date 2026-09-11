# Gromov — *Sign and Geometric Meaning of Curvature*

M. Gromov, *Rendiconti del Seminario Matematico e Fisico di Milano* 61 (1991), 9–123.

- Official Gromov/IHES page: https://www.ihes.fr/~gromov/expository/34/
- Official PDF: https://www.ihes.fr/~gromov/wp-content/uploads/2018/08/177.pdf
- IHES archive record: https://omeka.ihes.fr/document/M_90_95.pdf
- IHES archive terms: https://omeka.ihes.fr/mentions-legales

## Redistribution note

The paper is **freely readable from Gromov's official IHES page**, but that is not the same thing as an open-source/open-content grant. The IHES archive record marks the item `©IHES`, and the archive's legal page places attribution and non-commercial restrictions on its contents (using the label `BY NC NA`). I have therefore **not mirrored the PDF in this repository**. This file is an original reading guide and links to the official copy.

The IHES legal page is internally a little odd about the exact license abbreviation, so this note deliberately does not claim a particular Creative Commons license. If explicit redistribution rights for this specific paper turn up later, the PDF question can be revisited.

## What the paper is doing

Gromov's organizing problem is not “how do I calculate the curvature tensor?” but **what geometric phenomenon does the sign of curvature actually mean?** The curvature tensor is algebraically complicated; the paper repeatedly tries to turn tensor inequalities into pictures, distance comparisons, deformation rules, volume inequalities, or topology.

The recurring pattern is:

> infinitesimal sign condition → deformation/comparison principle → visible metric behavior → global geometric/topological consequence

The paper starts with ordinary convexity, where this program works almost perfectly, and then follows the same idea through sectional curvature, Ricci curvature, scalar curvature, curvature operators, and complexified curvature. As the curvature invariant becomes weaker or more averaged, the geometry becomes less directly visible and analytic machinery enters: minimal surfaces, spinors, Dirac operators, Bochner formulas, and harmonic maps.

This is a 1990/1991 expository paper. Some statements about what was then unknown are historical, not current status reports.

## Reading convention

Page numbers below are the **printed journal pages**. The PDF begins on printed p. 9. The fractional section numbers are Gromov's own: §½, §2½, §3½, §3⅔, §3¾, §6½, §7½, §7⅔, §7¾.

The equation notes below explain the **role** of the main displayed equations and recurring formulas. They are not a diplomatic transcription of every line of algebra; where the scan/OCR is ambiguous, the source PDF should control the exact sign convention.

---

## §0. The second fundamental form and convexity in Euclidean space — pp. 10–17

### Purpose

Build the model for the whole paper in the easiest setting. A sign on a second derivative means convexity; for a hypersurface, the corresponding second-order object is the second fundamental form. Gromov then replaces the local differential definition by an equidistant-deformation picture that can survive singularities and become global.

### Main equations and what they are for

- **First derivative sign ↔ monotonicity; second derivative sign ↔ convexity.** This is the one-dimensional toy model for interpreting a tensor sign synthetically.
- **Second fundamental form `II`.** Defined from the second-order normal departure of the hypersurface from its tangent plane. Purpose: isolate the first nonzero bending information after the tangent plane has removed the first-order part.
- **`II = (1/2) d/dε (g*ε)|ε=0`.** The induced metric on a parallel hypersurface changes as the surface is pushed normally. Purpose: reinterpret `II` as the first variation of metric under equidistant deformation.
- **`II(τ1,τ2) = g(Aτ1,τ2)`.** Introduces the shape operator `A`; its eigenvalues are the principal curvatures. Purpose: turn a bilinear form into an operator whose eigenvalues can be followed during deformation.
- **Euclidean tube/Riccati equation, schematically `dAε/dε = -Aε²` (with the paper's sign convention).** Purpose: principal curvature evolves by a simple ODE when the hypersurface is moved parallel to itself.
- **Principal-curvature evolution `λε ≈ λ0/(1+ελ0)` in the paper's convention.** Purpose: make focal blow-up and singularity formation visible as a denominator reaching zero.
- **Nearest-point projection to a closed convex hypersurface is distance non-increasing.** Purpose: turn infinitesimal convexity into a global metric statement.

### Figures

- **Fig. 1:** curve, tangent, normal, and second-order departure. The picture for what `II` measures.
- **Fig. 2:** choosing the interior/coorientation. Shows why the *sign* of `II` requires choosing which side is positive.
- **Fig. 3:** equidistant hypersurfaces. Shows normal offset as a geometric operation rather than a coordinate formula.
- **Fig. 4:** inward offsets of a nonround convex body. Shows that singularity formation generally occupies an interval of offset values, unlike the round sphere's single collapse time.

### Visual fragments

1. tangent line → quadratic normal gap;
2. flip the normal → flip curvature sign;
3. parallel offsets of a circle versus a nonround convex curve;
4. watch a principal radius shrink until a focal event occurs.

---

## §½. Generalized convexity — pp. 18–28

### Purpose

Ask how much of the convexity story survives after weakening “all principal curvatures nonnegative.” This produces mean convexity, `k`-convexity, pseudoconvexity, saddle behavior, immersed hypersurfaces, and a general local-to-global question about allowed principal-curvature patterns.

### Main equations and what they are for

- **Mean curvature `M = tr II = Σ λi`.** Purpose: replace full convexity by an averaged bending condition.
- **First variation of area/volume element under offset has sign controlled by `M`.** Purpose: give mean curvature a directly geometric meaning: outward offset locally expands area when mean curvature is positive.
- **For signed distance `δ`, mean convexity becomes a Laplacian inequality such as `Δδ ≥ 0` after the chosen sign convention.** Purpose: translate hypersurface geometry into a scalar PDE statement that still makes sense weakly near nonsmooth sets.
- **`k`-convexity: at least `k` principal curvatures have the required sign.** Purpose: interpolate between mean/partial convexity and ordinary convexity, then feed the sign information into Morse theory.
- **Principal-curvature map `Φ: W → R^(n-1)`, `w ↦ (λ1,…,λn-1)` (ordered).** Purpose: reformulate curvature restrictions as restrictions on the image, or distribution, of the list of principal curvatures. This is an early version of the paper's later “curvature lies in a cone” viewpoint.

### Figures

- **Fig. 5:** a locally convex immersed closed plane curve with self-intersections. Purpose: local convexity of an immersion does not mean its image is a globally convex set.
- **Fig. 6:** a “head-on collision” during a regular homotopy of slices. Purpose: show the event that a dimensional `k`-convexity condition rules out in a filling argument.
- **Fig. 7:** convex/concave versus saddle behavior. Purpose: visualize definite versus indefinite second fundamental form.

---

## §1. Recollection on length, distance and Riemannian metric — pp. 29–37

### Purpose

Rebuild Riemannian geometry from the metric side so curvature can later be recognized through distance rather than coordinates. The section explains how an infinitesimal quadratic form produces length and distance, and conversely how distance recovers the metric to second order.

### Main equations and what they are for

- **`length(c) = ∫ ||c'(t)||g dt`.** Purpose: integrate infinitesimal metric data into the length of a path.
- **`dist(x,y) = inf length(c)`.** Purpose: turn the local tensor `g` into a global metric.
- **Triangle equality along a minimizing geodesic.** Purpose: recognize geodesic segments from the distance function itself.
- **`gij = g(∂i,∂j)`.** Purpose: coordinate representation of the metric, preparatory to asking which derivatives are coordinate artifacts.
- **For `φ(x') = dist(x,x')²`, `Dφ(x)=0` and the Hessian at `x` recovers `g`.** Purpose: show that infinitesimal Riemannian metric and second-order behavior of squared distance are the same local information.
- **Jacobian/volume formulas, including `|Jac f| = sqrt(det(DD*))`.** Purpose: get the canonical Riemannian volume from the metric and prepare for volume comparison.
- **Geodesic coordinates satisfy first-derivative cancellation `∂k gij(x)=0`.** Purpose: remove first-order coordinate noise so second derivatives record genuine nonflatness.
- **Curvature-tensor formula from second derivatives of `g`, plus its symmetries and the first Bianchi identity.** Purpose: identify the coordinate-invariant remainder after the metric has been flattened to first order.
- **Flatness criterion `R=0` iff the metric is locally Euclidean.** Purpose: establish curvature as the obstruction to local Euclidean geometry.

No new numbered figures.

---

## §2. Equidistant deformation and the sectional curvature `K(V)` — pp. 38–45

### Purpose

Define sectional curvature using the same equidistant-deformation mechanism that made Euclidean convexity understandable. Instead of simply presenting the Riemann tensor, Gromov asks how the shape of nearby hypersurfaces accelerates as they move normally inside a curved ambient manifold.

### Main equations and what they are for

- **Normal exponential/equidistant map `dε: W → V`.** Purpose: create the one-parameter family whose metric and shape can be differentiated.
- **`II = (1/2) d/dε g*ε|0` in a curved ambient manifold.** Purpose: carry the Euclidean deformation definition over unchanged.
- **Choose `II=0` initially and differentiate the shape operator; call the derivative `BS`.** Purpose: isolate *ambient* curvature from initial bending of the hypersurface.
- **`K(σ) = -g(BS τ,τ)` for the 2-plane spanned by the normal and `τ` (paper's convention).** Purpose: define sectional curvature as the second-order effect of ambient geometry on equidistant hypersurfaces.
- **Sphere check `g*ε = cos²(ε) g0` for the relevant family on the unit sphere.** Purpose: calibrate the sign so the unit sphere has positive sectional curvature.
- **Scaling `K(RV) = R^-2 K(V)`.** Purpose: show curvature has dimensions of inverse length squared.
- **For a surface in Euclidean 3-space, Gaussian curvature = Jacobian of the Gauss map = product of principal curvatures.** Purpose: recover Gauss's intrinsic curvature from extrinsic bending and explain the convex/saddle sign visually.
- **Gauss equation `KW(σ) = KV(σ) + extrinsic term from II`.** Purpose: separate the curvature measured inside a hypersurface from ambient curvature plus bending of the embedding.
- **Riccati/tube equation in curved space: derivative of shape operator = quadratic self-term + ambient-curvature term.** Purpose: this is the engine for nearly all later comparison arguments. Exact signs depend on the coorientation convention.
- **Convexity criterion:** `K≥0` makes inward equidistant deformation preserve/improve convexity; `K≤0` gives the outward analogue. Purpose: a synthetic meaning of sectional-curvature sign.
- **Curvature operator `R` / quadratic form `Q` on `Λ²T`.** Purpose: package all sectional curvatures into one algebraic object and foreshadow §7.

No new numbered figures.

---

## §2½. Influence of `K(V)` on small balls in `V` — pp. 46–48

### Purpose

Replace tensors by the metric geometry of balls. Sectional-curvature sign controls how concentric balls scale compared with Euclidean balls.

### Main equations and what they are for

- **Ball-dilation comparison, schematically `B(v,λε) ≤ λ B(v,ε)` for `K≥0` in Gromov's Lipschitz-comparison notation; reversed for `K≤0`.** Purpose: express curvature sign entirely as a statement about distances inside small balls.
- **Definition of `B ≤ λB'` by existence of a Lipschitz map with constant `λ`.** Purpose: make “one ball is no larger than another” precise without coordinates.
- **Euclidean comparison of sufficiently small Riemannian balls.** Purpose: show how positive and negative curvature cause opposite second-order deviations from Euclidean scaling.
- **Banach-space exact scaling example.** Purpose: warn that a ball-scaling axiom which characterizes curvature in the Riemannian class need not force Euclidean structure among arbitrary metric spaces.

No new numbered figures.

---

## §3. Manifolds with positive sectional curvature — pp. 49–58

### Purpose

Push the local convexity law for `K≥0` into global geometry: convex boundaries remain convex while moving inward; singularities of distance levels can be controlled; repeated contraction leads to the soul picture; positive curvature also bounds diameter and topology.

### Main equations and what they are for

- **Inner parallel set `V_-ε = {v : dist(v,∂V) ≥ ε}`.** Purpose: perform global equidistant contraction of a domain.
- **Double-point singularity condition `dist(v,w') = dist(v,w'') = dist(v,W)`.** Purpose: identify one way a distance level becomes singular: two distinct nearest boundary points arrive together.
- **Focal-point criterion: the differential of the normal map loses injectivity; `||II||` blows up at first focal time.** Purpose: identify the infinitesimal version of the same collision.
- **Convex Contraction theorem (Gromoll–Meyer): for compact `V` with convex boundary and `K≥0`, all inner parallel sets remain convex.** Purpose: globalize the local tube equation through nonsmooth times.
- **Soul construction by repeated inward contraction.** Purpose: reduce a complete nonnegatively curved manifold to a compact totally geodesic core and recover the whole manifold as a normal bundle over it.
- **For a compact Lie group with a bi-invariant metric, `K(span{x,y}) = (1/4)||[x,y]||²` up to the normalization used.** Purpose: give an algebraic source of nonnegative curvature.
- **Bonnet-type diameter estimate: a positive lower curvature bound forces a finite upper diameter bound.** Purpose: show local positive curvature prevents indefinitely long minimizing geodesics.

### Figures

- **Fig. 8:** a minimizing path that hits the boundary. Purpose: detect failure of geodesic convexity.
- **Fig. 9:** locally convex immersed hypersurface which is not globally convex as an image. Purpose: separate local boundary convexity from global embedding behavior.
- **Figs. 10–11:** inward distance levels meeting themselves. Purpose: picture double-point singularities from multiple nearest boundary points.
- **Fig. 12:** piecewise-smooth convex approximation. Purpose: continue inward deformation through nonsmooth levels while keeping curvature controlled piecewise.
- **Fig. 13:** a smooth strictly convex support touching a nonsmooth convex set. Purpose: define strict convexity by barriers/supports at singular points.
- **Fig. 14:** geodesic spheres along a long minimizing segment. Purpose: visualize the tube-equation contradiction behind the diameter bound: curvature forces a focal/blow-up event before a minimizing segment can be too long.

---

## §3½. Distance function and Alexandrov–Toponogov theorem — pp. 59–60

### Purpose

Turn `K≥0` into a finite-point distance inequality. This is one of the paper's clearest successes at replacing a differential tensor condition by a purely metric statement.

### Main equations and what they are for

- **Distance-data map `MN: V^N → R^{N(N-1)/2}` sending an `N`-tuple to all pairwise distances.** Purpose: encode the metric geometry of finite configurations as points in Euclidean coordinate space.
- **Four-point comparison with one point lying on a minimizing segment.** Purpose: reduce curvature comparison to a small configuration that can be drawn.
- **Alexandrov–Toponogov inequality for `K≥0`: the corresponding point in `V` is at least as far from the opposite vertex as in the Euclidean comparison configuration.** Purpose: “fatter than Euclidean” triangles / concavity of distance, expressed without derivatives.
- **Infinitesimal converse.** Purpose: show this synthetic inequality recovers the original sectional-curvature lower bound in the smooth Riemannian setting.

### Figure

- **Fig. 15:** the four-point Euclidean comparison configuration. Purpose: put the Alexandrov–Toponogov inequality into one drawable diagram.

---

## §3⅔. Singular metric spaces with `K≥0` — pp. 61–62

### Purpose

Once Alexandrov–Toponogov is available as a distance axiom, drop smooth manifolds entirely. Curvature bounded below can be discussed for spaces with cone points, quotients, boundaries, and other singularities.

### Main equations and what they are for

- **Alexandrov–Toponogov is promoted from theorem to definition/axiom.** Purpose: define a curvature lower bound without a tangent bundle or curvature tensor.
- **Local almost-Euclidean estimate `(1-ε) ≤ distV/distEuclidean ≤ (1+ε)` on regular regions.** Purpose: quantify the idea that regular points of an Alexandrov space have Euclidean first-order geometry.

Examples include finite isometric quotients, convex subsets, convex hypersurfaces with intrinsic length, Euclidean cones, suspensions, products, and compact-group quotients.

No new numbered figures.

---

## §3¾. The sphere theorem and equidistant deformation of immersed hypersurfaces — pp. 63–69

### Purpose

Use pinched positive sectional curvature to force a global spherical topology. Gromov emphasizes a deformation/filling picture rather than treating the sphere theorem as an isolated comparison theorem.

### Main equations and what they are for

- **Pinching `c a < K < a`; after scaling, the critical case is `1/4 < K < 1`.** Purpose: measure how close all sectional curvatures are to one another, not merely whether they are positive.
- **`1/4` model:** complex/quaternionic projective spaces and the Cayley plane naturally realize the limiting interval `[1/4,1]`. Purpose: explain why the constant is geometrically sharp.
- **Sphere theorem: closed simply connected and `1/4 < K < 1` ⇒ homeomorphic to a sphere.** Purpose: a strong global topological consequence of a pointwise curvature interval.
- **Gauss–Bonnet in dimension 2, `∫ K dA = 2πχ(V)`.** Purpose: show the simplest possible conversion of local curvature into global topology.
- **Higher-dimensional Euler-form version `∫ Ω = χ(V)`.** Purpose: indicate the higher-dimensional analogue, while also showing that curvature-sign control becomes subtler.
- **Filling Lemma for a locally convex immersed hypersurface in `K>0`.** Purpose: construct the missing ball on the convex side; gluing it to an exponential-map ball produces a sphere covering `V`.

### Figures

- **Fig. 16:** a narrow sector in the tangent-space exponential map and its concentric slices. Purpose: apply the tube equation locally while ignoring unrelated self-intersections elsewhere on a geodesic sphere.
- **Fig. 17:** a self-intersecting locally convex plane curve. Purpose: show why the filling lemma needs ambient dimension at least 3.
- **Fig. 18:** inward equidistant deformation of an immersed locally convex hypersurface. Purpose: show the proposed filling process: positive curvature improves convexity until the hypersurface collapses to a point.

---

## §4. Negative sectional curvature — pp. 70–74

### Purpose

Run the sectional-curvature picture with the sign reversed. Outward parallel hypersurfaces stay convex; exponential maps spread distances; simply connected complete spaces become globally nonfolding. Then the discussion shifts toward large-scale topology and groups.

### Main equations and what they are for

- **`K≤0` iff small outward equidistant deformations preserve convexity.** Purpose: the sign-reversed synthetic counterpart of §3.
- **Exponential map is infinitesimally distance-increasing.** Purpose: show geodesics separate at least as fast as in Euclidean space.
- **Cartan–Hadamard: the universal cover of a complete `K≤0` manifold is diffeomorphic to `R^n`.** Purpose: local nonpositive curvature eliminates conjugate/focal folding on the simply connected cover.
- **Reverse ball-growth and Alexandrov–Toponogov comparisons.** Purpose: triangles become “thinner than Euclidean” and distance functions become more convex.
- **Splitting statements and flat factors.** Purpose: relate zero-curvature planes to product structure.

The later part uses these ideas to discuss Preissmann-type restrictions, flat tori, splitting, and the emergence of hyperbolic groups as a large-scale abstraction of strict negative curvature.

### Figure

- **Fig. 19:** outward equidistant deformation of a convex hypersurface. Purpose: the basic moving picture for `K≤0`.

---

## §5. Ricci curvature — pp. 75–86

### Purpose

Take the **trace** of the sectional-curvature/tube story. Sectional curvature controls every 2-plane; Ricci curvature controls the averaged behavior transverse to one direction. The clean geometric output is no longer individual convexity but **mean curvature, Jacobians, and volume growth**.

### Main equations and what they are for

- **Mean curvature `M(Wε) = tr Aε`.** Purpose: trace the shape operator so the deformation of an entire cross-sectional volume can be followed with one scalar.
- **Jacobian equation `dJ/dε = J tr Aε`, equivalently `d/dε log J = tr Aε`.** Purpose: convert mean curvature into the rate of volume distortion under the normal flow.
- **Ricci as the trace of the ambient curvature operator normal to a hyperplane: `tr BS = Ric(ν,ν)`.** Purpose: define Ricci as exactly the curvature term left after tracing the tube equation.
- **Traced tube equation `dM/dε = -tr(Aε²) - Ric(νε,νε)` up to the paper's orientation convention.** Purpose: basic ODE/inequality driving Ricci comparison geometry.
- **`Ric(ν,ν) = Σ K(ν,ei)` over an orthonormal transverse basis.** Purpose: explain Ricci as an average/sum of sectional curvatures containing the direction `ν`.
- **For `Ric≥0`, `d²/dε² log J ≤ 0`.** Purpose: normal volume distortion is log-concave; this is the traced analogue of convexity preservation.
- **Cauchy–Schwarz `tr(A²) ≥ (tr A)²/(n-1)`.** Purpose: close the traced tube equation into a scalar differential inequality.
- **For distance `r`, `Δr` equals mean curvature of a distance sphere where smooth.** Purpose: translate Ricci/mean-curvature geometry into a Laplacian comparison.
- **Bishop volume inequality, schematically `Vol B(λε) ≤ λ^n Vol B(ε)` for `Ric≥0`.** Purpose: positive Ricci slows volume growth relative to Euclidean space.
- **Positive lower Ricci bound compares ball growth to a round sphere and gives a diameter bound (Bonnet–Myers type).** Purpose: averaged positive curvature still limits global size.
- **Cheeger–Gromoll splitting: a complete `Ric≥0` space containing a line splits off an `R` factor.** Purpose: identify global rigidity forced by a perfectly straight infinite geodesic.
- **Abresch–Gromoll excess inequality.** Purpose: obtain a finer finite-distance restriction from `Ric≥0`; Gromov points toward a future synthetic Ricci theory involving both metric and measure.

The section also discusses Ricci-flat/Kähler examples, Einstein metrics, concentration of measure, and pseudoconvexity in complex geometry.

No new numbered figures; the text reuses the barrier idea of Fig. 13 and the four-point geometry of Fig. 15.

---

## §6. Positive scalar curvature — pp. 87–94

### Purpose

Trace once more. Scalar curvature is the most averaged of the three classical curvature signs. Its infinitesimal meaning is still visible in tiny-ball volume, but unlike sectional and Ricci curvature it does **not** simply integrate into a comparable global distance theory. This is where the paper decisively turns toward minimal surfaces and topology.

### Main equations and what they are for

- **`Sc = trg Ric = Σ K(ei,ej)` (with the paper's summation convention).** Purpose: scalar curvature is the total sectional curvature seen by an orthonormal frame.
- **Small geodesic-sphere Jacobian expansion `Jε = ε^(n-1)(1 - const·Ric(ν,ν) ε² + …)`.** Purpose: show Ricci as the first non-Euclidean correction in a fixed radial direction.
- **Small-sphere area and small-ball volume expansions with scalar-curvature correction.** Purpose: averaging the directional Ricci correction over the unit sphere produces `Sc`; positive scalar curvature makes sufficiently small balls have less volume than Euclidean balls of the same radius.
- **In dimension 3, `Sc = 2 K(TW) + 2 Ric(ν,ν)`.** Purpose: split scalar curvature into tangential and normal pieces along a surface.
- **Gauss equation `KW = KV(TW) + λ1λ2`, equivalently using `M² - tr A²`.** Purpose: replace ambient/tangential sectional curvature by the intrinsic Gaussian curvature of the surface plus its bending.
- **Second variation of area under normal deformation.** Purpose: combine scalar curvature, intrinsic Gaussian curvature, and shape terms into a single stability formula.
- **Gauss–Bonnet `∫W KW = 2πχ(W)`.** Purpose: turn the intrinsic curvature term in the stability formula into topology.
- **For an area-minimizing/minimal surface, first variation is zero and second variation is nonnegative.** Purpose: collide variational stability with `Sc≥0`; this forces restrictions on the topology of stable minimal surfaces.

This yields the Schoen–Yau route to topological obstructions to positive scalar curvature. The section also introduces Lichnerowicz's spinorial obstruction, then postpones its mechanism to §6½.

No new numbered figures.

---

## §6½. Spinors and the Dirac operator — pp. 95–100

### Purpose

Explain the second, very different route from `Sc>0` to topology. Instead of nonlinear minimal submanifolds inside `V`, use linear analysis on spinor bundles over `V`.

### Main equations and what they are for

- **Double cover `Spin(n) → SO(n)` and spin structure `Spin(V) → SO(V)`.** Purpose: provide the global geometric structure needed to define spinor bundles; obstruction is `w2(V)`.
- **Dirac operator `D+: Γ(S+) → Γ(S-)`, with adjoint `D-`.** Purpose: create an elliptic first-order operator whose index is topological but whose square sees curvature.
- **`ind D+ = dim ker D+ - dim ker D-`.** Purpose: produce a metric-independent integer/topological invariant from an operator built using the metric.
- **Bochner Laplacian `∇*∇` is nonnegative.** Purpose: supply the positive analytic term against which scalar curvature will be compared.
- **Lichnerowicz formula, in modern shorthand `D² = ∇*∇ + Sc/4`.** Purpose: put scalar curvature directly into an operator identity.
- **If `Sc>0`, a harmonic spinor must vanish.** Purpose: positivity of both terms in the Lichnerowicz formula forces the Dirac kernel to be zero.
- **Therefore the Dirac index / `Â`-genus vanishes under positive scalar curvature.** Purpose: convert a local curvature sign into a topological obstruction.
- **Twisted Dirac operators for almost-flat pulled-back bundles.** Purpose: strengthen “positive scalar curvature prevents large topology” into geometric size/enlargeability-type restrictions.

Gromov explicitly remarks that the geometric meaning of the Lichnerowicz identity itself remains obscure; that tension is part of the point of the paper.

No new numbered figures.

---

## §7. The curvature operator and related invariants — pp. 101–105

### Purpose

Return to the full curvature tensor but package it as a quadratic form `Q` or symmetric operator `R` on bivectors. This produces curvature positivity conditions stronger than positive sectional curvature and connects naturally to evolution equations and Bochner identities.

### Main equations and what they are for

- **Sectional curvature as `Q(τ∧ν, τ∧ν)` on decomposable bivectors.** Purpose: embed the familiar function on 2-planes into a quadratic form on all of `Λ²T`.
- **Curvature-operator positivity `Q(α,α) ≥ 0` for every bivector `α`.** Purpose: strengthen `K≥0`, which only tests decomposable bivectors.
- **Normalized Ricci flow `dg/dt = a(t)g - 2 Ric(g)` (and unnormalized `dg/dt = -2 Ric`).** Purpose: deform a metric by its own curvature while preserving useful curvature cones; in low dimensions Hamilton's analysis drives suitable positive metrics toward constant curvature.
- **Bochner–Weitzenböck formula `Δ = ∇*∇ + Rk` on `k`-forms.** Purpose: compare a natural Laplacian with a manifestly nonnegative rough Laplacian; their zero-order difference is curvature.
- **For 1-forms, `R1 = Ric*`.** Purpose: recover the Ricci tube/Laplacian story as the first case of a broader curvature-operator mechanism.
- **Positive curvature operator ⇒ positivity of the relevant `Rk` ⇒ vanishing of intermediate cohomology.** Purpose: turn operator positivity into a rational-homology-sphere conclusion.

The section ends by leading into Micallef–Moore: harmonic 2-spheres can detect a complexified curvature condition and give stronger sphere theorems.

No new numbered figures.

---

## §7½. Harmonic maps of surfaces and the complexified curvature `K_C` — pp. 106–116

### Purpose

Move the variational method from minimal hypersurfaces to maps. Curvature appears in the **second variation of energy**. For maps of surfaces, complex analysis supplies enough structured variation fields that positivity of a complexified curvature can force strong topological conclusions.

### Main equations and what they are for

- **Energy `E(f) = (1/2)∫W ||Df||²`.** Purpose: the functional whose critical points are harmonic maps.
- **Harmonic-map equation `Δf = 0`.** Purpose: nonlinear analogue of a harmonic function; for `W=S¹`, harmonic maps are constant-speed geodesics.
- **Second variation `δ²E = ∫ (||∇δ||² + curvature term)`.** Purpose: separate the stabilizing cost of varying the field from the way target curvature helps or hurts stability.
- **Curvature term expressed by the quadratic curvature form `Q` and `Df`.** Purpose: show exactly how the ambient curvature of the target enters energy stability.
- **For a parallel variation field along a closed geodesic, `δ²E` reduces essentially to minus an integral of sectional curvature.** Purpose: positive sectional curvature destabilizes certain minimizing closed geodesics; this feeds Synge's theorem.
- **Complex splitting of variation fields on an oriented surface.** Purpose: use the complex structure of a surface to manufacture many controlled variation fields even when real parallel fields do not exist.
- **Complexified curvature `K_C` / Hermitian extension of `Q`.** Purpose: identify the curvature sign actually tested by complex variations of harmonic 2-spheres.
- **Strict `1/4` pinching of sectional curvature ⇒ `K_C>0`.** Purpose: bridge the classical pinching hypothesis to the harmonic-map machinery.
- **Curvature term in the complex Hessian is `-4 K_C(...)` (in the paper's notation).** Purpose: positive complexified curvature creates negative directions in the Hessian of energy.
- **Micallef–Moore index argument.** Purpose: too many unstable directions for a nontrivial harmonic sphere are incompatible with the low-index spheres forced by topology, yielding a homotopy-sphere conclusion.

### Figure

- **Fig. 20:** bubbling of a map `S² → V` into several spherical components. Purpose: visualize the noncompactness obstructing naive energy minimization; energy can concentrate into bubbles while homotopy class splits among them.

This is a particularly good Short source: the actual figure is already an animation storyboard.

---

## §7⅔. Harmonic maps into manifolds with `K_C≤0` — pp. 117–119

### Purpose

Use the opposite sign to make energy minimization well behaved and then apply Bochner-type identities. Negative curvature of the target forces harmonic maps to become rigid.

### Main equations and what they are for

- **Eells–Sampson existence theorem for target `K≤0`.** Purpose: every homotopy class has a smooth energy-minimizing harmonic representative; negative curvature prevents the bubbling instability seen in Fig. 20.
- **Eells–Sampson formula `Δ||Df||² = ||Hess f||² + Curv`.** Purpose: compare change in energy density with a nonnegative Hessian term plus a curvature term.
- **Integrated over closed `W`, `∫Δ||Df||² = 0`.** Purpose: if the curvature term has the right sign, every nonnegative contribution must vanish, forcing rigidity of `f`.
- **Kähler identity/Hodge decomposition ingredients.** Purpose: exploit complex geometry on the source to sharpen ordinary harmonicity.
- **Complex Hessian `Hess_C` / pluriharmonicity condition.** Purpose: characterize maps whose restriction to every holomorphic curve is harmonic.
- **Integrated Siu–Sampson identity `∫||Hess_C||² - ∫K_C((Df)^4) = 0` (notation compressed).** Purpose: negative complexified curvature forces `Hess_C=0`; strict negativity also forces low rank.
- **Conclusion for `K_C<0`: harmonic maps from compact Kähler sources have rank at most 2.** Purpose: convert curvature sign into a strong topological restriction on what maps into the target can do.

No new numbered figures; Fig. 20 is explicitly reused as the phenomenon that `K≤0` prevents.

---

## §7¾. Metric classes defined by infinitesimal convex cones — pp. 120–121

### Purpose

Abstract the whole paper. Instead of naming one curvature invariant at a time, specify an orthogonally invariant subset/cone `C` in the vector space of algebraic curvature forms `Q`, and study metrics whose pointwise curvature lies in `C`.

### Main equations and what they are for

- **Condition `Qv ∈ C` for every point `v`.** Purpose: define a curvature class by one pointwise algebraic constraint.
- **`Sc≥0` corresponds to a very large half-space-type cone; `Q≥0`/positive curvature operator corresponds to a much smaller cone.** Purpose: organize the familiar curvature positivity notions by inclusion and strength.
- **Convexity and invariance of the cone under natural algebraic/differential operations.** Purpose: predict whether a curvature condition will interact well with heat flow, Bochner formulas, or deformation methods.

This last section makes explicit what much of the paper has implicitly been doing: searching for pointwise curvature regions whose algebraic shape survives a natural geometric process strongly enough to produce global consequences.

No new numbered figures.

---

# Figure index: what each picture is doing

1. **Fig. 1 — tangent/normal contact:** second fundamental form as quadratic departure from the tangent plane.
2. **Fig. 2 — chosen interior side:** coorientation makes curvature sign meaningful.
3. **Fig. 3 — parallel hypersurfaces:** equidistant deformation itself.
4. **Fig. 4 — singular inward offsets:** nonround convex bodies develop singular levels before extinction.
5. **Fig. 5 — immersed convex curve:** local convexity does not imply globally convex image.
6. **Fig. 6 — head-on collision:** forbidden event in the `k`-convex filling argument.
7. **Fig. 7 — convex versus saddle:** definite versus indefinite second fundamental form.
8. **Fig. 8 — geodesic hits boundary:** witness of nonconvexity of a domain.
9. **Fig. 9 — local but not global convexity:** an immersed/local boundary notion need not produce a globally convex image.
10. **Fig. 10 — inward pieces meet:** one mechanism for singular distance levels.
11. **Fig. 11 — another inward self-contact:** same multiple-nearest-point mechanism in a different geometry.
12. **Fig. 12 — piecewise convex approximation:** device for continuing contraction through nonsmooth stages.
13. **Fig. 13 — supporting convex barrier:** define strict convexity at a nonsmooth point.
14. **Fig. 14 — long minimizing geodesic:** geodesic spheres forced by positive curvature toward a focal/blow-up contradiction.
15. **Fig. 15 — four-point comparison:** the drawable core of Alexandrov–Toponogov.
16. **Fig. 16 — narrow exponential-map sector:** isolate a clean family of equidistant slices inside a potentially self-intersecting geodesic sphere.
17. **Fig. 17 — self-intersecting locally convex plane curve:** why the filling lemma's dimension restriction matters.
18. **Fig. 18 — shrink an immersed convex hypersurface:** geometric construction of the filling ball.
19. **Fig. 19 — outward convex offsets:** the basic picture of nonpositive sectional curvature.
20. **Fig. 20 — harmonic-map bubbling:** energy concentration breaks one sphere-map into several bubbles.

---

# The paper's conceptual ladder

For visualization work, the cleanest way to think about the whole paper is not by the section numbers but by what gets averaged:

1. **`II` / principal curvatures:** bending of one hypersurface.
2. **Sectional curvature `K`:** what ambient geometry does to one 2-plane / one normal slice.
3. **Ricci curvature:** trace over all sectional curvatures containing one direction → mean curvature and volume distortion.
4. **Scalar curvature:** trace again over directions → tiny-ball volume defect, but much weaker direct global metric control.
5. **Curvature operator / `K_C`:** reorganize the full tensor so it interacts with analytic processes such as Ricci flow, Bochner identities, and harmonic-map Hessians.

This progression explains why the first half of the paper is dominated by **moving surfaces, balls and distance**, while the latter half increasingly uses **operators, variational problems, and topology**.

# Strong Short candidates from the paper

The easiest fragments to animate without lying about the mathematics are:

1. **Fig. 3 + Fig. 4:** parallel inward offsets — circle versus nonround convex curve — and the birth of singularities.
2. **Fig. 1:** tangent line/plane versus second-order departure; what the second fundamental form actually sees.
3. **Fig. 15:** Euclidean comparison triangle versus a positively curved triangle; Alexandrov–Toponogov as a distance inequality.
4. **Fig. 19:** reverse the sign: outward offsets stay convex in `K≤0` geometry.
5. **Tiny-ball volume defect:** positive scalar curvature makes a sufficiently small geodesic ball slightly smaller than its Euclidean counterpart.
6. **Fig. 20:** harmonic-map bubbling. Gromov's own sequence of drawings is almost already a motion study.

For a first Gromov Short, **equidistant offsets and singularity formation** still has the best ratio of mathematical fidelity to visual immediacy.
