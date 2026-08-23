module Hopf.Core

%default total

||| Small host-side reference type.  The shader backend lowers its numeric
||| semantics to GLSL ES floats; Idris 2's primitive floating point is Double.
public export
record Vec3 where
  constructor MkVec3
  x : Double
  y : Double
  z : Double

public export
mapVec3 : (Double -> Double) -> Vec3 -> Vec3
mapVec3 f (MkVec3 x y z) = MkVec3 (f x) (f y) (f z)

public export
scaleVec3 : Double -> Vec3 -> Vec3
scaleVec3 k = mapVec3 (k *)

||| atan2 written in terms of Idris' primitive atan so the pure reference core
||| does not depend on a platform FFI binding.
public export
atan2 : Double -> Double -> Double
atan2 y x =
  if x > 0.0 then
    atan (y / x)
  else if x < 0.0 then
    if y >= 0.0
       then atan (y / x) + pi
       else atan (y / x) - pi
  else if y > 0.0 then
    pi / 2.0
  else if y < 0.0 then
    -(pi / 2.0)
  else
    0.0

||| Walczyk's coordinate convention, extracted from generate_fibration().
||| This is intentionally the reference formula, not yet a numerically guarded
||| version for the exact w = +/-1 boundary.
public export
walczykFiberPoint : Vec3 -> Double -> Vec3
walczykFiberPoint (MkVec3 a b c) phi =
  let theta = atan2 (-a) b - phi
      alpha = sqrt ((1.0 + c) / 2.0)
      beta  = sqrt ((1.0 - c) / 2.0)
      w     = alpha * cos theta
      qx    = alpha * sin theta
      qy    = beta * cos phi
      qz    = beta * sin phi
      radius = acos w / pi
      projection = radius / sqrt (1.0 - w * w)
   in MkVec3 (projection * qx)
             (projection * qy)
             (projection * qz)

||| Walczyk colors a fiber by its base point on S^2.
public export
fiberColor : Vec3 -> Vec3
fiberColor (MkVec3 a b c) =
  MkVec3 (a * 0.5 + 0.5)
         (b * 0.5 + 0.5)
         (c * 0.5 + 0.5)

||| Phase for a vertex shader using gl_VertexID.  The final sample repeats the
||| first so GL_LINE_STRIP closes the sampled circle geometrically.
public export
samplePhase : (sample : Nat) -> (samplesPerFiber : Nat) -> Double
samplePhase sample samplesPerFiber =
  case samplesPerFiber of
    Z => 0.0
    (S Z) => 0.0
    (S (S rest)) =>
      2.0 * pi * cast sample / cast (S rest)
